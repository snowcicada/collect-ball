#include <CCltTodayThread.h>
#include <CMysql.h>

CCltTodayThread::CCltTodayThread()
{

}

CCltTodayThread::~CCltTodayThread()
{

}

void CCltTodayThread::Run()
{
	int ret = 0;

	printf("开始采集足球今日赛事1\n");
	m_pJob->Run();
	printf("足球今日赛事采集完成1\n");
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

//		printf("Today Thread start\n");
//		m_pJob->Run();
//		printf("Today Thread end\n");
//		break;

		//todo
		if (m_CltTimeout.IsTimeout())
		{
			//todo
			printf("开始采集足球今日赛事1\n");
			m_pJob->Run();
			printf("足球今日赛事采集完成1\n");
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

