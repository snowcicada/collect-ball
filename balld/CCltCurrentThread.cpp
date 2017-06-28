#include <CCltCurrentThread.h>
#include <CMysql.h>

CCltCurrentThread::CCltCurrentThread()
{

}

CCltCurrentThread::~CCltCurrentThread()
{

}

void CCltCurrentThread::Run()
{
	int ret = 0;

	printf("开始采集足球滚球3\n");
	m_pJob->Run();
	printf("足球滚球采集完成3\n");
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

//		printf("Current Thread start\n");
//		m_pJob->Run();
//		printf("Current Thread end\n");
//		break;

		//todo
		if (m_CltTimeout.IsTimeout())
		{
			//todo
			printf("开始采集足球滚球3\n");
			m_pJob->Run();
			printf("足球滚球采集完成3\n");
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

