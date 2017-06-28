#pragma once
#include <stdhdr.h>
#include <CMutex.h>

class CSettings
{
private:
    CSettings();
    ~CSettings();

public:
    static CSettings   *Instance();
    static void         Destroy();

	//设置守护进程标志
    void			SetRunDaemon(bool ok);
    bool			GetRunDaemon();
    //添加用户名信息
    void			AddUserMap(const StringMap &upMap);
    UserInfo		*GetUserInfo();
    //添加网址
    void			AddUrlVec(const StringVector &vec);
    string          GetUrl();
    //设置数据库信息
    void			SetSqlInfo(const SqlInfo &si);
    void			GetSqlInfo(SqlInfo &si);
    //参数是否设置完成
    bool            IsValidParam();
//    //用户信息锁
//    void			LockUser();
//    void			UnlockUser();

private:
    bool                m_bRunDaemon;
    SqlInfo				m_SqlInfo;
    UserInfoMap         m_UserInfoMap;//用户信息
    StringVector		m_strUrlVec;//网址列表
    UserInfo m_UserInfo;

	CMutex				m_ValueMutex;//锁
//	CMutex				m_UserMutex;//用户信息锁
	static CMutex		m_SingleMutex;//单例锁
    static CSettings	*m_pInstance;

    //以下作为自动释放单例对象的方法
    //m_Garbo必须在源文件初始化
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (CSettings::m_pInstance)
            {
                delete CSettings::m_pInstance;
            }
        }
    };
    static CGarbo m_Garbo;
};
