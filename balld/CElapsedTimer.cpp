#include <CElapsedTimer.h>

CElapsedTimer::CElapsedTimer()
{
    m_iStartTime = 0;
    m_bStart = false;
}

CElapsedTimer::~CElapsedTimer()
{

}

time_t CElapsedTimer::ElapsedSecs()
{
    return m_bStart ? time(NULL)-m_iStartTime : 0;
}

void CElapsedTimer::Start()
{
    m_iStartTime = time(NULL);
    m_bStart = true;
}
