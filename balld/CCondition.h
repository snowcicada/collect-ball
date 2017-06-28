#pragma once
#include <iostream>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

class CCondition
{
public:
    CCondition();
    ~CCondition();

    bool    Wait();
    bool    TimedWait(time_t sec);
    bool    Signal();
    bool    Broadcast();

private:
    pthread_mutex_t m_ThreadMutex;
    pthread_cond_t  m_ThreadCond;
};
