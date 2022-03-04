#ifndef   DIAGNOSE_H
#define DIAGNOSE_H
#include <mutex>
#include <iostream>

using namespace std;
class Diagnose
{

public:
    static Diagnose * GetInstance();
    void initialize();
private:
    Diagnose();
    ~Diagnose();
    Diagnose(const Diagnose& dig)=delete;
    Diagnose &operator=(const Diagnose& dig)=delete;

    static void handler(int sig);
    void printfSignal(int sig);
    void log(string str);
    class Garbo 
    {
        public:
            ~Garbo()
            {
                if(Diagnose::Instance != NULL)
                {
                    Diagnose::Instance->log("SYSTEM END...\n"); 
                    delete Diagnose::Instance;
                    Diagnose::Instance = NULL;
                }
            }
    };
private:
    static Diagnose * Instance;
    static Garbo garbo;
    static mutex s_Mutex;


};






#endif
