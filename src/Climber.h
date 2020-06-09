//
// Created by Climber on 2020/6/9.
//

#ifndef CLIMBER_CLIMBER_H
#define CLIMBER_CLIMBER_H

#include <wx/process.h>
#include "ServerConfItem.h"

#define CLIMBER (Climber::GetInstance())

class Climber {
public:
    static void Init();

    static void Destroy();

    static Climber &GetInstance();

private:
    Climber();

    static Climber *s_instance;

public:
    bool IsRunning() const;

    void Start();

    void Stop();

    void Restart();

    void SetSystemProxy();

    void ClearSystemProxy();

private:
    void StopClient();

    void StartPrivoxy();

    void StopPrivoxy();

    long GetPrivoxyPid();

    void RunTrojan(ServerConfItem *conf);

private:
    bool m_running = false;
    wxString m_clientTmpConfigFile = wxEmptyString;
    wxString m_clientLogFile = wxEmptyString;
    long m_clientPid = 0;

    wxString m_privoxyTmpConfigFile = wxEmptyString;
    wxString m_privoxyLogFile = wxEmptyString;
    wxString m_privoxyPidFile = wxEmptyString;

};


#endif //CLIMBER_CLIMBER_H
