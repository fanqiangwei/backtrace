#ifndef   DIAGNOSE_H
#define DIAGNOSE_H
#include <mutex>

using namespace std;
class Diagnose
{

public:
    static Diagnose * GetInstance();
    void initialize();

private:
    Diagnose(){}
    Diagnose(const Diagnose& dig)=delete;
    Diagnose &operator=(const Diagnose& dig)=delete;

    static void handler(int sig);
    void printfSignal(int sig);
    void log(string str);
private:
    static Diagnose * Instance;
    static mutex s_Mutex;


};






#endif