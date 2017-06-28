#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <CThread.h>

CThread::CThread()
{
	m_ThreadId	= 0;
	m_pJob		= NULL;
	m_CltTimeout.SetTimeout(3600);
}

CThread::~CThread()
{
	if (m_pJob)
	{
		delete m_pJob;
		m_pJob = NULL;
	}
}

//创建线程
//线程创建成功后，默认处于睡眠状态
bool CThread::Create()
{
	int err;
	pthread_attr_t attr;
	size_t stacksize;

	//睡眠设置
	m_FinishedFlag.SetFlag(false);
	m_SleepFlag.SetFlag(true);

	pthread_attr_init(&attr);

#if 0
	//线程结束后,自动释放堆栈等等资源
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//分离
#else
	//必须使用pthread_join函数释放资源,否则资源和线程id不能得到释放
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//非分离
#endif

	//设置线程取消动作执行时机
//	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	stacksize = 1 * 1024 * 1024;//Bytes 1M
	pthread_attr_setstacksize(&attr, stacksize);

	err = pthread_create(&m_ThreadId, &attr, ThreadFunction, this);

	pthread_attr_destroy(&attr);

	HandleError(err);

	return (0 == err);
}

//启动线程
void CThread::Start()
{
	//下一次循环有效
	m_SleepFlag.SetFlag(false);
	m_SleepCond.Signal();
}

//暂停
void CThread::Pause()
{
	//下一次循环有效
	m_SleepFlag.SetFlag(true);
}

//退出
void CThread::Quit()
{
	//设置退出标志
	m_FinishedFlag.SetFlag(true);
	Start();
	pthread_join(m_ThreadId, NULL);
}

//线程是否结束
bool CThread::IsFinished()
{
	return m_FinishedFlag.GetFlag();
}

//线程是否睡眠,提供给远程控制客户端调用
bool CThread::IsSleep()
{
	return m_SleepFlag.GetFlag();
}

//判断退出与休眠标志
int CThread::IsLoop()
{
    if (m_FinishedFlag.GetFlag())
    {
    	return LOOP_QUIT;
    }

    if (m_SleepFlag.GetFlag())
    {
        m_SleepCond.Wait();
        return LOOP_SLEEP;
    }

    return LOOP_GO;
}

//判断是否准备结束或睡眠
bool CThread::IsQuitOrSleep()
{
	return (m_FinishedFlag.GetFlag() || m_SleepFlag.GetFlag());
}

//设置任务
void CThread::SetJob(CJob *job)
{
	m_pJob = job;
}

CJob *CThread::GetJob()
{
	return m_pJob;
}

//设置采集周期
void CThread::SetCltPeriod(time_t sec)
{
	m_CltTimeout.SetTimeout(sec);
}

time_t CThread::GetCltPeriod()
{
	return m_CltTimeout.GetTimeout();
}

void CThread::HandleError(int err)
{
	if (0 == err) return;

	m_LastErrorCode = errno;
	m_LastErrorStr.assign(strerror(m_LastErrorCode));

	#ifdef DEBUG_PRINTF
	printf("%s\n", m_LastErrorStr.c_str());
	#endif
}

void *CThread::ThreadFunction(void *arg)
{
	CThread *self = static_cast<CThread *>(arg);
	self->Run();

	return NULL;
}

//线程退出调用
void CThread::ExitThread(void *retval)
{
	m_FinishedFlag.SetFlag(true);
	pthread_exit(retval);
}

/*
子类Run使用:

void CThread::Run()
{
	while (!m_FinishedFlag.GetFlag())
	{
		if (m_SleepFlag.GetFlag())
		{
			//睡眠
			m_SleepCond.Wait();
			//线程唤醒后，判断一下退出标志
			if (m_FinishedFlag.GetFlag())
			{
				break;
			}
		}

		//线程处理
	}
}

*/
