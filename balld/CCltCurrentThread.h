#pragma once
#include <CThread.h>

class CCltCurrentThread : public CThread
{
public:
	CCltCurrentThread();
	~CCltCurrentThread();

protected:
    virtual void Run();
};

