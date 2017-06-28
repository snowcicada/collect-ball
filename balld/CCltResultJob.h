#pragma once
#include <stdhdr.h>
#include <CJob.h>
#include <CBall.h>

class CCltResultJob : public CJob
{
public:
	CCltResultJob();
	~CCltResultJob();
	virtual void Run();

private:
	CBall *m_pMatchResult;
};
