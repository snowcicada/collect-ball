#include <CCltResultThread.h>
#include <CMysql.h>

CCltResultThread::CCltResultThread()
{

}

CCltResultThread::~CCltResultThread()
{

}

void CCltResultThread::Run()
{
	int ret = 0;

	printf("开始采集足球赛果4\n");
	m_pJob->Run();
	printf("足球赛果采集完成4\n");
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

//		m_pJob->Run();
//		break;

		//todo
		if (m_CltTimeout.IsTimeout())
		{
			//todo
			printf("开始采集足球赛果4\n");
			m_pJob->Run();
			printf("足球赛果采集完成4\n");
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

