#ifndef CALLBACK_H
#define CALLBACK_H
#include <map>

typedef  int (*callback)(int , int );

class map;

class TEST{

public:
    TEST();
    ~TEST();
    void setListener(callback listen);
    void run();

private:

    callback m_LastListener;
    std::map<int,callback> m_MCallback;
    int m_Index;


};

#endif