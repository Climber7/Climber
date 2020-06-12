//
// Created by Climber on 2020/6/9.
//

#ifndef CLIMBER_CLIMBER_H
#define CLIMBER_CLIMBER_H

#include <thread>
#include <httplib.h>
#include <wx/process.h>

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

    static bool RestartClient();

    static void KillClient();

    static void RestartPrivoxy();

    static void KillPrivoxy();

    void RestartPacServer();

    void KillPacServer(bool joinThread = true);

    static void ResetSystemProxy();

    static void ClearSystemProxy();

private:
    bool m_running = false;
    std::thread *m_pacServerThread = nullptr;
    httplib::Server *m_pacServer = nullptr;

};


#endif //CLIMBER_CLIMBER_H
