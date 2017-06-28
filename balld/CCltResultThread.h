#pragma once
#include <CThread.h>

class CCltResultThread : public CThread
{
public:
	CCltResultThread();
	~CCltResultThread();

protected:
    virtual void Run();
};

