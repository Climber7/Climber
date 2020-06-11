//
// Created by Climber on 2020/6/9.
//

#include "utils.h"
#include "Climber.h"
#include "Configuration.h"
#include "ServerConfManager.h"
#include "Paths.h"
#include "pac.h"

#ifdef CLIMBER_WINDOWS

#define CLIMBER_SHADOWSOCKS ("climber_shadowsocks_libev.exe")
#define CLIMBER_TROJAN ("climber_trojan.exe")
#define CLIMBER_PRIVOXY ("climber_privoxy.exe")

#elif defined CLIMBER_DARWIN

#define CLIMBER_SHADOWSOCKS ("climber_shadowsocks_libev")
#define CLIMBER_TROJAN ("climber_trojan")
#define CLIMBER_PRIVOXY ("climber_privoxy")

#endif

Climber *Climber::s_instance = nullptr;

void Climber::Init() {
    if (s_instance == nullptr) {
        s_instance = new Climber();
    }
}

void Climber::Destroy() {
    if (s_instance != nullptr) {
        s_instance->StopPacServer(false);
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

    KillClient();

    int index = CONFIGURATION.GetSelectedServerIndex();
    if (index == -1) {
        wxMessageDialog(
                nullptr,
                _("Can't start Climber, please select a server first."),
                _("Error")).ShowModal();
        return;
    }

    const auto *conf = SERVER_CONF_MANAGER.Get(index);
    switch (conf->GetType()) {
        // TODO other type
        case SERVER_TYPE_SS:
            RunShadowsocks(conf);
            break;
        case SERVER_TYPE_TROJAN:
            RunTrojan(conf);
            break;
        default:
            wxLogWarning("Unsupported type %s", conf->GetTypeName());
            break;
    }

    RunPrivoxy();
    StartPacServer();
    SetSystemProxy();

    m_running = true;
}

void Climber::Stop() {
    ClearSystemProxy();
    KillClient();
    KillPrivoxy();
    StopPacServer();
    m_running = false;
}

void Climber::Restart() {
    Stop();
    Start();
}

void Climber::SetSystemProxy() {
    const auto &mode = CONFIGURATION.GetProxyMode();
    if (mode == PROXY_MODE_DIRECT) {
        ClearSystemProxy();
    } else if (mode == PROXY_MODE_PAC) {
        setProxyPac(wxString::Format("http://127.0.0.1:%d/proxy.pac", CONFIGURATION.GetPacPort()),
                    CONFIGURATION.GetProxyBypass());
    } else if (mode == PROXY_MODE_GLOBAL) {
        setProxy("127.0.0.1", CONFIGURATION.GetSocksPort(),
                 "127.0.0.1", CONFIGURATION.GetHttpPort(),
                 CONFIGURATION.GetProxyBypass());
    }
}

void Climber::ClearSystemProxy() {
    clearProxy();
}

void Climber::RunShadowsocks(const ServerConfItem *conf) {
    auto clientTmpConfigFile = Paths::GetTmpDirFile("shadowsocks.json");
//    auto clientLogFile = Paths::GetLogDirFile("shadowsocks.log");
    auto localAddr = CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1";
    auto localPort = CONFIGURATION.GetSocksPort();
    conf->WriteTo(clientTmpConfigFile, localAddr, localPort);

    auto shadowsocks = Paths::GetBinDirFile(CLIMBER_SHADOWSOCKS);
    wxExecute(wxString::Format("\"%s\" -c \"%s\"", shadowsocks, clientTmpConfigFile),
              wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
    // TODO log
}

void Climber::RunTrojan(const ServerConfItem *conf) {
    auto clientTmpConfigFile = Paths::GetTmpDirFile("trojan.json");
    auto clientLogFile = Paths::GetLogDirFile("trojan.log");
    auto localAddr = CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1";
    auto localPort = CONFIGURATION.GetSocksPort();
    conf->WriteTo(clientTmpConfigFile, localAddr, localPort);

    auto trojan = Paths::GetBinDirFile(CLIMBER_TROJAN);
    wxExecute(wxString::Format("\"%s\" -c \"%s\" -l \"%s\"", trojan, clientTmpConfigFile, clientLogFile),
              wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
}

void Climber::KillClient() {
    killProcessByName(CLIMBER_SHADOWSOCKS);
    killProcessByName(CLIMBER_TROJAN);
}

void Climber::RunPrivoxy() {
    KillPrivoxy();

    auto privoxyTmpConfigFile = Paths::GetTmpDirFile("privoxy.conf");
    auto privoxyLogFile = Paths::GetLogDirFile("privoxy.log");
    auto privoxyConfigTplFile = Paths::GetAssetsDirFile("privoxy.conf");
    auto config = readTextFile(privoxyConfigTplFile);
    config.Replace("__PRIVOXY_BIND_IP__", CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1");
    config.Replace("__PRIVOXY_BIND_PORT__", wxString::Format("%d", CONFIGURATION.GetHttpPort()));
    config.Replace("__PRIVOXY_LOG_FILE__", privoxyLogFile);
    config.Replace("__SOCKS_HOST__", "127.0.0.1");
    config.Replace("__SOCKS_PORT__", wxString::Format("%d", CONFIGURATION.GetSocksPort()));
    writeTextFile(privoxyTmpConfigFile, config);

    auto privoxy = Paths::GetBinDirFile(CLIMBER_PRIVOXY);
    wxExecute(wxString::Format("\"%s\" \"%s\"", privoxy, privoxyTmpConfigFile),
              wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
}

void Climber::KillPrivoxy() {
    killProcessByName(CLIMBER_PRIVOXY);
}

void Climber::StartPacServer() {
    if (m_pacServerThread != nullptr) return;

    m_pacServerThread = new std::thread([&]() {
        auto pacTpl = readTextFile(Paths::GetAssetsDirFile("proxy.pac"));
        m_pacServer = new httplib::Server();
        m_pacServer->Get("/proxy.pac", [&](const httplib::Request &req, httplib::Response &res) {
            auto host = req.headers.find("Host")->second;
            std::string ip;
            auto startPos = host.find(":");
            if (startPos != wxNOT_FOUND) {
                ip = host.substr(0, startPos);
            } else {
                ip = host;
            }
            wxString pac = getPacScript(pacTpl, ip, CONFIGURATION.GetSocksPort(), CONFIGURATION.GetHttpPort());
            res.set_content(pac.ToStdString(), "text/plain");
        });
        wxLogMessage("PAC server start at %s:%d",
                     CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1",
                     CONFIGURATION.GetPacPort());
        bool ok = m_pacServer->listen(CONFIGURATION.GetShareOnLan() ? "0.0.0.0" : "127.0.0.1",
                                      CONFIGURATION.GetPacPort());
        if (!ok) {
            wxLogMessage("PAC server error listening!");
        }
    });
}

void Climber::StopPacServer(bool joinThread) {
    if (m_pacServer == nullptr) return;

    m_pacServer->stop();

    if (joinThread && m_pacServerThread->joinable()) {
        m_pacServerThread->join();
        delete m_pacServerThread;
    }

    delete m_pacServer;
    m_pacServer = nullptr;
    m_pacServerThread = nullptr;

    wxLogMessage("PAC server stopped!");
}
