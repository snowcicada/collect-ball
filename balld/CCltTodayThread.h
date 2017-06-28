#pragma once
#include <CThread.h>

class CCltTodayThread : public CThread
{
public:
	CCltTodayThread();
	~CCltTodayThread();

protected:
    virtual void Run();
};

