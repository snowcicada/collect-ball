#include <CFlagMutex.h>

CFlagMutex::CFlagMutex()
{
	m_bFlag = true;
}

bool CFlagMutex::GetFlag()
{
	bool flag;
	m_Mutex.Lock();
	flag = m_bFlag;
	m_Mutex.Unlock();
	return flag;
}

void CFlagMutex::SetFlag(bool flag)
{
	m_Mutex.Lock();
	m_bFlag = flag;
	m_Mutex.Unlock();
}

