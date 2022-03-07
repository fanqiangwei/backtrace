#ifndef   DIAGNOSE_H
#define DIAGNOSE_H
#include <mutex>
#include <iostream>

using namespace std;
class Diagnose
{

public:
    static Diagnose * GetInstance();
    void Initialize();
    void UnInitialize();
private:
    Diagnose();
    ~Diagnose();
    Diagnose(const Diagnose& dig)=delete;
    Diagnose &operator=(const Diagnose& dig)=delete;

    static void handler(int sig);
    void printfSignal(int sig);
    void log(string str);
    string getProgramName();
    class Garbo 
    {
    public:
        ~Garbo();
    };
private:
    static Diagnose * Instance;
    static Garbo garbo;
    static mutex s_Mutex;
    string m_exeName;

};






#endif
