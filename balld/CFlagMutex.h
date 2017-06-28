#pragma once
#include <iostream>
#include <CMutex.h>

using namespace std;

class CFlagMutex
{
public:
    CFlagMutex();
    bool    GetFlag();
    void    SetFlag(bool flag);

private:
    CMutex  m_Mutex;
    bool    m_bFlag;
};
