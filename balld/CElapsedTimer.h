#pragma once
#include <stdhdr.h>

class CElapsedTimer
{
public:
    CElapsedTimer();
    ~CElapsedTimer();
    time_t  ElapsedSecs();
    void    Start();

private:
    time_t  m_iStartTime;
    bool    m_bStart;
};
