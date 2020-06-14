//
// Created by Climber on 2020/6/4.
//

#include <nlohmann/json.hpp>
#include "Configuration.h"
#include "ClientManager.h"
#include "Paths.h"
#include "utils.h"

#ifdef CLIMBER_DARWIN
#define PROXY_BYPASS_KEY ("proxy_bypass_darwin")
#endif
#ifdef CLIMBER_WINDOWS
#define PROXY_BYPASS_KEY ("proxy_bypass_windows")
#endif

using nlohmann::json;

Configuration *Configuration::s_instance = nullptr;

const char *Configuration::s_supportedLanguages[] = {
        "en_US",
        "zh_CN",
        "zh_TW",
};

wxString Configuration::s_supportedLanguageNames[] = {
        wxT("English (US)"),
        wxT("中文 (简体)"),
        wxT("中文 (中國台灣)"),
};

void Configuration::Init() {
    if (s_instance == nullptr) {
        s_instance = new Configuration();
    }
}

void Configuration::Destroy() {
    if (s_instance != nullptr) {
        s_instance->Save();
        delete s_instance;
        s_instance = nullptr;
    }
}

Configuration &Configuration::GetInstance() {
    return *s_instance;
}

wxArrayString Configuration::GetSupportedLanguageNames() {
    wxArrayString arr;
    for (auto &l : s_supportedLanguageNames) {
        arr.Add(l);
    }
    return arr;
}

bool Configuration::GetEnable() const {
    return m_enable;
}

void Configuration::SetEnable(bool enable) {
    m_enable = enable;
    Save();
}

const wxString &Configuration::GetProxyMode() const {
    return m_proxyMode;
}

void Configuration::SetProxyMode(const wxString &mode) {
    if (mode == PROXY_MODE_DIRECT
        || mode == PROXY_MODE_PAC
        || mode == PROXY_MODE_GLOBAL) {
        m_proxyMode = wxString(mode);
    }
    Save();
}

int Configuration::GetSelectedServerIndex() const {
    return m_selectedServer;
}

void Configuration::SetSelectedServerIndex(int index) {
    if (index < 0 || index >= CLIENT_MANAGER.Count()) {
        m_selectedServer = -1;
    } else {
        m_selectedServer = index;
    }
    Save();
}

const wxString &Configuration::GetLanguage() const {
    return m_language;
}

int Configuration::GetLanguageCode() const {
    return LanguageNameToCode(m_language);
}

int Configuration::GetLanguageIndex() const {
    int index = 0;
    for (const auto *l : s_supportedLanguages) {
        if (m_language == l) return index;
        ++index;
    }
    return 0;
}

void Configuration::SetLanguage(const wxString &language) {
    if (!IsLanguageSupported(language)) {
        wxLogWarning("Language %s not supported\n", language);
        return;
    }
    m_language = language;
    Save();
}

void Configuration::SetLanguageCode(int code) {
    wxString language = wxLocale::GetLanguageCanonicalName(code);
    SetLanguage(language);
}

void Configuration::SetLanguageIndex(int index) {
    wxString language = s_supportedLanguages[index];
    SetLanguage(language);
}

bool Configuration::GetShareOnLan() const {
    return m_shareOnLan;
}

void Configuration::SetShareOnLan(bool shareOnLan) {
    m_shareOnLan = shareOnLan;
    Save();
}

int Configuration::GetSocksPort() const {
    return m_socksPort;
}

void Configuration::SetSocksPort(int port) {
    m_socksPort = port;
    Save();
}

int Configuration::GetHttpPort() const {
    return m_httpPort;
}

void Configuration::SetHttpPort(int port) {
    m_httpPort = port;
    Save();
}

int Configuration::GetPacPort() const {
    return m_pacPort;
}

void Configuration::SetPacPort(int port) {
    m_pacPort = port;
    Save();
}

const wxString &Configuration::GetProxyBypass() {
    return m_proxyBypass;
}

void Configuration::SetProxyBypass(const wxString &bypass) {
    m_proxyBypass = bypass;
    Save();
}

Configuration::Configuration() {
    m_configurationFile = Paths::GetConfigDirFile("config.json");
    wxLogMessage("Configuration File: %s\n", m_configurationFile);

    InitDefaults();
    Load();
    InitLanguageSupport();
}

void Configuration::InitDefaults() {
    int languageCode = wxLocale::GetSystemLanguage();
    wxString language = wxLocale::GetLanguageCanonicalName(languageCode).ToStdString();
    if (IsLanguageSupported(language)) {
        m_language = language;
    }
}

void Configuration::Load() {
    std::string jsonStr = readTextFile(m_configurationFile).ToStdString();
    if (jsonStr.empty()) {
        return;
    }

    auto obj = json::parse(jsonStr);

    if (obj.find("enable") != obj.end()) {
        m_enable = obj["enable"];
    }

    if (obj.find("proxy_mode") != obj.end()) {
        std::string mode = obj["proxy_mode"];
        if (mode == PROXY_MODE_DIRECT
            || mode == PROXY_MODE_PAC
            || mode == PROXY_MODE_GLOBAL) {
            m_proxyMode = wxString(mode);
        }
    }

    if (obj.find("selected_server") != obj.end()) {
        int index = obj["selected_server"];
        if (index < 0 || index >= CLIENT_MANAGER.Count()) {
            m_selectedServer = -1;
        } else {
            m_selectedServer = index;
        }
    }

    if (obj.find("language") != obj.end()) {
        std::string language = obj["language"];
        if (IsLanguageSupported(language)) {
            m_language = wxString(language);
        }
    }

    if (obj.find("share_on_lan") != obj.end()) {
        m_shareOnLan = obj["share_on_lan"];
    }

    if (obj.find("socks_port") != obj.end()) {
        m_socksPort = obj["socks_port"];
    }

    if (obj.find("http_port") != obj.end()) {
        m_httpPort = obj["http_port"];
    }

    if (obj.find("pac_port") != obj.end()) {
        m_pacPort = obj["pac_port"];
    }

    if (obj.find(PROXY_BYPASS_KEY) != obj.end()) {
        std::string bypass = obj[PROXY_BYPASS_KEY];
        m_proxyBypass = wxString(bypass);
    }

}

void Configuration::Save() {
    json obj;
    obj["enable"] = m_enable;
    obj["proxy_mode"] = m_proxyMode;
    obj["selected_server"] = m_selectedServer;
    obj["language"] = m_language;
    obj["share_on_lan"] = m_shareOnLan;
    obj["socks_port"] = m_socksPort;
    obj["http_port"] = m_httpPort;
    obj["pac_port"] = m_pacPort;
    obj[PROXY_BYPASS_KEY] = m_proxyBypass;
    writeTextFile(m_configurationFile, wxString(obj.dump(4)));
}

void Configuration::InitLanguageSupport() {
#ifdef CLIMBER_WINDOWS
    wxLocale::AddCatalogLookupPathPrefix(Paths::GetLocaleDir());
#endif
    wxLocale *locale;
    int language = GetLanguageCode();
    if (wxLocale::IsAvailable(language)) {
        locale = new wxLocale(language);
        locale->AddCatalog("Climber");
        wxLogMessage("Load locale data %s\n", locale->IsOk() ? "ok" : "failed");
    } else {
        wxLogMessage("Language %d not available\n", language);
    }
}

bool Configuration::IsLanguageSupported(const wxString &language) {
    for (const auto *l : s_supportedLanguages) {
        if (language == l) return true;
    }
    return false;
}

int Configuration::LanguageNameToCode(const wxString &language) {
    for (int code = wxLANGUAGE_UNKNOWN + 1; code < wxLANGUAGE_USER_DEFINED; ++code) {
        if (language == wxLocale::GetLanguageCanonicalName(code)) {
            return code;
        }
    }
    return wxLANGUAGE_UNKNOWN;
}


