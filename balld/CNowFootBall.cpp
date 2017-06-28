#include <CNowFootBall.h>
#include <CRegex.h>
#include <CMysql.h>
#include <CPublic.h>
#include <CSettings.h>
#include <CJson.h>

CNowFootBall::CNowFootBall()
	: CBall(TIME_CURRENT)
{
}

CNowFootBall::~CNowFootBall()
{
}

//采集
bool CNowFootBall::Collect()
{
	int size;
	BallData ballData;
	MatchBallList mbList;
	int betArray[] = {BET_RE, BET_RPD, BET_RT, BET_RF};
	PParse FuncArray[] = {&CNowFootBall::ParseDuying, &CNowFootBall::ParseBodan, &CNowFootBall::ParseTotal, &CNowFootBall::ParseHalfFull};

	//下载并解析betArray中的玩法
	size = sizeof(betArray)/sizeof(int);
	for (int i = 0; i < size; i++)
	{
		if (DownloadBallAllPage(BALL_FT, (BetType)betArray[i], ballData))
		{
			(this->*FuncArray[i])(ballData, mbList);
		}
	}
	//更新数据库
	UpdateToSql(mbList);
	//释放内存
	FreeMatchBall(mbList);

	return true;
}

//独赢
void CNowFootBall::ParseDuying(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime, strTmp;
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
			ball->iMatchType = 1;
			ball->iMasterRedcard = 0;
			ball->iGuestRedcard = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime);
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

		strTmp = kvMap["timer"];
		if (CPublic::ContainSubstr(strTmp, "中场"))
		{
			ball->iMatchCurTime = 100;
		}
		else if (CPublic::ContainSubstr(strTmp, ":"))
		{
			ball->iMatchCurTime = 101;//timer字段显示为时间,界面显示为'-'01:34a,沙滩足球
		}
		else
		{
			ball->iMatchCurTime = atoi(strTmp.c_str());
		}

		ball->iMasterRedcard= atoi(kvMap["redcard_h"].c_str());
		ball->iGuestRedcard	= atoi(kvMap["redcard_c"].c_str());
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->strMatchShowType = kvMap["strong"];
		ball->strMatchHrShowType = kvMap["hstrong"];
		ball->strScore		= CreateScoreJson(kvMap["score_h"], kvMap["score_c"], ball->iMatchCurTime);
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
void CNowFootBall::ParseBodan(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime, strTmp;
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
			ball->iMatchType = 1;
			ball->iMasterRedcard = 0;
			ball->iGuestRedcard = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime);
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

		strTmp = kvMap["timer"];
		if (CPublic::ContainSubstr(strTmp, "中场"))
		{
			ball->iMatchCurTime = 100;
		}
		else if (CPublic::ContainSubstr(strTmp, ":"))
		{
			ball->iMatchCurTime = 101;//timer字段显示为时间,界面显示为'-'01:34a,沙滩足球
		}
		else
		{
			ball->iMatchCurTime = atoi(strTmp.c_str());
		}

		ball->iMasterRedcard= atoi(kvMap["redcard_h"].c_str());
		ball->iGuestRedcard	= atoi(kvMap["redcard_c"].c_str());
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->strMatchShowType = kvMap["strong"];
		ball->strBodan		= CreateBodanJson(kvMap);
	}
}

//总入球
void CNowFootBall::ParseTotal(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime, strTmp;
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
			ball->iMatchType = 1;
			ball->iMasterRedcard = 0;
			ball->iGuestRedcard = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime);
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

		strTmp = kvMap["timer"];
		if (CPublic::ContainSubstr(strTmp, "中场"))
		{
			ball->iMatchCurTime = 100;
		}
		else if (CPublic::ContainSubstr(strTmp, ":"))
		{
			ball->iMatchCurTime = 101;//timer字段显示为时间,界面显示为'-'01:34a,沙滩足球
		}
		else
		{
			ball->iMatchCurTime = atoi(strTmp.c_str());
		}

		ball->iMasterRedcard= atoi(kvMap["redcard_h"].c_str());
		ball->iGuestRedcard	= atoi(kvMap["redcard_c"].c_str());
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->strMatchShowType = kvMap["strong"];
		ball->strZrq		= CreateTotalJson(kvMap);
	}
}

//半全场
void CNowFootBall::ParseHalfFull(const BallData &data, MatchBallList &lst)
{
	StringMap kvMap;
	StringVector *sv;
	uint size, szField, szVec;
	MatchBall *ball;
	string strGid, strDateTime, strDate, strTime, strTmp;
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
			ball->iMatchType = 1;
			ball->iMasterRedcard = 0;
			ball->iGuestRedcard = 0;
			lst.push_back(ball);
		}

		//赋值
		//处理日期
		CRegex::GetDateTime(strDateTime, strDate, strTime);
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

		strTmp = kvMap["timer"];
		if (CPublic::ContainSubstr(strTmp, "中场"))
		{
			ball->iMatchCurTime = 100;
		}
		else if (CPublic::ContainSubstr(strTmp, ":"))
		{
			ball->iMatchCurTime = 101;//timer字段显示为时间,界面显示为'-'01:34a,沙滩足球
		}
		else
		{
			ball->iMatchCurTime = atoi(strTmp.c_str());
		}

		ball->iMasterRedcard= atoi(kvMap["redcard_h"].c_str());
		ball->iGuestRedcard	= atoi(kvMap["redcard_c"].c_str());
		ball->strMatchDate	= strDate;
		ball->strMatchTime	= strTime;
		ball->iCoverDate	= sec;
		ball->strMatchShowType = kvMap["strong"];
		ball->strBqc		= CreateHalfFullJson(kvMap);
	}
}

//生成独赢赔率json
string CNowFootBall::CreateDyOddsJson(const string &mh, const string &mc, const string &mn,
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
string CNowFootBall::CreateRqNumJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//全场
	jobj["ratio"]	= kvmap["ratio"];

	//半场
	jobj["hratio"]	= kvmap["hratio"];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成让球赔率json
string CNowFootBall::CreateRqOddsJson(const string &rh, const string &rc, const string &hrh, const string &hrc)
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
string CNowFootBall::CreateDxNumJson(StringMap &kvmap)
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
string CNowFootBall::CreateDxOddsJson(const string &ouh, const string &ouc,
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
string CNowFootBall::CreateBodanJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//规则:
	//HxCy 代表H队和C队的比分为x:y
	//ior_OVH 代表其他
	//2队:1:0 2:0 2:1 3:0 3:1 3:2 4:0 4:1 4:2 4:3
	//共用:0:0 1:1 2:2 3:3 4:4
	jobj["ior_H1C0"] = kvmap["ior_RH1C0"];
	jobj["ior_H2C0"] = kvmap["ior_RH2C0"];
	jobj["ior_H2C1"] = kvmap["ior_RH2C1"];
	jobj["ior_H3C0"] = kvmap["ior_RH3C0"];
	jobj["ior_H3C1"] = kvmap["ior_RH3C1"];
	jobj["ior_H3C2"] = kvmap["ior_RH3C2"];
	jobj["ior_H4C0"] = kvmap["ior_RH4C0"];
	jobj["ior_H4C1"] = kvmap["ior_RH4C1"];
	jobj["ior_H4C2"] = kvmap["ior_RH4C2"];
	jobj["ior_H4C3"] = kvmap["ior_RH4C3"];

	jobj["ior_H0C1"] = kvmap["ior_RH0C1"];
	jobj["ior_H0C2"] = kvmap["ior_RH0C2"];
	jobj["ior_H1C2"] = kvmap["ior_RH1C2"];
	jobj["ior_H0C3"] = kvmap["ior_RH0C3"];
	jobj["ior_H1C3"] = kvmap["ior_RH1C3"];
	jobj["ior_H2C3"] = kvmap["ior_RH2C3"];
	jobj["ior_H0C4"] = kvmap["ior_RH0C4"];
	jobj["ior_H1C4"] = kvmap["ior_RH1C4"];
	jobj["ior_H2C4"] = kvmap["ior_RH2C4"];
	jobj["ior_H3C4"] = kvmap["ior_RH3C4"];

	jobj["ior_H0C0"] = kvmap["ior_RH0C0"];
	jobj["ior_H1C1"] = kvmap["ior_RH1C1"];
	jobj["ior_H2C2"] = kvmap["ior_RH2C2"];
	jobj["ior_H3C3"] = kvmap["ior_RH3C3"];
	jobj["ior_H4C4"] = kvmap["ior_RH4C4"];
	jobj["ior_OVH"] = kvmap["ior_ROVH"];
	jobj["ior_OVC"] = kvmap["ior_ROVC"];//无用

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成总入球json
string CNowFootBall::CreateTotalJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["ior_T01"]	= kvmap["ior_RT01"];
	jobj["ior_T23"] = kvmap["ior_RT23"];
	jobj["ior_T46"] = kvmap["ior_RT46"];
	jobj["ior_OVER"] = kvmap["ior_ROVER"];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成半全场json
string CNowFootBall::CreateHalfFullJson(StringMap &kvmap)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//规则:
	//主/主 主/和 主/客 和/主 和/和 和/客 客/主 客/和 客/客
	jobj["ior_FHH"] = kvmap["ior_RFHH"];
	jobj["ior_FHN"] = kvmap["ior_RFHN"];
	jobj["ior_FHC"] = kvmap["ior_RFHC"];
	jobj["ior_FNH"] = kvmap["ior_RFNH"];
	jobj["ior_FNN"] = kvmap["ior_RFNN"];
	jobj["ior_FNC"] = kvmap["ior_RFNC"];
	jobj["ior_FCH"] = kvmap["ior_RFCH"];
	jobj["ior_FCN"] = kvmap["ior_RFCN"];
	jobj["ior_FCC"] = kvmap["ior_RFCC"];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成比分json
string CNowFootBall::CreateScoreJson(const string &scoreh, const string &scorec, int curTime)
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_h"] = atoi(scoreh.c_str());//主队分数
	jobj["score_c"] = atoi(scorec.c_str());//客队分数
	jobj["time"]	= curTime;//比赛进行时间

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//从链表搜索gid相同的数据
MatchBall *CNowFootBall::FindMatchBall(MatchBallList &lst, const string &gid)
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
void CNowFootBall::UpdateToSql(const MatchBallList &lst)
{
	MatchBall *ball;
	string strCmd, strErr, strScore, strLastScore;
	StringVector strVec;
	char buf[256];
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

		//读取比分结果的处理
		strCmd = "select score from football where match_id='" + ball->strMatchId + "'";
		mysql.Query(strCmd);
		mysql.GetResult();
		//读出结果,连接完结果,再replace
		if (mysql.NextRow())
		{
			//存在
			//判断score的比分是否一样
			strScore = mysql.ValueString(0);
			//以;分割
			strVec = CPublic::Split(strScore, ';');
			if (strVec.empty())
			{
				strScore = ball->strScore + ';';
			}
			else
			{
				//取最后一个
				strLastScore = strVec.back();
				if (!IsSameScore(strLastScore, ball->strScore))
				{
					//不一样
					strScore += ball->strScore + ';';
				}
			}
		}
		else
		{
			//不存在
			strScore = ball->strScore+ ';';
		}

		//match_hr_showType
		strCmd = "replace into ball(match_id, match_halfId, match_name, master_name, "
		"guest_name, match_date, match_time, score, dy, rqnum, rq, dxnum, dx, dan, shuang, "
		"rlj, bodan, zrq, bqc, ball_type, match_showType, match_hr_showType, "
		"match_curTime, master_id, guest_id, cover_date, match_type, master_redcard, guest_redcard) values('";

		strCmd += ball->strMatchId + "', '" + ball->strMatchHalfId + "', '" + ball->strMatchName
				+ "', '" + ball->strMasterName + "', '" + ball->strGuestName + "', '" + ball->strMatchDate
				+ "', '" + ball->strMatchTime + "', '" + strScore + "', '" + ball->strDy + "', '"
				+ ball->strRqnum + "', '" + ball->strRq + "', '" + ball->strDxnum + "', '" + ball->strDx
				+ "', '" + ball->strDan + "', '" + ball->strShuang + "', '" + ball->strRlj + "', '"
				+ ball->strBodan + "', '" + ball->strZrq + "', '" + ball->strBqc + "', 'FT', '"
				+ ball->strMatchShowType + "', '" + ball->strMatchHrShowType + "', ";

		sprintf(buf, "%d, %d, %d, %ld, %d, %d, %d)", ball->iMatchCurTime, ball->iMasterId, ball->iGuestId, ball->iCoverDate,
				ball->iMatchType, ball->iMasterRedcard, ball->iGuestRedcard);
		strCmd += buf;

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

	/*


//			strCmd = "update football set match_halfId='" + ball->strMatchHalfId + "', match_date='"
//					+ ball->strMatchDate + "', match_time='" + ball->strMatchTime + "', score='"
//					+ ball->strScore + "', dy='" + ball->strDy + "', rqnum='" + ball->strRqnum + "', rq='"
//					+ ball->strRq + "', dxnum='" + ball->strDxnum + "', dx='" + ball->strDx + "', dan='"
//					+ ball->strDan + "', shuang='" + ball->strShuang + "', rlj='" + ball->strRlj + "', bodan='"
//					+ ball->strBodan + "', zrq='" + ball->strZrq + "', bqc='" + ball->strBqc
//					+ "', ball_type='FT', match_showType='" + ball->strMatchShowType + "', match_hr_showType='"
//					+ ball->strMatchHrShowType + "', ";
//
//			sprintf(buf, "match_curTime=%d, cover_date=%ld, match_type=%d, master_redcard=%d, guest_redcard=%d "
//					"where match_id='%s'",ball->iMatchCurTime, ball->iCoverDate, ball->iMatchType,
//					ball->iMasterRedcard, ball->iGuestRedcard, ball->strMatchId.c_str());
//			strCmd += buf;
	*/
}

//释放内存
void CNowFootBall::FreeMatchBall(MatchBallList &lst)
{
	for (MatchBallList::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		delete *it;
	}
	lst.clear();
}

//比较2个json比分是否相同
bool CNowFootBall::IsSameScore(const string &scoreJson1, const string &scoreJson2)
{
	Json::Value jv1, jv2;
	int score1h, score1c, score2h, score2c;

	if (!CJson::ParseStringToJsonValue(scoreJson1, jv1)
		|| !CJson::ParseStringToJsonValue(scoreJson2, jv2))
	{
		return false;
	}

	score1h = jv1["score_h"].asInt();
	score1c = jv1["score_c"].asInt();
	score2h = jv2["score_h"].asInt();
	score2c = jv2["score_c"].asInt();

	if (score1h != score2h || score1c != score2c)
	{
		return false;
	}

	return true;
}

