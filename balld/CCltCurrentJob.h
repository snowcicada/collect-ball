#pragma once
#include <stdhdr.h>
#include <CJob.h>
#include <CBall.h>

class CCltCurrentJob : public CJob
{
public:
	CCltCurrentJob();
	~CCltCurrentJob();
	virtual void Run();

private:
	CBall	*m_pBallArray[6];
};
