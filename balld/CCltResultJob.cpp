#include <CCltResultJob.h>
#include <CMatchResult.h>

CCltResultJob::CCltResultJob()
{
	m_pMatchResult = new CMatchResult();
}

CCltResultJob::~CCltResultJob()
{
	if (m_pMatchResult)
	{
		delete m_pMatchResult;
	}
}

void CCltResultJob::Run()
{
	m_pMatchResult->Collect();
}

