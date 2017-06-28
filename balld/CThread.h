#pragma once
#include <stdhdr.h>
#include <CFlagMutex.h>
#include <CCondition.h>
#include <CJob.h>

#if 1
#define DEBUG_PRINTF
#endif

#define LOOP_QUIT		0 //退出
#define LOOP_SLEEP		1 //睡眠
#define LOOP_GO			2 //正常执行
#define LOOP_CONTINUE	3 //continue

class CThread
{
public:

    CThread();
    virtual ~CThread();

	//创建线程
	bool		Create();
	//启动线程
    void        Start();
    //暂停
    void        Pause();
    //退出
    void		Quit();
    //线程是否结束
    bool		IsFinished();
    //线程是否睡眠,提供给远程控制客户端调用
    bool		IsSleep();
    //判断退出与休眠标志
	int         IsLoop();
	//判断是否准备结束或睡眠
	bool		IsQuitOrSleep();
	//设置任务
	void		SetJob(CJob *job);
	CJob		*GetJob();
	//设置采集周期
	void		SetCltPeriod(time_t sec);
	time_t		GetCltPeriod();

    pthread_t   GetThreadId() const {return m_ThreadId;}
    int         GetLastErrorCode() const {return m_LastErrorCode;}
    string      GetLastErrorStr() const {return m_LastErrorStr;}
    void        HandleError(int err);

protected:
    virtual void Run() = 0;
    static void *ThreadFunction(void *arg);
    //线程退出调用
    void		ExitThread(void *retval=NULL);

    CCondition	m_SleepCond;//睡眠等待
    CFlagMutex	m_SleepFlag;//睡眠标志
    CFlagMutex	m_FinishedFlag;//结束标志
    CJob		*m_pJob;
    CTimeout	m_CltTimeout;

private:
    pthread_t   m_ThreadId;
    int         m_LastErrorCode;
    string      m_LastErrorStr;
};
