#include <CFootBall.h>
#include <CRegex.h>
#include <CMysql.h>
#include <CPublic.h>
#include <CSettings.h>

CFootBall::CFootBall()
{
}

CFootBall::CFootBall(uint type)
	: CBall(type)
{
}

CFootBall::~CFootBall()
{
}

//采集
bool CFootBall::Collect()
{
	int size;
	BallData ballData;
//	MatchChampionMap mcMap;
	MatchBallList lst;
	int betArray[] = {BET_R, BET_PD, BET_T, BET_F, BET_P3};//, BET_FS, BET_RES
	PParse FuncArray[] = {&CFootBall::ParseDuying, &CFootBall::ParseBodan, &CFootBall::ParseTotal, &CFootBall::ParseHalfFull, &CFootBall::ParsePass};

	//下载并解析betArray中的玩法
	size = sizeof(betArray)/sizeof(int);
	for (int i = 0; i < size; i++)
	{
		if (DownloadBallAllPage(BALL_FT, (BetType)betArray[i], ballData))
		{
			(this->*FuncArray[i])(ballData, lst);
		}
	}
	//处理数据库
	UpdateToSql(lst);
	//释放内存
	FreeMatchBall(lst);


//	//下载冠军数据
//	if (DownloadBallChampion(BALL_FT, mcMap))
//	{
//		//...
//	}
//
//	//下载赛果数据
//	//...

	return true;
}

//独赢
void CFootBall::ParseDuying(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime;
	bool bRunBall;//是否是滚球
	long sec;

	szField = data.FieldVec.size();
	for (VecList::const_iterator it = data.DataVecList.begin(); it != data.DataVecList.end(); ++it)
	{
		sv = *it;
		szVec = sv->size();
		size = (szVec > szField) ? szField : szVec;

		//生成映射
		kvMap.clear();
		for (uint i = 0; i < size; i++)
		{
			kvMap[data.FieldVec.at(i)] = sv->at(i);
		}

		//取出对应的字段
		strGid = kvMap["gid"];
		strDateTime = kvMap["datetime"];

		//寻找数据
		ball = FindMatchBall(lst, strGid);
		if (!ball)
		{
			ball = new MatchBall;
			ball->iCoverDate = 0;
			ball->iGuestId = 0;
			ball->iMasterId = 0;
			ball->iMatchType = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime, &bRunBall);
		//时间转换为24小时制
		strTime = ConvertTimeTo24(strTime);
		//时间转换为秒
		sec = ConvertTimeToSec(strDate, strTime);

		ball->strMatchId	= strGid;
		ball->strMatchHalfId= kvMap["hgid"];
		ball->strMatchName	= kvMap["league"];
		ball->iMasterId		= atoi(kvMap["gnum_h"].c_str());
		ball->strMasterName	= kvMap["team_h"];
		ball->iGuestId		= atoi(kvMap["gnum_c"].c_str());
		ball->strGuestName	= kvMap["team_c"];
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->iMatchType	= bRunBall ? 1 : 0;//滚球为1 不是为0
		ball->strMatchShowType = kvMap["strong"];
		ball->strMatchHrShowType = kvMap["hstrong"];
		ball->strDy			= CreateDyOddsJson(kvMap["ior_MH"], kvMap["ior_MC"], kvMap["ior_MN"],
														kvMap["ior_HMH"], kvMap["ior_HMC"], kvMap["ior_HMN"]);
		ball->strRqnum		= CreateRqNumJson(kvMap);
		ball->strRq			= CreateRqOddsJson(kvMap["ior_RH"], kvMap["ior_RC"],
														kvMap["ior_HRH"], kvMap["ior_HRC"]);
		ball->strDxnum		= CreateDxNumJson(kvMap);
		ball->strDx			= CreateDxOddsJson(kvMap["ior_OUH"], kvMap["ior_OUC"],
														kvMap["ior_HOUH"], kvMap["ior_HOUC"]);
		ball->strDan		= kvMap["ior_EOO"];
		ball->strShuang		= kvMap["ior_EOE"];
	}
}

//波胆
void CFootBall::ParseBodan(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime;
	bool bRunBall;//是否是滚球
	long sec;

	szField = data.FieldVec.size();
	for (VecList::const_iterator it = data.DataVecList.begin(); it != data.DataVecList.end(); ++it)
	{
		sv = *it;
		szVec = sv->size();
		size = (szVec > szField) ? szField : szVec;

		//生成映射
		kvMap.clear();
		for (uint i = 0; i < size; i++)
		{
			kvMap[data.FieldVec.at(i)] = sv->at(i);
		}

		//取出对应的字段
		strGid = kvMap["gid"];
		strDateTime = kvMap["datetime"];

		//寻找数据
		ball = FindMatchBall(lst, strGid);
		if (!ball)
		{
			ball = new MatchBall;
			ball->iCoverDate = 0;
			ball->iGuestId = 0;
			ball->iMasterId = 0;
			ball->iMatchType = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime, &bRunBall);
		//时间转换为24小时制
		strTime = ConvertTimeTo24(strTime);
		//时间转换为秒
		sec = ConvertTimeToSec(strDate, strTime);

		ball->strMatchId	= strGid;
		ball->strMatchName	= kvMap["league"];
		ball->iMasterId		= atoi(kvMap["gnum_h"].c_str());
		ball->strMasterName	= kvMap["team_h"];
		ball->iGuestId		= atoi(kvMap["gnum_c"].c_str());
		ball->strGuestName	= kvMap["team_c"];
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->iMatchType	= bRunBall ? 1 : 0;//滚球为1 不是为0
		ball->strMatchShowType = kvMap["strong"];
		ball->strBodan		= CreateBodanJson(kvMap);
	}
}

//总入球
void CFootBall::ParseTotal(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime;
	bool bRunBall;//是否是滚球
	long sec;

	szField = data.FieldVec.size();
	for (VecList::const_iterator it = data.DataVecList.begin(); it != data.DataVecList.end(); ++it)
	{
		sv = *it;
		szVec = sv->size();
		size = (szVec > szField) ? szField : szVec;

		//生成映射
		kvMap.clear();
		for (uint i = 0; i < size; i++)
		{
			kvMap[data.FieldVec.at(i)] = sv->at(i);
		}

		//取出对应的字段
		strGid = kvMap["gid"];
		strDateTime = kvMap["datetime"];

		//寻找数据
		ball = FindMatchBall(lst, strGid);
		if (!ball)
		{
			ball = new MatchBall;
			ball->iCoverDate = 0;
			ball->iGuestId = 0;
			ball->iMasterId = 0;
			ball->iMatchType = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime, &bRunBall);
		//时间转换为24小时制
		strTime = ConvertTimeTo24(strTime);
		//时间转换为秒
		sec = ConvertTimeToSec(strDate, strTime);

		ball->strMatchId	= strGid;
		ball->strMatchName	= kvMap["league"];
		ball->iMasterId		= atoi(kvMap["gnum_h"].c_str());
		ball->strMasterName	= kvMap["team_h"];
		ball->iGuestId		= atoi(kvMap["gnum_c"].c_str());
		ball->strGuestName	= kvMap["team_c"];
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->iMatchType	= bRunBall ? 1 : 0;//滚球为1 不是为0
		ball->strMatchShowType = kvMap["strong"];
		ball->strZrq		= CreateTotalJson(kvMap);
	}
}

//半全场
void CFootBall::ParseHalfFull(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime;
	bool bRunBall;//是否是滚球
	long sec;

	szField = data.FieldVec.size();
	for (VecList::const_iterator it = data.DataVecList.begin(); it != data.DataVecList.end(); ++it)
	{
		sv = *it;
		szVec = sv->size();
		size = (szVec > szField) ? szField : szVec;

		//生成映射
		kvMap.clear();
		for (uint i = 0; i < size; i++)
		{
			kvMap[data.FieldVec.at(i)] = sv->at(i);
		}

		//取出对应的字段
		strGid = kvMap["gid"];
		strDateTime = kvMap["datetime"];

		//寻找数据
		ball = FindMatchBall(lst, strGid);
		if (!ball)
		{
			ball = new MatchBall;
			ball->iCoverDate = 0;
			ball->iGuestId = 0;
			ball->iMasterId = 0;
			ball->iMatchType = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime, &bRunBall);
		//时间转换为24小时制
		strTime = ConvertTimeTo24(strTime);
		//时间转换为秒
		sec = ConvertTimeToSec(strDate, strTime);

		ball->strMatchId	= strGid;
		ball->strMatchName	= kvMap["league"];
		ball->iMasterId		= atoi(kvMap["gnum_h"].c_str());
		ball->strMasterName	= kvMap["team_h"];
		ball->iGuestId		= atoi(kvMap["gnum_c"].c_str());
		ball->strGuestName	= kvMap["team_c"];
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->iMatchType	= bRunBall ? 1 : 0;//滚球为1 不是为0
		ball->strMatchShowType = kvMap["strong"];
		ball->strBqc		= CreateHalfFullJson(kvMap);
	}
}

//综合过关
void CFootBall::ParsePass(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime;
	bool bRunBall;//是否是滚球
	long sec;

	szField = data.FieldVec.size();
	for (VecList::const_iterator it = data.DataVecList.begin(); it != data.DataVecList.end(); ++it)
	{
		sv = *it;
		szVec = sv->size();
		size = (szVec > szField) ? szField : szVec;

		//生成映射
		kvMap.clear();
		for (uint i = 0; i < size; i++)
		{
			kvMap[data.FieldVec.at(i)] = sv->at(i);
		}

		//取出对应的字段
		strGid = kvMap["gid"];
		strDateTime = kvMap["datetime"];

		//寻找数据
		ball = FindMatchBall(lst, strGid);
		if (!ball)
		{
			ball = new MatchBall;
			ball->iCoverDate = 0;
			ball->iGuestId = 0;
			ball->iMasterId = 0;
			ball->iMatchType = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime, &bRunBall);
		//时间转换为24小时制
		strTime = ConvertTimeTo24(strTime);
		//时间转换为秒
		sec = ConvertTimeToSec(strDate, strTime);

		ball->strMatchId	= strGid;
		ball->strMatchHalfId= kvMap["hgid"];
		ball->strMatchName	= kvMap["league"];
		ball->iMasterId		= atoi(kvMap["gnum_h"].c_str());
		ball->strMasterName	= kvMap["team_h"];
		ball->iGuestId		= atoi(kvMap["gnum_c"].c_str());
		ball->strGuestName	= kvMap["team_c"];
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->iMatchType	= bRunBall ? 1 : 0;//滚球为1 不是为0
		ball->strMatchShowType = kvMap["strong"];
		ball->strMatchHrShowType = kvMap["hstrong"];
		ball->strDy			= CreateDyOddsJson(kvMap["ior_MH"], kvMap["ior_MC"], kvMap["ior_MN"],
														kvMap["ior_HPMH"], kvMap["ior_HPMC"], kvMap["ior_HPMN"]);
		ball->strRqnum		= CreateRqNumJson(kvMap);
		ball->strRq			= CreateRqOddsJson(kvMap["ior_PRH"], kvMap["ior_PRC"],
														kvMap["ior_HPRH"], kvMap["ior_HPRC"]);
		ball->strDxnum		= CreateDxNumJson(kvMap);
		ball->strDx			= CreateDxOddsJson(kvMap["ior_POUH"], kvMap["ior_POUC"],
														kvMap["ior_HPOUH"], kvMap["ior_HPOUC"]);
		ball->strDan		= kvMap["ior_PO"];
		ball->strShuang		= kvMap["ior_PE"];
	}
}

//生成独赢赔率json
string CFootBall::CreateDyOddsJson(const string &mh, const string &mc, const string &mn,
							const string &hmh, const string &hmc, const string &hmn)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//全场
	jobj["ior_MH"] = mh;//主队
	jobj["ior_MC"] = mc;//客队
	jobj["ior_MN"] = mn;//和

	//半场
	jobj["ior_HMH"] = hmh;//主队
	jobj["ior_HMC"] = hmc;//客队
	jobj["ior_HMN"] = hmn;//和

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成让球数赔率json
string CFootBall::CreateRqNumJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["ratio"]	= kvmap["ratio"];//全场
	jobj["hratio"]	= kvmap["hratio"];//半场

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成让球赔率json
string CFootBall::CreateRqOddsJson(const string &rh, const string &rc, const string &hrh, const string &hrc)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//全场
	jobj["ior_RH"] = rh;
	jobj["ior_RC"] = rc;

	//半场
	jobj["ior_HRH"] = hrh;
	jobj["ior_HRC"] = hrc;

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成大小数json
string CFootBall::CreateDxNumJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//全场
	jobj["ratio_o"]	= kvmap["ratio_o"];
	jobj["ratio_u"]	= kvmap["ratio_u"];

	//半场
	jobj["hratio_o"]	= kvmap["hratio_o"];
	jobj["hratio_u"]	= kvmap["hratio_u"];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成大小赔率json
string CFootBall::CreateDxOddsJson(const string &ouh, const string &ouc,
									const string &houh, const string &houc)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//全场
	jobj["ior_OUH"] = ouh;
	jobj["ior_OUC"] = ouc;

	//半场
	jobj["ior_HOUH"] = houh;
	jobj["ior_HOUC"] = houc;

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成波胆josn
string CFootBall::CreateBodanJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//规则:
	//HxCy 代表H队和C队的比分为x:y
	//ior_OVH 代表其他
	//2队:1:0 2:0 2:1 3:0 3:1 3:2 4:0 4:1 4:2 4:3
	//共用:0:0 1:1 2:2 3:3 4:4
	jobj["ior_H1C0"] = kvmap["ior_H1C0"];
	jobj["ior_H2C0"] = kvmap["ior_H2C0"];
	jobj["ior_H2C1"] = kvmap["ior_H2C1"];
	jobj["ior_H3C0"] = kvmap["ior_H3C0"];
	jobj["ior_H3C1"] = kvmap["ior_H3C1"];
	jobj["ior_H3C2"] = kvmap["ior_H3C2"];
	jobj["ior_H4C0"] = kvmap["ior_H4C0"];
	jobj["ior_H4C1"] = kvmap["ior_H4C1"];
	jobj["ior_H4C2"] = kvmap["ior_H4C2"];
	jobj["ior_H4C3"] = kvmap["ior_H4C3"];

	jobj["ior_H0C1"] = kvmap["ior_H0C1"];
	jobj["ior_H0C2"] = kvmap["ior_H0C2"];
	jobj["ior_H1C2"] = kvmap["ior_H1C2"];
	jobj["ior_H0C3"] = kvmap["ior_H0C3"];
	jobj["ior_H1C3"] = kvmap["ior_H1C3"];
	jobj["ior_H2C3"] = kvmap["ior_H2C3"];
	jobj["ior_H0C4"] = kvmap["ior_H0C4"];
	jobj["ior_H1C4"] = kvmap["ior_H1C4"];
	jobj["ior_H2C4"] = kvmap["ior_H2C4"];
	jobj["ior_H3C4"] = kvmap["ior_H3C4"];

	jobj["ior_H0C0"] = kvmap["ior_H0C0"];
	jobj["ior_H1C1"] = kvmap["ior_H1C1"];
	jobj["ior_H2C2"] = kvmap["ior_H2C2"];
	jobj["ior_H3C3"] = kvmap["ior_H3C3"];
	jobj["ior_H4C4"] = kvmap["ior_H4C4"];
	jobj["ior_OVH"] = kvmap["ior_OVH"];
	jobj["ior_OVC"] = kvmap["ior_OVC"];//无用

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成总入球json
string CFootBall::CreateTotalJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["ior_T01"]	= kvmap["ior_T01"];
	jobj["ior_T23"] = kvmap["ior_T23"];
	jobj["ior_T46"] = kvmap["ior_T46"];
	jobj["ior_OVER"] = kvmap["ior_OVER"];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成半全场json
string CFootBall::CreateHalfFullJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//规则:
	//主/主 主/和 主/客 和/主 和/和 和/客 客/主 客/和 客/客
	jobj["ior_FHH"] = kvmap["ior_FHH"];
	jobj["ior_FHN"] = kvmap["ior_FHN"];
	jobj["ior_FHC"] = kvmap["ior_FHC"];
	jobj["ior_FNH"] = kvmap["ior_FNH"];
	jobj["ior_FNN"] = kvmap["ior_FNN"];
	jobj["ior_FNC"] = kvmap["ior_FNC"];
	jobj["ior_FCH"] = kvmap["ior_FCH"];
	jobj["ior_FCN"] = kvmap["ior_FCN"];
	jobj["ior_FCC"] = kvmap["ior_FCC"];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//从链表搜索gid相同的数据
MatchBall *CFootBall::FindMatchBall(MatchBallList &lst, const string &gid)
{
	MatchBall *ball = NULL;
	for (MatchBallList::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		ball = *it;
		if (gid == ball->strMatchId)
		{
			break;
		}
		else
		{
			ball = NULL;
		}
	}
	return ball;
}

//更新到数据库,还得改进
void CFootBall::UpdateToSql(const MatchBallList &lst)
{
	MatchBall *ball;
	string strCmd, strErr;
	char buf[1024];
	CMysql mysql;
	SqlInfo sqlInfo;
	int count;
	CSettings *pSet = CSettings::Instance();

	//获取数据库信息
	pSet->GetSqlInfo(sqlInfo);

	if (!mysql.Init())
	{
		return;
	}

	if (!mysql.Connect(sqlInfo.ipAddr, sqlInfo.user, sqlInfo.passwd, sqlInfo.dbname, sqlInfo.port))
	{
		return;
	}

	count = 0;
	for (MatchBallList::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		ball = *it;
		//match_hr_showType
		strCmd = "replace into ball(match_id, match_halfId, match_name, master_name, "
		"guest_name, match_date, match_time, dy, rqnum, rq, dxnum, dx, dan, shuang, rlj, "
		"bodan, zrq, bqc, ball_type, match_showType, match_hr_showType, master_id, guest_id, "
		"cover_date, match_type) values('";

		strCmd += ball->strMatchId + "', '" + ball->strMatchHalfId + "', '" + ball->strMatchName
				+ "', '" + ball->strMasterName + "', '" + ball->strGuestName + "', '" + ball->strMatchDate
				+ "', '" + ball->strMatchTime + "', '" + ball->strDy + "', '"
				+ ball->strRqnum + "', '" + ball->strRq + "', '" + ball->strDxnum + "', '" + ball->strDx
				+ "', '" + ball->strDan + "', '" + ball->strShuang + "', '" + ball->strRlj + "', '"
				+ ball->strBodan + "', '" + ball->strZrq + "', '" + ball->strBqc + "', 'FT', '"
				+ ball->strMatchShowType + "', '" + ball->strMatchHrShowType + "', ";
		sprintf(buf, "%d, %d, %ld, %d)", ball->iMasterId, ball->iGuestId, ball->iCoverDate, ball->iMatchType);
		strCmd += buf;

//		printf("%s\n", ball->strMatchName.c_str());
		mysql.Query(strCmd);

		strErr = mysql.GetSqlError();
		if (!strErr.empty())
		{
			printf("error:%s\n", strErr.c_str());
		}
		else
		{
//			printf("数据库更新成功-%d\n", ++count);
		}
	}
	mysql.Close();
}

//释放内存
void CFootBall::FreeMatchBall(MatchBallList &lst)
{
	for (MatchBallList::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		delete *it;
	}
	lst.clear();
}

