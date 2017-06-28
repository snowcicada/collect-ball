#include <CCltFutureThread.h>
#include <CMysql.h>

CCltFutureThread::CCltFutureThread()
{

}

CCltFutureThread::~CCltFutureThread()
{

}

void CCltFutureThread::Run()
{
	int ret = 0;

	printf("开始采集足球早盘2\n");
	m_pJob->Run();
	printf("足球早盘采集完成2\n");
	m_CltTimeout.StartTimer();
	while (1)
	{
		ret = IsLoop();
		if (LOOP_QUIT == ret)
		{
			break;
		}
		else if (LOOP_SLEEP == ret)
		{
			continue;
		}

//		printf("Future Thread start\n");
//		m_pJob->Run();
//		printf("Future Thread end\n");
//		break;

		//todo
		if (m_CltTimeout.IsTimeout())
		{
			//todo
			printf("开始采集足球早盘2\n");
			m_pJob->Run();
			printf("足球早盘采集完成2\n");
			m_CltTimeout.StartTimer();
		}
		else
		{
			sleep(1);
		}
	}
	CMysql::ThreadEnd();
	ExitThread();
}

