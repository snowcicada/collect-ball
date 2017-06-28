#include <CCollectThread.h>
#include <CCurl.h>
#include <CPublic.h>
#include <CRegex.h>
#include <CSettings.h>
#include <CJson.h>
#include <CTimeout.h>
#include <CElapsedTimer.h>
#include <ball.h>

CCollectThread::CCollectThread()
{
	m_iCltTimeType	= TIME_NONE;
	m_iCltPeriod	= -1;

//	int len = sizeof(g_pTodayArray)/(3*sizeof(int));
//	cout << len << endl;
//	for (int i = 0; i < len; i++)
//	{
//		printf("%s %s %s\n",
//				g_pTimeArrZh[g_pTodayArray[i][2]], g_pBallArrZh[g_pTodayArray[i][0]], g_pBetArrZh[g_pTodayArray[i][1]]);
//	}
//
//	len = sizeof(g_pFutureArray)/(3*sizeof(int));
//	cout << len << endl;
//	for (int i = 0; i < len; i++)
//	{
//		printf("%s %s %s\n",
//				g_pTimeArrZh[g_pFutureArray[i][2]], g_pBallArrZh[g_pFutureArray[i][0]], g_pBetArrZh[g_pFutureArray[i][1]]);
//	}
//
//	len = sizeof(g_pCurrentArray)/(3*sizeof(int));
//	cout << len << endl;
//	for (int i = 0; i < len; i++)
//	{
//		printf("%s %s %s\n",
//				g_pTimeArrZh[g_pCurrentArray[i][2]], g_pBallArrZh[g_pCurrentArray[i][0]], g_pBetArrZh[g_pCurrentArray[i][1]]);
//	}
}

CCollectThread::~CCollectThread()
{

}

//滚球 今日赛事 早盘
void CCollectThread::SetCltTimeType(TimeType type)
{
	m_iCltTimeType = type;
}

//设置采集周期
void CCollectThread::SetCltPeriod(time_t sec)
{
	m_iCltPeriod = sec;
}

void CCollectThread::Run()
{
    int ret;
    CTimeout to;

	//没有设置参数，直接退出线程
	if (TIME_NONE == m_iCltTimeType || -1 == m_iCltPeriod)
	{
		goto thr_exit;
	}

	//设置采集周期
	to.SetTimeout(m_iCltPeriod);
	//启动首次采集
//	RunCore();

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

		//定时采集数据
		//临时
		RunCore();
//		if (to.IsTimeout())
//		{
//			RunCore();
//			to.StartTimer();
//		}
//		else
//		{
//			sleep(1);
//		}
		break;
	}

thr_exit:
	ExitThread();
}

//线程核心处理
void CCollectThread::RunCore()
{
	UserInfo *pui;
    string strUrl;
    CSettings *pSet = CSettings::Instance();

	//获取用户信息
	pui = pSet->GetUserInfo();
	if (!pui)
	{
		return;
	}

	//获取网址,并判断是否在维护
	strUrl = pSet->GetUrl();
//	cout << "strurl=" << strUrl << endl;
	if (!DealSysMaintain(strUrl))
	{
		return;
	}

	//登陆网站
	if (!DealLogin(pui, strUrl))
	{
		return;
	}

	//登陆成功,开始采集数据
	CollectAllBallData(pui);

	return;
}

//处理系统维护状态,
bool CCollectThread::DealSysMaintain(const string &url)
{
	CTimeout to;

	to.SetTimeout(60);
	while (IsMaintain(url))
	{
		//计时:1分钟判断一次网页是否处于维护状态
		to.StartTimer();
		while (1)
		{
			if (IsQuitOrSleep())
			{
				return false;
			}

			if (to.IsTimeout())
			{
				break;
			}
			else
			{
				sleep(1);
			}
		}
	}
	return true;
}

//处理登陆
bool CCollectThread::DealLogin(UserInfo *pui, const string &url)
{
	//判断是否需要登陆
	if (pui->reLogin)
	{
		int ret;
		string strHtml;

		//login
		if (!Login(url, pui->user, pui->passwd, strHtml))
		{
			return false;
		}
		ret = GetHtmlError(strHtml);
		if (ERR_UPWRONG == ret || ERR_USERBAN == ret || ERR_NOTFOUND || ERR_UNKNOWN == ret)
		{
			pui->userState = ret;
			return false;
		}

		//解析登陆成功后的信息
		if (!ParseHtml(strHtml, pui))
		{
			return false;
		}
	}
	return true;
}

//是否在维护
bool CCollectThread::IsMaintain(const string &url)
{
    CCurl curl;
    string strHtml;

    if (!curl.Init())
    {
        return false;
    }

    if (!curl.Get(url, strHtml))
    {
        return true;
    }

    if (CPublic::ContainSubstr(strHtml, "Maintenance"))
    {
        return true;
    }

    return false;
}

//登陆到网站
//url的格式:http://www.hg3088.com
bool CCollectThread::Login(const string &url, const string &user, const string &passwd, string &html)
{
	CCurl curl;
	string strUrl, strField;
	StringMap strMap;

	if (!curl.Init())
	{
		return false;
	}

	strUrl = url + "/app/member/login.php";

	//生成表单域
	strMap["uid"] 		= "";
	strMap["langx"] 	= "zh-cn";
	strMap["mac"] 		= "";
	strMap["ver"] 		= "";
	strMap["JE"] 		= "false";
	strMap["username"] 	= user;
	strMap["passwd"] 	= passwd;
	strMap["radio"] 	= "web_new";
	LinkFieldMap(strMap, strField);

	//登陆
	if (!curl.Post(strUrl, strField, html, NULL))
	{
		return false;
	}
	CPublic::WriteToFile("/opt/login_yes.html", html);

	return true;
}

//解析登陆成功后的页面,得到相应的网址和uid
bool CCollectThread::ParseHtml(const string &html, UserInfo *pui)
{
    string strUrl;
    StringMap strMap;

    if (!CRegex::GetUrl(html, strUrl) || !CRegex::GetHiddenKeyValue(html, strMap))
    {
        return false;
    }

    for (StringMap::iterator it = strMap.begin(); it != strMap.end(); ++it)
	{
		cout << it->first << " " << it->second << endl;
	}

    pui->url        = strUrl;
    pui->uid        = strMap["uid"];
    pui->langx      = strMap["langx"];
    pui->mtype      = strMap["mtype"];
    pui->today_gmt  = strMap["today_gmt"];
    pui->chghasLogin= strMap["chghasLogin"];

	return true;
}

//根据各种类型生成特定的网址
string CCollectThread::CreateUrl(const string &baseUrl, const string &uid, const string &lang,
								const string &mtype, BallType ballType, BetType betType,
								TimeType timeType, int page)
{
	char urlBuf[512];
	BetType bType;

	//获取投注类型
	bType = betType;
	if (BALL_FT == ballType || BALL_OP == ballType)
	{
		if (BET_PR == betType)
		{
			bType = BET_P3;
		}
	}

	//冠军
	if (BET_FS == betType)
	{
		//http://w782.hg3088.com/app/member/browse_FS/reloadgame_R.php?mid=10220238&uid=28aa067cm10220238l44916264&langx=zh-cn&choice=ALL&LegGame=ALL&pages=1&records=40&FStype=FT&area_id=&league_id=&rtype=fs&hot_game=
		sprintf(urlBuf, "%s/app/member/browse_FS/reloadgame_R.php?mid=1&uid=%s&langx=%s&choice=ALL&LegGame=ALL&pages=1&"
				"records=40&FStype=%s&area_id=&league_id=&rtype=fs&hot_game=",
				baseUrl.c_str(), uid.c_str(), lang.c_str(), g_pBallTypeArray[ballType]);
		return string(urlBuf);
	}
	else
	{
		//其他球 其他玩法
		//http://w151.hg3088.com/app/member/FT_future/body_var.php?uid=1c2d9b27m10236972l44412697&rtype=r&langx=zh-cn&g_date=&mtype=4&page_no=0&league_id=&hot_game=
		sprintf(urlBuf, "%s/app/member/%s_%s/body_var.php?uid=%s&rtype=%s&langx=%s&mtype=%s&page_no=%d&league_id=&hot_game=",
				baseUrl.c_str(), g_pBallTypeArray[ballType], g_pTimeTypeArray[timeType], uid.c_str(),
				g_pBetTypeArray[bType], lang.c_str(), mtype.c_str(), page);
	}

	return string(urlBuf);
}

//采集数据
//从0页开始循环采集，直到匹配不到数据为止
bool CCollectThread::CollectAllBallData(UserInfo *pui)
{
//	StringList strList;
//	string strJsonData;
//	int sz;
//	int (*pArr)[3];
//
//	switch (m_iCltTimeType)
//	{
//	case TIME_TODAY:
//		sz = sizeof(g_pTodayArray)/(3*sizeof(int));
//		pArr = g_pTodayArray;
//		break;
//	case TIME_CURRENT:
//		sz = sizeof(g_pCurrentArray)/(3*sizeof(int));
//		pArr = g_pCurrentArray;
//		break;
//	case TIME_FUTURE:
//		sz = sizeof(g_pFutureArray)/(3*sizeof(int));
//		pArr = g_pFutureArray;
//		break;
//	case TIME_NONE:
//	default:
//		return false;
//		break;
//	}
//
//	for (int i =0; i < sz; i++)
//	{
//		printf("Thread(%lu):%s %s %s\n",
//				GetThreadId(), g_pTimeArrZh[pArr[i][2]], g_pBallArrZh[pArr[i][0]], g_pBetArrZh[pArr[i][1]]);
////		cout << "Thread" << "CLT:" << g_pBallTypeArray[pArr[i][0]] << " " << g_pBetTypeArray[pArr[i][1]] << endl;
////		CollectBallData(pui, (BallType)pArr[i][0], (BetType)pArr[i][1], (TimeType)pArr[i][2], strList);
//		//采集完数据，匹配Array字段与Array中的值，生成map
//		//然后根据不同球型，不同玩法，匹配出需要的数据，并写入对应的数据库中
////		CJson::CreateJsonArray(strList, strJsonData);
////		WriteToSql((BallType)ballArr[i][0], (BetType)ballArr[i][1], (TimeType)ballArr[i][2], strJsonData);
////		WriteToSql((BallType)pArr[i][0], (BetType)pArr[i][1], (TimeType)pArr[i][2], strList);
//	}

	return true;
}

//循环采集每种类型的数据
bool CCollectThread::CollectBallData(UserInfo *pui, BallType ballType, BetType betType, TimeType timeType, StringList &lst)
{
	string strUrl;
	StringList strList;
	int page;

	lst.clear();
	if (BET_FS == betType)
	{
		//冠军，只采集一个页面
		strUrl = CreateUrl(pui->url, pui->uid, pui->langx, pui->mtype, ballType, betType, timeType, 0);
//		cout << strUrl << endl;
		CollectData(strUrl, strList, &page);
		//合并到lst总链表,效率比赋值高
		lst.splice(lst.end(), strList);
	}
	else
	{
		strUrl = CreateUrl(pui->url, pui->uid, pui->langx, pui->mtype, ballType, betType, timeType, 0);
		CollectData(strUrl, strList, &page);//获取页数
//		cout << strUrl << endl;

		for (int i = 0; i < page; i++)
		{
			strUrl = CreateUrl(pui->url, pui->uid, pui->langx, pui->mtype, ballType, betType, timeType, i);
			//有一次采集失败，就跳出循环
			if (!CollectData(strUrl, strList))
			{
				break;
			}
			//合并到lst总链表
			lst.splice(lst.end(), strList);
//			cout << strUrl << endl;
		}
	}
//	cout << endl;
	return true;
}

//从指定网址采集数据
bool CCollectThread::CollectData(const string &url, StringList &lst, int *page)
{
	CCurl curl;
	string strHtml, strErr;

	//初始化
	if (!curl.Init())
	{
		return false;
	}

	//下载页面
	if (!curl.Get(url, strHtml, NULL))
	{
		return false;
	}

	//判断错误
	if (GetHtmlError(strHtml) != ERR_OK)
	{
		cout << "+++++clt error" << endl;
		return false;
	}

//	char buf[512];
//	static int ct = 1;
//	sprintf(buf, "/var/log/balld/%d.txt", ct++);
//	CPublic::WriteToFile(buf, strHtml);

	//匹配页数
	if (page)
	{
		if (!CRegex::GetPage(strHtml, *page))
		{
			return false;
		}
	}

	//匹配数据
    if (!CRegex::GetArray(strHtml, lst))
    {
        return false;
    }

    //删除<font...</font>内容
    string strTmp;
    for (StringList::iterator it = lst.begin(); it != lst.end(); ++it)
    {
        RemoveSub(*it);
    }

	return true;
}

//写入数据库
bool CCollectThread::WriteToSql(BallType ballType, BetType betType, TimeType timeType, const string &data)
{
//	cout << g_pBallTypeArray[ballType] << ":" << endl << data << endl;
	char buf[512];
	sprintf(buf, "/var/log/balld/%s_%s_%s.txt", g_pBallTypeArray[ballType], g_pBetTypeArray[betType], g_pTimeTypeArray[timeType]);
	CPublic::WriteToFile(buf, data);
	return true;
}

bool CCollectThread::WriteToSql(BallType ballType, BetType betType, TimeType timeType, const StringList &data)
{
	char buf[512];
	fstream fs;
	string strTmp;

	sprintf(buf, "/var/log/balld/%s_%s_%s.txt", g_pBallTypeArray[ballType], g_pBetTypeArray[betType], g_pTimeTypeArray[timeType]);

	fs.open(buf, ios::out|ios::trunc);
	if (fs.bad())
	{
		return false;
	}
	for (StringList::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		strTmp = *it + '\n';
		if (fs.write(strTmp.c_str(), strTmp.size()).bad())
		{
			break;
		}
	}
	fs.close();
	return true;
}

//连接表单域
void CCollectThread::LinkFieldMap(StringMap &kvMap, string &str)
{
    str.clear();
    for (StringMap::iterator it = kvMap.begin(); it != kvMap.end(); ++it)
    {
        str += CCurl::UrlEncode(it->first) + '=' +CCurl::UrlEncode(it->second) + '&';
    }
    if (str.begin() != str.end())
    {
        str.erase(str.end() - 1);
    }
}

//获取网页的错误信息
int CCollectThread::GetHtmlError(const string &html)
{
	int ret;
	string strErr;

	ret = ERR_UNKNOWN;
	//判断是否登陆成功
	if (CPublic::ContainSubstr(html, "uid") || CPublic::ContainSubstr(html, "Array"))
	{
		//登陆成功 采集成功
		ret = ERR_OK;
	}
	else if (CPublic::ContainSubstr(html, "404 Not Found"))
	{
		//网站正在维护
		ret = ERR_NOTFOUND;
	}
	else if (CPublic::ContainSubstr(html, "logout_warn"))
	{
		//您已被强制登出
		ret = ERR_LOGOUT;
	}
	else if (CPublic::ContainSubstr(html, "alert"))
	{
		//有错误信息,匹配出来
		if (CRegex::GetError(html, strErr))
		{
			if (CPublic::ContainSubstr(strErr, "帐号或密码不正确"))
			{
				ret = ERR_UPWRONG;
			}
			else if (CPublic::ContainSubstr(strErr, "帐号已被停用"))
			{
				ret = ERR_USERBAN;
			}
		}
	}
	else
	{
		//目前还未遇到的错误信息,写入文件
		string strFileTime = "/var/log/balld/" + CPublic::GetCurrentFileTime() + ".html";
		CPublic::WriteToFile(strFileTime, html);
	}

	if (!strErr.empty())
	{
		cout << "strErr=" << strErr << endl;
	}

	return ret;
}

//删除字符串中网页的元素:<br> <font xx>xxx</font>
//暂时只删除<font...</font>
void CCollectThread::RemoveSub(string &str)
{
	string::size_type idx, idxe;

//	while (1)
//	{
//		idx = str.find("<br>", 0);
//		if (string::npos == idx)
//		{
//			break;
//		}
//		str.erase(idx, 4);
//	}

	while (1)
	{
		idx = str.find("<font", 0);
		if (string::npos == idx)
		{
			break;
		}
		idxe = str.find("</font>", idx);
		idxe += 7;

		str.erase(idx, idxe-idx+1);
	}
}

