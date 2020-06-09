//
// Created by Climber on 2020/6/9.
//

#include <fstream>
#include <sstream>
#include "utils.h"
#include "Climber.h"
#include "Configuration.h"
#include "ServerConfManager.h"
#include "Paths.h"

Climber *Climber::s_instance = nullptr;

void Climber::Init() {
    if (s_instance == nullptr) {
        s_instance = new Climber();
    }
}

void Climber::Destroy() {
    if (s_instance != nullptr) {
        s_instance->Stop();
        delete s_instance;
        s_instance = nullptr;
    }
}

Climber &Climber::GetInstance() {
    return *s_instance;
}

Climber::Climber() {

}

bool Climber::IsRunning() const {
    return m_running;
}

void Climber::Start() {
    if (m_running) return;

    // TODO 检查是否有残留的进程没有关闭

    int index = CONFIGURATION.GetSelectedServerIndex();
    if (index == -1) {
        wxMessageDialog(
                nullptr,
                _("Can't start Climber, please select a server first."),
                _("Error")).ShowModal();
        return;
    }

    auto *conf = SERVER_CONF_MANAGER.Get(index);
    switch (conf->GetType()) {
        // TODO other type
        case SERVER_TYPE_SS:
            break;
        case SERVER_TYPE_SSR:
            break;
        case SERVER_TYPE_VMESS:
            break;
        case SERVER_TYPE_TROJAN:
            RunTrojan(conf);
            break;
        default:
            printf("Unsupported type %s", conf->GetTypeName().c_str().AsChar());
            break;
    }

    wxMilliSleep(500);
    m_running = wxProcess::Exists(m_clientPid);
    if (!m_running) {
        wxMessageDialog(
                nullptr,
                _("Start Climber failed, please check log."),
                _("Error")).ShowModal();
    }

    if (m_running) {
        StartPrivoxy();
        SetSystemProxy();
    }
}

void Climber::Stop() {
    StopClient();
    StopPrivoxy();
    ClearSystemProxy();
    m_running = false;
}

void Climber::Restart() {
    Stop();
    Start();
}

void Climber::SetSystemProxy() {
    // TODO
}

void Climber::ClearSystemProxy() {
    // TODO
}

void Climber::StopClient() {
    if (m_clientPid == 0) return;
    // wxKill not work, don't know why
    killProcess(m_clientPid);
    if (wxFileExists(m_clientTmpConfigFile)) {
        wxRemoveFile(m_clientTmpConfigFile);
    }
    if (wxFileExists(m_clientLogFile)) {
        wxRemoveFile(m_clientLogFile);
    }
    m_clientPid = 0;
    m_clientTmpConfigFile = wxEmptyString;
    m_clientLogFile = wxEmptyString;
}

void Climber::StartPrivoxy() {
    // TODO 检查是否有残留的进程没有关闭

    int randNum = abs(rand());
    m_privoxyTmpConfigFile = Paths::GetTmpDirFile(wxString::Format("privoxy_%d.conf", randNum));
    m_privoxyPidFile = Paths::GetTmpDirFile(wxString::Format("privoxy_%d.pid", randNum));
    m_privoxyLogFile = Paths::GetLogDirFile(wxString::Format("privoxy_%d.log", randNum));
    auto privoxyConfigTplFile = Paths::GetAssetsDirFile("privoxy.conf");
    std::ifstream in(privoxyConfigTplFile.ToStdString(), std::ios::in);
    if (!in.is_open()) {
        printf("Open \"%s\" failed, %s\n", privoxyConfigTplFile.c_str().AsChar(), strerror(errno));
    }
    std::stringstream ss;
    ss << in.rdbuf();
    in.close();
    auto config = wxString(ss.str());
    config.Replace("__PRIVOXY_BIND_IP__", CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1");
    config.Replace("__PRIVOXY_BIND_PORT__", wxString::Format("%d", CONFIGURATION.GetHttpPort()));
    config.Replace("__PRIVOXY_LOG_FILE__", m_privoxyLogFile);
    config.Replace("__SOCKS_HOST__", "127.0.0.1");
    config.Replace("__SOCKS_PORT__", wxString::Format("%d", CONFIGURATION.GetSocksPort()));
    std::ofstream out(m_privoxyTmpConfigFile.ToStdString(), std::ios::out);
    if (!out.is_open()) {
        printf("Open \"%s\" failed, %s\n", m_privoxyTmpConfigFile.c_str().AsChar(), strerror(errno));
    }
    out << config.ToStdString();
    out.close();

#ifdef CLIMBER_WINDOWS
    auto privoxy = Paths::GetBinDirFile("privoxy.exe");
#endif
#ifdef CLIMBER_DARWIN
    auto privoxy = Paths::GetBinDirFile("privoxy");
#endif
    wxExecute(
            wxString::Format("\"%s\" --pidfile \"%s\" \"%s\"", privoxy, m_privoxyPidFile, m_privoxyTmpConfigFile),
            wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
}

void Climber::StopPrivoxy() {
    long privoxyPid = GetPrivoxyPid();
    if (privoxyPid == 0) return;
    // wxKill not work, don't know why
    killProcess(privoxyPid);
    if (wxFileExists(m_privoxyTmpConfigFile)) {
        wxRemoveFile(m_privoxyTmpConfigFile);
    }
    if (wxFileExists(m_privoxyLogFile)) {
        wxRemoveFile(m_privoxyLogFile);
    }
    if (wxFileExists(m_privoxyPidFile)) {
        wxRemoveFile(m_privoxyPidFile);
    }
    m_privoxyTmpConfigFile = wxEmptyString;
    m_privoxyLogFile = wxEmptyString;
    m_privoxyPidFile = wxEmptyString;
}

long Climber::GetPrivoxyPid() {
    if (m_privoxyPidFile.empty()) {
        return 0;
    }
    std::ifstream in(m_privoxyPidFile.ToStdString(), std::ios::in);
    if (!in.is_open()) {
        return 0;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    in.close();
    return std::strtol(ss.str().c_str(), nullptr, 10);
}

void Climber::RunTrojan(ServerConfItem *conf) {
    int randNum = abs(rand());
    m_clientTmpConfigFile = Paths::GetTmpDirFile(wxString::Format("trojan_%d.json", randNum));
    m_clientLogFile = Paths::GetLogDirFile(wxString::Format("trojan_%d.log", randNum));
    conf->SetLocalAddr(CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1");
    conf->SetLocalPort(CONFIGURATION.GetSocksPort());
    conf->WriteTo(m_clientTmpConfigFile);
#ifdef CLIMBER_WINDOWS
    auto trojan = Paths::GetBinDirFile("trojan.exe");
#endif
#ifdef CLIMBER_DARWIN
    auto trojan = Paths::GetBinDirFile("trojan");
#endif
    m_clientPid = wxExecute(
            wxString::Format("\"%s\" -c \"%s\" -l \"%s\"", trojan, m_clientTmpConfigFile, m_clientLogFile),
            wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
}
