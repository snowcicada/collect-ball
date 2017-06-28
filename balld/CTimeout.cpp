#include <CTimeout.h>

CTimeout::CTimeout()
{
    m_LastTime	= 0;
    m_Timeout	= 0;
}

void CTimeout::StartTimer()
{
	m_LastTime = time(NULL);
}

bool CTimeout::IsTimeout()
{
	if (-1 == m_Timeout)
	{
		return false;
	}
    return (time(NULL) - m_LastTime > m_Timeout);
}

void CTimeout::SetTimeout(time_t sec)
{
    m_Timeout = sec;
}

time_t CTimeout::GetTimeout()
{
	return m_Timeout;
}

void CTimeout::UpdateLastTime()
{
	StartTimer();
}
