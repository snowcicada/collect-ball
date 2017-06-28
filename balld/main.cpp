#include <stdhdr.h>
#include <CPublic.h>
#include <CJson.h>
#include <CCurl.h>
#include <CTcpServer.h>
#include <CTcpSocket.h>
#include <CSettings.h>
#include <CRegex.h>
#include <signal.h>
#include <CMysql.h>
#include <CCltTodayThread.h>
#include <CCltFutureThread.h>
#include <CCltCurrentThread.h>
#include <CCltResultThread.h>

#include <CCltTodayJob.h>
#include <CCltFutureJob.h>
#include <CCltCurrentJob.h>
#include <CCltResultJob.h>
//#include <CBall.h>
//#include <CCltTodayJob.h>
//#include <CMatchResult.h>

#if 0
#define LOCKFILE	"/var/run/balld.pid"
#define LOCKMODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

static const char *g_cmd = "balld";

static void Usage()
{
	printf("Usage : %s [-d] [-h]\n", g_cmd);
	exit(1);
}

static bool IsRunning()
{
//    CTcpSocket tcpSock;
//    string strRecv;
//
//    if (!tcpSock.ConnectToHost("127.0.0.1", 26120))
//    {
//        return false;
//    }
//    if (!tcpSock.Send(MSG_RUN))
//    {
//        return false;
//    }
//    if (!tcpSock.Recv(strRecv))
//    {
//        return false;
//    }
//    if (MSG_RUN != strRecv)
//    {
//        return false;
//    }
//    tcpSock.Close();
    return true;
}

static void Parse(int argc, char **argv, CSettings *pSet)
{
	int opt;

	struct option longOpts[] =
	{
		{"daemon", 	required_argument, 	NULL, 'd'},
		{"help", 	no_argument, 		NULL, 'h'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "dh", longOpts, NULL)) != -1)
	{
		switch (opt)
		{
		case 'd':
			pSet->SetRunDaemon(true);
			break;
		case 'h':
			Usage();
			break;
		default: /* '?' */
		    Usage();
			break;
		}
	}
}

//fork 2 times
static void SetDaemon()
{
    pid_t pid;
    if ((pid = fork()) != 0)
    {
        exit(0);//parent
    }
    //child
    setsid();
    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) != 0)
    {
        exit(0);//child
    }
    //grandson
    chdir("/");
    umask(0);

    //重定向0 1 2, 不能关闭, 否则cout会造成程序崩溃
    int fd = open("/dev/null", O_RDWR);
	if (fd < 0)
	{
		printf("open \"/dev/null\" failed.\n");
		exit(1);
	}

	if (dup2(fd, STDIN_FILENO) < 0)
	{
		printf("dup2(STDIN) failed.\n");
		exit(1);
	}

	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		printf("dup2(STDOUT) failed.\n");
		exit(1);
	}

	if (dup2(fd, STDERR_FILENO) < 0)
	{
		printf("dup2(STDERR) failed.\n");
		exit(1);
	}

    //关闭无用到fd，这些fd可能并未关闭
    for (int i = 3; i < 64; i++)
    {
        close(i);
    }
}

#endif

int main(int argc, char **argv)
{
//	CSettings *pSet = CSettings::Instance();//global setting
//	Parse(argc, argv, pSet);

    //single application
//    if (IsRunning())
//    {
//        printf("%s is already run.\n", g_cmd);
//        exit(0);
//    }

//	if (pSet->GetRunDaemon())
//    {
//        SetDaemon();
//    }

    //开启守护进程日志
//    openlog("balld", LOG_PID, LOG_LOCAL3);
//    syslog(LOG_INFO, "balld started.");

	//init curl
	if (!CCurl::GlobalInit())
	{
		return -1;
	}

	//初始化数据库
	if (!CMysql::GlobalInit())
	{
		return -1;
	}

	//TODO
	//数据库
	CSettings *pSet = CSettings::Instance();
	SqlInfo sqlInfo;
	sqlInfo.ipAddr = "192.168.1.217";
	sqlInfo.user = "root";
	sqlInfo.passwd = "haoying123";
	sqlInfo.dbname = "sport";
	sqlInfo.port = 3306;
	pSet->SetSqlInfo(sqlInfo);


//	CCltTodayThread todayThread;
//	CCltFutureThread futureThread;
//	CCltCurrentThread currentThread;
//	CCltResultThread resultThread;

//	//今日赛事
//	todayThread.SetCltPeriod(180);
//	todayThread.SetJob(new CCltTodayJob());
//	todayThread.Create();
//	todayThread.Start();
//
//	//滚球
//	currentThread.SetCltPeriod(60);
//	currentThread.SetJob(new CCltCurrentJob());
//	currentThread.Create();
//	currentThread.Start();
//
//	//早盘
//	currentThread.SetCltPeriod(180);
//	futureThread.SetJob(new CCltFutureJob());
//	futureThread.Create();
//	futureThread.Start();

	//赛果
//	resultThread.SetCltPeriod(300);
//	resultThread.SetJob(new CCltResultJob());
//	resultThread.Create();
//	resultThread.Start();

	getchar();
//	todayThread.Quit();
//	futureThread.Quit();
//	currentThread.Quit();
//	resultThread.Quit();

	//curl下载
//	string html;
//	CCurl curl;
//	curl.Init();
////	curl.Get("http://w151.hg3088.com/app/member/result/result_sfs.php?game_type=FT&today=2013-09-06&uid=1c8121abm10266692l44919859&langx=zh-cn", html);
//	curl.Get("www.ntdtv.com", html);
//	CPublic::WriteToFile("/opt/fq.html", html);

    //命令通道loop
//    cout << "start loop" << endl;
//    CTcpServer tcpSrv;
//    if (!tcpSrv.Listen(SOCKET_PORT))
//	{
//		return -1;
//	}
//	tcpSrv.EpollLoop();
//	cout << "end loop" << endl;
//
	CMysql::GlobalEnd();
	CCurl::GlobalCleanup();
//
//	syslog(LOG_INFO, "balld finished.");
//
//    //关闭守护进程日志
//    closelog();

	return 0;
}
