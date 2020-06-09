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

    static Configuration &GetInstance();

    static wxArrayString GetSupportedLanguageNames();

public:
    int GetProxyMode() const;

    void SetProxyMode(int mode);

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

    bool PortAlreadyInUse(int port) const;

private:
    Configuration();

    void InitPath();

    void InitDefaults();

    void Load();

    void Save();

private:
    static bool IsLanguageSupported(const wxString &language);

    static int LanguageNameToCode(const wxString &language);

private:
    wxString m_configurationFile;

private:
    int m_proxyMode = DEFAULT_PROXY_MODE;
    wxString m_language = DEFAULT_LANGUAGE;
    bool m_autoStart = DEFAULT_AUTO_START;
    bool m_shareOnLan = DEFAULT_SHARE_ON_LAN;
    int m_socksPort = DEFAULT_SOCKS_PORT;
    int m_httpPort = DEFAULT_HTTP_PORT;
    int m_pacPort = DEFAULT_PAC_PORT;

private:
    static Configuration *s_instance;
    static const char *s_supportedLanguages[];
    static wxString s_supportedLanguageNames[];

};


#endif //CLIMBER_CONFIGURATION_H
