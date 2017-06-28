#pragma once
#include <stdhdr.h>
#include <CJob.h>
#include <CBall.h>

class CCltTodayJob : public CJob
{
public:
	CCltTodayJob();
	~CCltTodayJob();
	virtual void Run();

private:
	CBall	*m_pBallArray[6];
};
