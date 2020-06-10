//
// Created by Climber on 2020/6/4.
//

#ifndef CLIMBER_CONFIGURATION_H
#define CLIMBER_CONFIGURATION_H

#include <wx/wx.h>
#include "defs.h"

#define CONFIGURATION  (Configuration::GetInstance())


class Configuration {
public:
    static void Init();

    static void Destroy();

    static Configuration &GetInstance();

    static wxArrayString GetSupportedLanguageNames();

public:
    bool GetEnable() const;

    void SetEnable(bool enable);

    const wxString &GetProxyMode() const;

    void SetProxyMode(const wxString &mode);

    int GetSelectedServerIndex() const;

    void SetSelectedServerIndex(int index);

    const wxString &GetLanguage() const;

    int GetLanguageCode() const;

    int GetLanguageIndex() const;

    void SetLanguage(const wxString &language);

    void SetLanguageCode(int code);

    void SetLanguageIndex(int index);

    bool GetAutoStart() const;

    void SetAutoStart(bool autoStart);

    bool GetShareOnLan() const;

    void SetShareOnLan(bool shareOnLan);

    int GetSocksPort() const;

    void SetSocksPort(int port);

    int GetHttpPort() const;

    void SetHttpPort(int port);

    int GetPacPort() const;

    void SetPacPort(int port);

    // TODO
    const wxString &GetProxyBypass() { return m_proxyBypass; }

    bool PortAlreadyInUse(int port) const;

private:
    Configuration();

    void InitDefaults();

    void Load();

    void Save();

    void InitLanguageSupport();

private:
    static bool IsLanguageSupported(const wxString &language);

    static int LanguageNameToCode(const wxString &language);

private:
    wxString m_configurationFile;

private:
    bool m_enable = false;
    wxString m_proxyMode = DEFAULT_PROXY_MODE;
    int m_selectedServer = -1;
    wxString m_language = DEFAULT_LANGUAGE;
    bool m_autoStart = DEFAULT_AUTO_START;
    bool m_shareOnLan = DEFAULT_SHARE_ON_LAN;
    int m_socksPort = DEFAULT_SOCKS_PORT;
    int m_httpPort = DEFAULT_HTTP_PORT;
    int m_pacPort = DEFAULT_PAC_PORT;
    wxString m_proxyBypass = wxEmptyString;

private:
    static Configuration *s_instance;
    static const char *s_supportedLanguages[];
    static wxString s_supportedLanguageNames[];

};


#endif //CLIMBER_CONFIGURATION_H
