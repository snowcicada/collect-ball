#include <CCltCurrentJob.h>
#include <CNowFootBall.h>
#include <CNowBasketBall.h>
#include <CNowTennisBall.h>
#include <CNowVolleyBall.h>
#include <CNowBaseBall.h>
#include <CNowOpBall.h>

CCltCurrentJob::CCltCurrentJob()
{
	for (int i = 0; i < 6; i++)
	{
		m_pBallArray[i] = NULL;
	}
	m_pBallArray[0] = new CNowFootBall();
//	m_pBallArray[1] = new CNowBasketBall();
//	m_pBallArray[2] = new CNowTennisBall();
//	m_pBallArray[3] = new CNowVolleyBall();
//	m_pBallArray[4] = new CNowBaseBall();
//	m_pBallArray[5] = new CNowOpBall();
}

CCltCurrentJob::~CCltCurrentJob()
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

void CCltCurrentJob::Run()
{
	m_pBallArray[0]->Collect();
}

