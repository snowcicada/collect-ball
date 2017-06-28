#include <CSettings.h>
#include <CPublic.h>
#include <CTcpSocket.h>

CSettings		   *CSettings::m_pInstance = NULL;
CMutex				CSettings::m_SingleMutex;
CSettings::CGarbo	CSettings::m_Garbo;

CSettings::CSettings()
{
    //初始值
    m_bRunDaemon = false;

    m_UserInfo.user = "linux12";
    m_UserInfo.passwd = "q1234567q";
    m_UserInfo.url = "http://w151.hg3088.com";
	m_UserInfo.uid = "38f20039m10266692l47526851";
	m_UserInfo.langx = "zh-cn";
	m_UserInfo.mtype = "4";
	m_UserInfo.today_gmt = "2013-09-24";
}

CSettings::~CSettings()
{
}

CSettings *CSettings::Instance()
{
	m_SingleMutex.Lock();
    if (!m_pInstance)
    {
        m_pInstance = new CSettings();
    }
    m_SingleMutex.Unlock();
    return m_pInstance;
}

void CSettings::Destroy()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void CSettings::SetRunDaemon(bool ok)
{
    m_ValueMutex.Lock();
    m_bRunDaemon = ok;
    m_ValueMutex.Unlock();
}

bool CSettings::GetRunDaemon()
{
    m_ValueMutex.Lock();
    bool ok = m_bRunDaemon;
    m_ValueMutex.Unlock();
    return ok;
}

//添加用户名信息
void CSettings::AddUserMap(const StringMap &upMap)
{
    UserInfo *pui;
    UserInfoMap::iterator fit;

	m_ValueMutex.Lock();
	for (StringMap::const_iterator it = upMap.begin(); it != upMap.end(); ++it)
	{
	    fit = m_UserInfoMap.find(it->first);
	    if (m_UserInfoMap.end() != fit)
        {
            //find
            pui = fit->second;
            if (pui->passwd != it->second)
            {
                //密码改变
                pui->passwd     = it->second;
                pui->reLogin    = true;//要重新登陆
                pui->userState	= ERR_OK;
            }
        }
        else
        {
            //not find
            pui = new UserInfo;
            pui->user       = it->first;
            pui->passwd     = it->second;
            pui->reLogin    = true;//要登陆
            pui->userState	= ERR_OK;
            m_UserInfoMap[it->first] = pui;

            cout << "user: " << it->first << "  " << it->second << endl;
        }
	}
	m_ValueMutex.Unlock();
}

//需要优化，暂时使用第一个没有被禁用的用户
UserInfo *CSettings::GetUserInfo()
{
//    UserInfo *pui = NULL;
//    m_ValueMutex.Lock();
//    for (UserInfoMap::iterator it = m_UserInfoMap.begin(); it != m_UserInfoMap.end(); ++it)
//    {
//        pui = it->second;
//        if (ERR_OK != pui->userState)
//        {
//            break;
//        }
//    }
//    m_ValueMutex.Unlock();
//    return pui;
	return &m_UserInfo;
}

//添加网址
void CSettings::AddUrlVec(const StringVector &vec)
{
	m_ValueMutex.Lock();
	for (StringVector::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if (find(m_strUrlVec.begin(), m_strUrlVec.end(), *it) == m_strUrlVec.end())
		{
			//not find
			m_strUrlVec.push_back(*it);
			cout << "url: " << *it << endl;
		}
	}
	m_ValueMutex.Unlock();
}

//遍历使用所有url
string CSettings::GetUrl()
{
    static int ct = 0;
    string url;
    m_ValueMutex.Lock();
    url = m_strUrlVec[++ct % m_strUrlVec.size()];
    m_ValueMutex.Unlock();
    return url;
}

//设置数据库信息
void CSettings::SetSqlInfo(const SqlInfo &si)
{
	m_ValueMutex.Lock();
	m_SqlInfo = si;
//	m_SqlInfo.ipAddr	= si.ipAddr;
//	m_SqlInfo.user		= si.user;
//	m_SqlInfo.passwd	= si.passwd;
//	m_SqlInfo.dbname	= si.dbname;
//	m_SqlInfo.port		= si.port;
	m_ValueMutex.Unlock();
}

void CSettings::GetSqlInfo(SqlInfo &si)
{
	m_ValueMutex.Lock();
	si = m_SqlInfo;
	m_ValueMutex.Unlock();
}

//参数是否设置完成
bool CSettings::IsValidParam()
{
    bool ok = false;
    m_ValueMutex.Lock();
//    ok = (!m_UserInfoMap.empty() && !m_strUrlVec.empty() && !m_strSqlAddr.empty()
//          && !m_strSqlUser.empty() && !m_strSqlPasswd.empty());
	//暂时更改
//	    ok = (!m_UserInfoMap.empty() && !m_strUrlVec.empty());
    m_ValueMutex.Unlock();
    return ok;
}

////用户信息锁
//void CSettings::LockUser()
//{
//	m_UserMutex.Lock();
//}
//
//void CSettings::UnlockUser()
//{
//	m_UserMutex.Unlock();
//}


