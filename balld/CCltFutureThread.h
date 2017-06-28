#pragma once
#include <CThread.h>

class CCltFutureThread : public CThread
{
public:
	CCltFutureThread();
	~CCltFutureThread();

protected:
    virtual void Run();
};

