#include <CBall.h>
#include <ball.h>
#include <CCurl.h>
#include <CPublic.h>
#include <CRegex.h>
#include <CSettings.h>

CBall::CBall()
{
	m_iTimeType = TIME_NONE;
}

CBall::CBall(uint type)
{
	m_iTimeType = type;
}

CBall::~CBall()
{

}

//采集
bool CBall::Collect()
{
	return true;
}

//设置模式:滚球 今日 早盘
void CBall::SetTimeType(uint type)
{
	m_iTimeType = type;
}

//生成网址
string CBall::CreateUrl(const string &baseUrl, const string &uid, const string &mtype, const string &datetime,
						const string &gid, BallType ballType, BetType betType, int page)
{
	char urlBuf[1024];
	const char *pUrl		= baseUrl.c_str();
	const char *pUid		= uid.c_str();
	const char *pMtype		= mtype.c_str();
	const char *pDateTime	= datetime.c_str();
	const char *pBallType	= g_pBallTypeArray[ballType];
	const char *pBetType	= g_pBetTypeArray[betType];
	const char *pTimeType	= g_pTimeTypeArray[m_iTimeType];
	const char *pLang		= UID_LANG;
	const char *pGid		= gid.c_str();

	urlBuf[0] = 0;
	//更多,当存在更多的情况下要采集更多链接
	//http://w151.hg3088.com/app/member/result/result_sp.php?uid=7f8c8647m10266692l46415105&gtype=FT&gid=1468308&langx=zh-cn
	//不考虑日期

	if (betType >= BET_R && betType <= BET_P3)
	{
		//玩法:BET_R BET_PD BET_T BET_F BET_ALL BET_PR BET_P3
		//http://w151.hg3088.com/app/member/FT_future/body_var.php?uid=1c2d9b27m10236972l44412697&rtype=r&langx=zh-cn&g_date=&mtype=4&page_no=0&league_id=&hot_game=
		sprintf(urlBuf, "%s/app/member/%s_%s/body_var.php?uid=%s&rtype=%s&langx=%s&mtype=%s&page_no=%d&league_id=&hot_game=",
				pUrl, pBallType, pTimeType, pUid, pBetType, pLang, pMtype, page);
	}
	else if (betType >= BET_RES && betType <= BET_SFSRES)
	{
		//赛果:BET_RES BET_FSRES BET_SFSRES
		//http://w151.hg3088.com/app/member/result/result.php?game_type=FT&today=2013-09-03&uid=7d951b6m10266692l45089836&langx=zh-cn
		//http://w151.hg3088.com/app/member/result/result_fs.php?game_type=FT&today=2013-09-03&uid=1c8121abm10266692l44919859&langx=zh-cn
		//http://w151.hg3088.com/app/member/result/result_sfs.php?game_type=FT&today=2013-09-07&uid=1c8121abm10266692l44919859&langx=zh-cn
		sprintf(urlBuf, "%s/app/member/result/%s.php?game_type=%s&today=%s&uid=%s&langx=%s",
				pUrl, pBetType, pBallType, pDateTime, pUid, pLang);
	}
	else if (betType >= BET_RE && betType <= BET_RF)
	{
		//滚球:BET_RE BET_RPD BET_RT BET_RF
		//http://w151.hg3088.com/app/member/FT_browse/body_var.php?uid=1c2d9b27m10236972l44412697&rtype=re&langx=zh-cn&mtype=4&page_no=0&delay=&league_id=&hot_game=
		sprintf(urlBuf, "%s/app/member/%s_%s/body_var.php?uid=%s&rtype=%s&langx=%s&mtype=%s&page_no=%d&delay=&league_id=&hot_game=",
				pUrl, pBallType, pTimeType, pUid, pBetType, pLang, pMtype, page);
	}
	else if (BET_RESSP == betType)
	{
		//http://w151.hg3088.com/app/member/result/result_sp.php?uid=7f8c8647m10266692l46415105&gtype=FT&gid=1468308&langx=zh-cn
		sprintf(urlBuf, "%s/app/member/result/%s.php?uid=%s&gtype=%s&gid=%s&langx=%s",
				pUrl, pBetType, pUid, pBallType, pGid, pLang);
	}
	else if (BET_FS == betType)
	{
		//冠军:BET_FS
		//http://w782.hg3088.com/app/member/browse_FS/reloadgame_R.php?mid=10220238&uid=28aa067cm10220238l44916264&langx=zh-cn&choice=ALL&LegGame=ALL&pages=1&records=40&FStype=FT&area_id=&league_id=&rtype=fs&hot_game=
		sprintf(urlBuf, "%s/app/member/browse_FS/reloadgame_R.php?mid=1&uid=%s&langx=%s&choice=ALL&LegGame=ALL&pages=1&"
				"records=40&FStype=%s&area_id=&league_id=&rtype=fs&hot_game=",
				pUrl, pUid, pLang, pBallType);
	}

	return string(urlBuf);
}

//下载页面
bool CBall::DownloadPage(const string &url, string &html)
{
	CCurl curl;
	int ret;

	//初始化
	if (!curl.Init())
	{
		return false;
	}

	//下载页面
	if (!curl.Get(url, html))
	{
		return false;
	}

	//判断错误
	ret = GetHtmlError(html);
	if (ERR_OK != ret)
	{
		printf("GetHtmlError:%d\n", ret);
		return false;
	}

	return true;
}

//下载所有页面
bool CBall::DownloadBallAllPage(BallType ballType, BetType betType, BallData &ballData)
{
	int page = 0;
	string strUid, strBaseUrl, strMtype, strUrl, strHtml;
	StringList strPageList;

	FreeBallData(ballData);

	//获取已登陆的uid
	if (!GetLoginUid(strUid, strBaseUrl, strMtype))
	{
		return false;
	}

	//下载一个类型的所有页面
	strUrl = CreateUrl(strBaseUrl, strUid, strMtype, "", "", ballType, betType);
	printf("下载页面_%s_%s\n", g_pBallTypeArray[ballType], g_pBetTypeArray[betType]);
	if (!DownloadPage(strUrl, strHtml))
	{
		return false;
	}
	CRegex::GetPage(strHtml, page);
	strPageList.push_back(strHtml);//添加到链表

	//遍历所有页面
	if (page >= 2)
	{
		for (int i = 1; i < page; i++)
		{
			strUrl = CreateUrl(strBaseUrl, strUid, strMtype, "", "", ballType, betType, i);
			printf("下载页面_%s_%s\n", g_pBallTypeArray[ballType], g_pBetTypeArray[betType]);
			DownloadPage(strUrl, strHtml);
			strPageList.push_back(strHtml);//添加到链表
		}
	}

	//匹配所有页面的数据
	ParseArray(strPageList, ballData);

	return true;
}

//下载冠军
bool CBall::DownloadBallChampion(BallType ballType, MatchChampionMap &mcMap)
{
	string strUid, strBaseUrl, strMtype, strUrl, strHtml;

	//获取已登陆的uid
	if (!GetLoginUid(strUid, strBaseUrl, strMtype))
	{
		return false;
	}

	//下载一个类型的所有页面
	strUrl = CreateUrl(strBaseUrl, strUid, strMtype, "", "", ballType, BET_FS);
	printf("下载页面_%s_%s\n", g_pBallTypeArray[ballType], g_pBetTypeArray[BET_FS]);
	if (!DownloadPage(strUrl, strHtml))
	{
		return false;
	}

	//匹配冠军数据
	ParseChampion(strHtml, mcMap);

	return true;
}

//下载赛果
bool CBall::DownloadBallResult(BallType ballType, BetType betType, const string &datetime, string &html)
{
	string strUid, strBaseUrl, strMtype, strUrl;

	//获取已登陆的uid
	if (!GetLoginUid(strUid, strBaseUrl, strMtype))
	{
		return false;
	}

	//下载普通赛果
	strUrl = CreateUrl(strBaseUrl, strUid, strMtype, datetime, "", ballType, betType);
	cout << strUrl << endl;
	printf("下载页面_%s_%s\n", g_pBallTypeArray[ballType], g_pBetTypeArray[betType]);
	if (!DownloadPage(strUrl, html))
	{
		return false;
	}

	return true;
}

//下载赛果更多页面
bool CBall::DownloadBallResultMore(BallType ballType, const string &gid, string &html)
{
	string strUid, strBaseUrl, strMtype, strUrl;

	//获取已登陆的uid
	if (!GetLoginUid(strUid, strBaseUrl, strMtype))
	{
		return false;
	}

	//下载普通赛果
	strUrl = CreateUrl(strBaseUrl, strUid, strMtype, "", gid, ballType, BET_RESSP);
	cout << strUrl << endl;
	printf("下载页面_%s_%s\n", g_pBallTypeArray[ballType], g_pBetTypeArray[BET_RESSP]);
	if (!DownloadPage(strUrl, html))
	{
		return false;
	}

	return true;
}

//匹配Array内容到BallData
void CBall::ParseArray(const StringList &htmlList, BallData &ballData)
{
	//匹配所有页面的数据
	StringList strList, strAllList;
	string strField;

	for (StringList::const_iterator it = htmlList.begin(); it != htmlList.end(); ++it)
	{
		CRegex::GetArray(*it, strList);
		if (strList.size() > 1)
		{
			//有数据
			strField = strList.front();//得到字段名称
			strList.pop_front();//删除第一个字段名称
			strAllList.splice(strAllList.end(), strList);
		}
	}

	//匹配结果存入BallData
	//匹配字段名称
	if (!CRegex::GetField(strField, ballData.FieldVec))
	{
		return;
	}

	//匹配数据
	StringVector *strDataVec;
	for (StringList::iterator it = strAllList.begin(); it != strAllList.end(); ++it)
	{
		strDataVec = new StringVector;
		CRegex::GetField(*it, *strDataVec);
		ballData.DataVecList.push_back(strDataVec);
	}
}

void CBall::ParseArray(const string &html, BallData &ballData)
{
	StringList strList;
	strList.push_back(html);
	ParseArray(strList, ballData);
}

//匹配冠军结果
void CBall::ParseChampion(const string &html, MatchChampionMap &mcMap)
{
	StringList strList;
	int resNum;
	StringVector strFieldVec;
	string strFirstTeam, strSecondTeam;
	MatchChampion *mc;
	MatchMode *mm;
	MatchRes *mr;
	mcMap.clear();
	MatchChampionMap::iterator mcmIt;

	//匹配页面的数据
	CRegex::GetArray(html, strList);

	//匹配结果存入MatchChampionMap
	for (StringList::iterator it = strList.begin(); it != strList.end(); ++it)
	{
		//('28877','2013-09-06 14:45','世界杯2014欧洲外围赛','德国vs奥地利-两队都进球','Y','2','N','FS01','否','1.85','N','FS02','是','1.85','FT');
		CRegex::GetField(*it, strFieldVec);
		if (strFieldVec.size() < 6) continue;
		//添加到mcMap
		mcmIt = mcMap.find(strFieldVec[2]);
		if (mcMap.end() != mcmIt)
		{
			//find
			mc = mcmIt->second;
		}
		else
		{
			//not find
			//创建赛事,添加到map
			mc = new MatchChampion;
			mcMap[strFieldVec[2]] = mc;
		}

		//创建新的玩法
		mm = new MatchMode;
		mm->Gid = strFieldVec[0];
		mm->ModeName = strFieldVec[3];
		mm->MatchDateTime = strFieldVec[1];
		mc->modeList.push_back(mm);

		//创建N个结果
		/*
		规则
		Y		4
		Num		5
		N		6	10	14	18
		FS01	7	11	15	19
		Name	8	12	16	20
		赔率		9	13	17	21

		计算公式
		6 + 4*n
		7 + 4*n
		8 + 4*n
		9 + 4*n
		*/
		//得到结果个数
		resNum = atoi(strFieldVec[5].c_str());
		if ((int)strFieldVec.size() < (9 + 4 * (resNum-1) + 1)) continue;
		for (int i = 0; i < resNum; i++)
		{
			//创建结果
			mr = new MatchRes;
			mr->Res = strFieldVec[8 + 4 * i];
			mr->Odds = strFieldVec[9 + 4 * i];

			//添加结果
			mm->ResMap[strFieldVec[7 + 4 * i]] = mr;
		}
	}

	//设置赛事时间
	for (MatchChampionMap::iterator it = mcMap.begin(); it != mcMap.end(); ++it)
	{
		mc = it->second;
		if (!mc->modeList.empty())
		{
			mm = mc->modeList.front();
			mc->MatchDateTime = mm->MatchDateTime;
		}
	}
}

//匹配赛果
void CBall::ParseResult(const string &html)
{
}

//获取登陆uid
bool CBall::GetLoginUid(string &uid, string &baseUrl, string &mtype)
{
	CSettings *pSet = CSettings::Instance();
	UserInfo *ui = pSet->GetUserInfo();

	uid = ui->uid;
	baseUrl = ui->url;
	mtype = ui->mtype;

	return true;
}

//设置uid无效
void CBall::SetUidInvalid(const string &uid)
{

}

//打印冠军结果
void CBall::PrintChampion(const MatchChampionMap &mcMap)
{
	MatchChampion *mc = NULL;
	MatchMode *mm = NULL;
	MatchRes *mr = NULL;
	for (MatchChampionMap::const_iterator it1 = mcMap.begin(); it1 != mcMap.end(); ++it1)
	{
		mc = it1->second;
		cout << it1->first << ":" << endl;
		for (MatchModeList::iterator it2 = mc->modeList.begin(); it2 != mc->modeList.end(); ++it2)
		{
			mm = *it2;
			cout << "\t" << mm->Gid << ":" << endl;
			cout << "\t\t" << mm->ModeName << endl;
			cout << "\t\t" << mm->MatchDateTime << endl;
			for (MatchResMap::iterator it3 = mm->ResMap.begin(); it3 != mm->ResMap.end(); ++it3)
			{
				mr = it3->second;
				cout << "\t\t结果:" << it3->first << "\t" << mr->Res << "\t" << mr->Odds << endl;
			}
		}
	}
}

//释放冠军结果的空间
void CBall::FreeChampion(MatchChampionMap &mcMap)
{
	MatchChampion *mc = NULL;
	MatchMode *mm = NULL;
	for (MatchChampionMap::iterator it1 = mcMap.begin(); it1 != mcMap.end(); ++it1)
	{
		mc = it1->second;
		for (MatchModeList::iterator it2 = mc->modeList.begin(); it2 != mc->modeList.end(); ++it2)
		{
			mm = *it2;
			for (MatchResMap::iterator it3 = mm->ResMap.begin(); it3 != mm->ResMap.end(); ++it3)
			{
				delete it3->second;
			}
			delete *it2;
		}
		delete it1->second;
	}
}

//转换时间为24小时
string CBall::ConvertTimeTo24(const string &strTime)
{
	char ch;
	string strTmp = strTime;
	StringVector strVec;
	int hour = 0, minute = 0;
	char buf[32];

	if (strTmp.empty())
	{
		return "";
	}

	//若最后面有空格,则去掉
	while (!strTmp.empty())
	{
		ch = strTmp[strTmp.size()-1];
		if (' ' == ch)
		{
			strTmp.erase(strTmp.end() - 1);
		}
		else
		{
			break;
		}
	}

	//不是12小时制
	ch = strTmp[strTmp.size()-1];
	ch = tolower(ch);
	if ('p' != ch && 'a' != ch)
	{
		return strTime;
	}

	strVec = CPublic::Split(strTmp, ':');
	if (strVec.size() != 2)
	{
		return "";
	}

	hour = atoi(strVec[0].c_str());
	minute = atoi(strVec[1].c_str());

	ch = tolower(ch);
	if ('p' == ch)
	{
		hour += 12;
	}

	sprintf(buf, "%02d:%02d", hour, minute);

	return string(buf);
}

//转换时间为时间戳
time_t CBall::ConvertTimeToSec(const string &strDate, const string &strTime)
{
	struct tm m;
	struct tm *pTm;
	time_t tmCur, tmusa;
	StringVector strVec;
	string strTmp;

	//清空
	memset(&m, 0, sizeof(m));

	//获取当前年份
	//本地时间转换为美国时间
	time(&tmCur);
	tmusa = tmCur - 43200;//相差12个小时
	pTm = localtime(&tmusa);
	m.tm_year = pTm->tm_year;

	//获取日期
	strVec = CPublic::Split(strDate, '-');
	if (strVec.size() != 2) return 0;
	m.tm_mon  = atoi(strVec[0].c_str()) - 1;
	m.tm_mday = atoi(strVec[1].c_str());

	//获取时间
	strTmp = ConvertTimeTo24(strTime);
	strVec = CPublic::Split(strTmp, ':');
	if (strVec.size() != 2) return 0;
	m.tm_hour = atoi(strVec[0].c_str());
	m.tm_min = atoi(strVec[1].c_str());

	//转换为时间戳
	return mktime(&m);
}

//获取美国时间的前一天, bToday:true今天 false昨天
string CBall::GetUsaDate(bool bToday)
{
	time_t tmCur, tmusa;
	struct tm *pTm;
	char buf[256];

	//本地时间转换为美国时间
	time(&tmCur);
	if (bToday)
	{
		tmusa = tmCur - 43200;//相差12个小时
	}
	else
	{
		//昨天:相差12个小时+24小时
		tmusa = tmCur - 129600;//相差12个小时
	}

	//转换时间
	pTm = localtime(&tmusa);
	sprintf(buf, "%04d-%02d-%02d", pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday);
	return string(buf);
}

//释放空间
void CBall::FreeBallData(BallData &ballData)
{
	for (VecList::iterator it = ballData.DataVecList.begin(); it != ballData.DataVecList.end(); ++it)
	{
		delete *it;
	}
	ballData.FieldVec.clear();
	ballData.DataVecList.clear();
}

//获取网页错误
int CBall::GetHtmlError(const string &html)
{
	int ret;
	string strErr;

	ret = ERR_UNKNOWN;
	//判断是否登陆成功
	if (CPublic::ContainSubstr(html, "uid") && CPublic::ContainSubstr(html, "newdomain"))
	{
		//登陆成功
		ret = ERR_OK;
	}
	else if (CPublic::ContainSubstr(html, "uid") && CPublic::ContainSubstr(html, "chg_passwd"))
	{
		//登陆成功,提示修改密码
		ret = ERR_EDITPASSWD;
	}
	else if (CPublic::ContainSubstr(html, "Array"))
	{
		//采集成功
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
	else if (CPublic::ContainSubstr(html, "System error"))
	{
		//系统错误,出现在赛果
		ret = ERR_SYSTEMERROR;
	}
	else
	{
		//目前还未遇到的错误信息,写入文件
		static bool bFirst = true;
		string strFileTime = "/var/log/balld/" + CPublic::GetCurrentFileTime() + ".html";

		if (bFirst)
		{
			bFirst = false;
			mkdir("/var/log/balld", 0755);
		}

		CPublic::WriteToFile(strFileTime, html);
	}

	if (!strErr.empty())
	{
		cout << "strErr=" << strErr << endl;
	}

	return ret;
}

