
#include "diagnose.h"
#include <unistd.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include "diagnose.h"
using namespace std;



void fun3()
{
    int *foo = (int*)-1;
    printf("%d\n", *foo);
}

void fun2()
{
    fun3();
}
void fun1()
{
    fun2();
}


int main(int argc, char **argv)
{
    Diagnose::GetInstance()->Initialize();
    int i = 0;
    //float j = 7 / i;
    fun1();
}
/*

#include <iostream>
#include <memory>
#include "callback.h"

int function(int a, int b){
    return a + b; 
}

int main(int argv, char *argc[])
{
    std::shared_ptr<TEST> test = std::make_shared<TEST>();
    test->setListener(function);
    test->run();
}*/