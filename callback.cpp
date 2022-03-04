#include "callback.h"
#include <iostream>

TEST::TEST(){
    m_Index = 0;
}

TEST::~TEST(){
    m_Index = 0;
}

void TEST::setListener(callback listen){
    m_LastListener = listen;
    m_Index++;
    m_MCallback[m_Index] = listen;
}

void TEST::run(){
    for(auto iter = m_MCallback.begin(); iter != m_MCallback.end(); iter++){
        std::cout << iter->second(5, 6) << std::endl;
    }
}