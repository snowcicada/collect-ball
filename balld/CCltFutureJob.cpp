#include <CCltFutureJob.h>
#include <CFootBall.h>
#include <CBasketBall.h>
#include <CTennisBall.h>
#include <CVolleyBall.h>
#include <CBaseBall.h>
#include <COpBall.h>

CCltFutureJob::CCltFutureJob()
{
	for (int i = 0; i < 6; i++)
	{
		m_pBallArray[i] = NULL;
	}
	m_pBallArray[0] = new CFootBall(TIME_FUTURE);
//	m_pBallArray[1] = new CBasketBall(TIME_FUTURE);
//	m_pBallArray[2] = new CTennisBall(TIME_FUTURE);
//	m_pBallArray[3] = new CVolleyBall(TIME_FUTURE);
//	m_pBallArray[4] = new CBaseBall(TIME_FUTURE);
//	m_pBallArray[5] = new COpBall(TIME_FUTURE);
}

CCltFutureJob::~CCltFutureJob()
{
	for (int i = 0; i < 6; i++)
	{
		if (m_pBallArray[i])
		{
			delete m_pBallArray[i];
			m_pBallArray[i] = NULL;
		}
	}
}

void CCltFutureJob::Run()
{
	m_pBallArray[0]->Collect();
}

