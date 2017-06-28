#include <CCondition.h>

CCondition::CCondition()
{
	pthread_mutex_init(&m_ThreadMutex, NULL);
	pthread_cond_init(&m_ThreadCond, NULL);
}

CCondition::~CCondition()
{
	pthread_cond_destroy(&m_ThreadCond);
	pthread_mutex_destroy(&m_ThreadMutex);
}

bool CCondition::Wait()
{
	int ret;

	pthread_mutex_lock(&m_ThreadMutex);//lock

	ret = pthread_cond_wait(&m_ThreadCond, &m_ThreadMutex);

	pthread_mutex_unlock(&m_ThreadMutex);//unlock
	return (0 == ret);
}

bool CCondition::TimedWait(time_t sec)
{
	int ret;
	struct timeval now;
	struct timespec tsp;

	pthread_mutex_lock(&m_ThreadMutex);//lock

	gettimeofday(&now, NULL);
	tsp.tv_sec = now.tv_sec + sec;
	tsp.tv_nsec = now.tv_usec * 1000;

	ret = pthread_cond_timedwait(&m_ThreadCond, &m_ThreadMutex, &tsp);

	pthread_mutex_unlock(&m_ThreadMutex);//unlock

	return (0 == ret);
}

bool CCondition::Signal()
{
	return (0 == pthread_cond_signal(&m_ThreadCond));
}

bool CCondition::Broadcast()
{
	return (0 == pthread_cond_broadcast(&m_ThreadCond));
}
