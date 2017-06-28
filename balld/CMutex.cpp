#include <CMutex.h>

CMutex::CMutex()
{
	pthread_mutex_init(&m_Mutex, NULL);
}

CMutex::~CMutex()
{
	pthread_mutex_destroy(&m_Mutex);
}

bool CMutex::Lock()
{
	return (0 == pthread_mutex_lock(&m_Mutex));
}

bool CMutex::TryLock()
{
	return (0 == pthread_mutex_trylock(&m_Mutex));
}

bool CMutex::Unlock()
{
	return (0 == pthread_mutex_unlock(&m_Mutex));
}
