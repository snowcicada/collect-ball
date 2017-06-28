#pragma once
#include <pthread.h>

class CMutex
{
public:
    CMutex();
    ~CMutex();

    bool    Lock();
    bool    TryLock();
    bool    Unlock();

private:
    pthread_mutex_t m_Mutex;
};
