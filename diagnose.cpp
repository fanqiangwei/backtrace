#include "diagnose.h"
#include <unistd.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

Diagnose * Diagnose::Instance = NULL;
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

void Diagnose::initialize()
{
    signal(SIGSEGV, handler); 	   /* Segmentation violation (ANSI).  */
    signal(SIGBUS, handler);      /* BUS error (4.2 BSD).  */
    signal(SIGFPE, handler);      /* Floating-point exception (ANSI).  */
    signal(SIGPIPE, handler);     /* Broken pipe (POSIX).  */
    signal(SIGSTKFLT, handler);   /* Stack fault.  */
}

void Diagnose::handler(int sig)
{
    fprintf(stderr, "Error: signal %d:\n", sig);
    GetInstance()->printfSignal(sig);
    void *array[10];
    size_t size;
    char **func_name_cache;

    size = backtrace (array, 10);
    //backtrace_symbols_fd(array, size, STDERR_FILENO);
    func_name_cache = backtrace_symbols (array, size);

    for (size_t i = 0; i < size; i++)
    {
        cout << func_name_cache[i] << endl;
        GetInstance()->log(func_name_cache[i]);
    }

    free (func_name_cache);
    exit(1);
}

void Diagnose::printfSignal(int sig)
{
    string str = "Error: signal " + to_string(sig) + ":\n";
    switch (sig)
    {
    case 7:
    case 8:
    case 11:
    case 13:
    case 16:
        log(str);        
        break;
    
    default:
        break;
    }
}

void Diagnose::log(string str)
{
    ofstream out("corefile.txt", ios::app);
    if (out.is_open()) 
    {
        out << str << "\n";
        out.close();
    }
}
