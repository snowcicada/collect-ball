#pragma once
#include <time.h>

class CTimeout
{
public:
    CTimeout();
    //重新设置启动时间
    void	StartTimer();
    //判断是否超时
    bool	IsTimeout();
    //设置超时时间
    void	SetTimeout(time_t sec);
    time_t	GetTimeout();
    //更新最后访问启动时间
    void	UpdateLastTime();

private:
    time_t m_LastTime;
    time_t m_Timeout;
};
