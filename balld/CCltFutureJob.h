#pragma once
#include <stdhdr.h>
#include <CJob.h>
#include <CBall.h>

class CCltFutureJob : public CJob
{
public:
	CCltFutureJob();
	~CCltFutureJob();
	virtual void Run();

private:
	CBall	*m_pBallArray[6];
};
