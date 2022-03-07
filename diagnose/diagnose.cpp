#include "diagnose.h"
#include <unistd.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdlib>

#define MAXFILESIZE 10*1024*1024   
#define COREFILENAME "corefile.txt"
Diagnose * Diagnose::Instance = NULL;
Diagnose::Garbo Diagnose::garbo;
mutex Diagnose::s_Mutex;
Diagnose* Diagnose::GetInstance()
{
    if(Instance == NULL)
    {
        lock_guard<mutex> lock(s_Mutex);
        if(Instance == NULL)
        {
            Instance = new Diagnose();
        }  
    }
    return Instance;
}

void Diagnose::Initialize()
{
    signal(SIGSEGV, handler); 	   /* Segmentation violation (ANSI).  */
    signal(SIGBUS, handler);      /* BUS error (4.2 BSD).  */
    signal(SIGFPE, handler);      /* Floating-point exception (ANSI).  */
    signal(SIGPIPE, handler);     /* Broken pipe (POSIX).  */
    signal(SIGSTKFLT, handler);   /* Stack fault.  */
}

void Diagnose::UnInitialize()
{
    signal(SIGSEGV, SIG_DFL); 	   /* Segmentation violation (ANSI).  */
    signal(SIGBUS, SIG_DFL);      /* BUS error (4.2 BSD).  */
    signal(SIGFPE, SIG_DFL);      /* Floating-point exception (ANSI).  */
    signal(SIGPIPE, SIG_DFL);     /* Broken pipe (POSIX).  */
    signal(SIGSTKFLT, SIG_DFL);   /* Stack fault.  */
}

Diagnose::Diagnose():m_exeName(""){
    log("SYSTEM START..."); 
}

Diagnose::~Diagnose()
{

}

void Diagnose::handler(int sig)
{
    fprintf(stderr, "ERROR: signal %d:\n", sig);
    GetInstance()->printfSignal(sig);
    void *array[10];
    size_t size;
    char **func_name_cache;

    size = backtrace (array, 10);
    //backtrace_symbols_fd(array, size, STDERR_FILENO);
    func_name_cache = backtrace_symbols (array, size);

    for (size_t i = 0; i < size; i++)
    {
        string tmpStr = func_name_cache[i];
        string diamandStr = "";
        int pos = tmpStr.find("[0x");
        diamandStr = "addr2line -a -C -p -f " + tmpStr.substr(pos+1, 8) + " -e " + GetInstance()->getProgramName() + " | tee -a " + COREFILENAME;
        cout << tmpStr << endl;
        GetInstance()->log(tmpStr);
        system(diamandStr.c_str());
    }

    free (func_name_cache);
    //GetInstance()->UnInitialize();
    exit(1);
}

void Diagnose::printfSignal(int sig)
{
    string str = "";
    switch (sig)
    {
    case 7:
        str = "ERROR exit for bus:";
        break;
    case 8:
        str = "ERROR backtrace for caculate:";
        break;    
    case 11:
        str = "ERROR backtrace for memory:";
        break;
    case 13:
        str = "ERROR backtrace for pipe:";
        break;
    case 16:
        str = "ERROR backtrace for stack fault:";              
        break;
    default:
        str = "ERROR: signal " + to_string(sig) + ":";
        break;
    }
    log(str); 
}

void Diagnose::log(string str)
{
    ofstream out(COREFILENAME, ios::app);
    if (out.is_open()) 
    {
        size_t dstFileSize = out.tellp();
        if(dstFileSize > MAXFILESIZE)
        {
            out.close();
            out.open(COREFILENAME);
            if(!out.is_open())
            {
                cout<< "open corefile.txt failed" << endl;
                return;
            }
        }

        time_t tmp;   
        struct tm *timp; 
        time(&tmp);   
        timp = localtime(&tmp);
        str = "{" +to_string(1900 + timp->tm_year) + "-" + to_string(1 + timp->tm_mon)+ "-" + to_string(timp->tm_mday) 
                        + " " + to_string(timp->tm_hour) + ":" + to_string(timp->tm_min) + ":" + to_string(timp->tm_sec) + "} " + str;
        out << str << "\n";
        out.close();
    }else{
        cout<< "open corefile.txt failed" << endl;
    }
}

string Diagnose::getProgramName()
{
    if(m_exeName != "")
    {
        return m_exeName;
    }

    std::string _exeName = "/proc/self/exe";

    size_t linksize = 256;

    char   exeName[256] = {0};

    if(readlink(_exeName.c_str() , exeName, linksize) !=-1 )
    {
        m_exeName = exeName;
    }
    return m_exeName;
}


Diagnose::Garbo::~Garbo()
{
    if(Diagnose::Instance != NULL)
    {
        Diagnose::Instance->log("SYSTEM END...\n"); 
        delete Diagnose::Instance;
        Diagnose::Instance = NULL;
    }
}