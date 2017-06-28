#include <CMatchResult.h>
#include <CRegex.h>
#include <CMysql.h>
#include <CPublic.h>
#include <CSettings.h>

CMatchResult::CMatchResult()
{

}

CMatchResult::~CMatchResult()
{

}

//采集
bool CMatchResult::Collect()
{
	MatchResultList mrList;

	//处理所有赛果
	DealFtResult(mrList);
//	DealBkResult(mrList);

	//更新到数据库
	UpdateToSql(mrList);

	//释放内存
	FreeMatchResult(mrList);

	return true;
}

//处理足球赛果数据
void CMatchResult::DealFtResult(MatchResultList &mrList)
{
	string strHtml;
	string strDateTime;
	StringVector strVecId;
	StringMap strGidHtmlMap;

	/***下载今天足球普通赛果***/
	strDateTime = GetUsaDate(true);
	if (!DownloadBallResult(BALL_FT, BET_RES, strDateTime, strHtml))
	{
		return;
	}
	//处理赛果
	ParseFtResult(strHtml, mrList);
	//匹配存在更多选项的id
	CRegex::GetFtResMatchIdMore(strHtml, strVecId);
	//下载更多选项页面
	for (StringVector::iterator it = strVecId.begin(); it != strVecId.end(); ++it)
	{
		if (!DownloadBallResultMore(BALL_FT, *it, strHtml)) continue;
		strGidHtmlMap[*it] = strHtml;
	}
	//处理更多选项的数据
	ParseFtResultMore(strGidHtmlMap, mrList);

	/***下载昨天足球普通赛果***/
	strDateTime = GetUsaDate(false);
	if (!DownloadBallResult(BALL_FT, BET_RES, strDateTime, strHtml))
	{
		return;
	}
	//处理赛果
	ParseFtResult(strHtml, mrList);
	//匹配存在更多选项的id
	CRegex::GetFtResMatchIdMore(strHtml, strVecId);
	//下载更多选项页面
	for (StringVector::iterator it = strVecId.begin(); it != strVecId.end(); ++it)
	{
		if (!DownloadBallResultMore(BALL_FT, *it, strHtml)) continue;
		strGidHtmlMap[*it] = strHtml;
	}
	//处理更多选项的数据
	ParseFtResultMore(strGidHtmlMap, mrList);
}

//匹配一个赛事
void CMatchResult::ParseFtResult(const string &strHtml, MatchResultList &mrList)
{
	StringVector strMatchVec, strVecTime, strVecScore, strVecTeam, strVecId;
	string strTitle;
	MatchResult *mRes;
	uint sz;

	//匹配所有赛事块
	CRegex::GetFtResMatch(strHtml, strMatchVec);
	for (StringVector::iterator it = strMatchVec.begin(); it != strMatchVec.end(); ++it)
	{
		CRegex::GetFtResMatchTitle(*it, strTitle);
		CRegex::GetFtResMatchId(*it, strVecId);
		CRegex::GetFtResTime(*it, strVecTime);
		CRegex::GetFtResTeam(*it, strVecTeam);
		CRegex::GetFtResScore(*it, strVecScore);

		//必须要相等，否则崩溃
		sz = strVecId.size();
		if (strTitle.empty()
			|| strVecTime.size() != sz*2
			|| strVecTeam.size() != sz*2
			|| strVecScore.size() != sz*4)
		{
			continue;
		}

		for (uint i = 0; i < sz; i++)
		{
			mRes = new MatchResult;
			mRes->iBallType		= BALL_FT;
			mRes->strMatchId	= strVecId[i];
			mRes->strMatchTitle	= strTitle;
			mRes->strDate		= strVecTime[i*2+0];
			mRes->strTime		= strVecTime[i*2+1];
			mRes->strTeam[0]	= strVecTeam[i*2+0];
			mRes->strTeam[1]	= strVecTeam[i*2+1];
			for (int j = 0; j < 4; j++)
			{
				mRes->strScore[j] = strVecScore[i*4+j];
			}
			mRes->iTime			= ConvertTimeToSec(mRes->strDate, mRes->strTime);
			mRes->bMore		 	= false;
			mrList.push_back(mRes);
		}
	}
}

//匹配足球赛事的更多,htmlMap保存着id和html
void CMatchResult::ParseFtResultMore(const StringMap &htmlMap, MatchResultList &mrList)
{
	StringVector strVecScore;
	MatchResult *mRes;

	for (StringMap::const_iterator it = htmlMap.begin(); it != htmlMap.end(); ++it)
	{
		//匹配结果
		CRegex::GetFtResMoreScore(it->second, strVecScore);
		if (strVecScore.size() != 16) continue;

		//寻找已存在的结果数据
		mRes = FindMatchResult(mrList, it->first);
		if (!mRes) continue;
		mRes->bMore = true;

		//赋值到数组
		for (int i = 0; i < 16; i++)
		{
			mRes->strMoreScore[i] = strVecScore[i];
		}
	}
}

//生成足球比分json
string CMatchResult::CreateFtScoreJson(const string score[4])
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_h_1"] 	= score[0];//半场 half
	jobj["score_h_2"] 	= score[1];
	jobj["score_f_1"] 	= score[2];//全场 full
	jobj["score_f_2"] 	= score[3];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成足球更多结果json
string CMatchResult::CreateFtMoreScoreJson(const string moreScore[16], bool bMore)
{
	//不存在更多，返回空
	if (!bMore)
	{
		return "";
	}

	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	//与网页的顺序一样
	for (uint i = 0; i < 16; i++)
	{
		jobj.append(moreScore[i]);
	}

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成篮球比分json
string CMatchResult::CreateBkScoreJson(const string score[16])
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_1_1"] 	= score[0];//第1节
	jobj["score_1_2"] 	= score[1];
	jobj["score_2_1"] 	= score[2];//第2节
	jobj["score_2_2"] 	= score[3];
	jobj["score_3_1"] 	= score[4];//第3节
	jobj["score_3_2"] 	= score[5];
	jobj["score_4_1"] 	= score[6];//第4节
	jobj["score_4_2"]	= score[7];
	jobj["score_fh_1"]	= score[8];//上半场 first half
	jobj["score_fh_2"]	= score[9];
	jobj["score_sh_1"]	= score[10];//下半场 second half
	jobj["score_sh_2"]	= score[11];
	jobj["score_ot_1"]	= score[12];//加时 overtime
	jobj["score_ot_2"]	= score[13];
	jobj["score_f_1"]	= score[14];//全场 full
	jobj["score_f_2"]	= score[15];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成网球比分json,前面14个数据
string CMatchResult::CreateTnScoreJson(const string score[16])
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_1_1"] 	= score[0];//{GAME_TN_1}
	jobj["score_1_2"] 	= score[1];
	jobj["score_2_1"] 	= score[2];//{GAME_TN_2}
	jobj["score_2_2"] 	= score[3];
	jobj["score_3_1"] 	= score[4];//{GAME_TN_3}
	jobj["score_3_2"] 	= score[5];
	jobj["score_4_1"] 	= score[6];//{GAME_TN_4}
	jobj["score_4_2"]	= score[7];
	jobj["score_5_1"]	= score[8];//{GAME_TN_5}
	jobj["score_5_2"]	= score[9];
	jobj["score_6_1"]	= score[10];//{GAME_TN_6}
	jobj["score_6_2"]	= score[11];
	jobj["score_end_1"]	= score[12];//完赛(局)
	jobj["score_end_2"]	= score[13];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成排球比分json,前面6个数据
string CMatchResult::CreateVbScoreJson(const string score[16])
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_1_1"] 	= score[0];//{GAME_VB_1}
	jobj["score_1_2"] 	= score[1];
	jobj["score_2_1"] 	= score[2];//{GAME_VB_2}
	jobj["score_2_2"] 	= score[3];
	jobj["score_end_1"]	= score[4];//完赛(局)
	jobj["score_end_2"]	= score[5];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成棒球比分jso
string CMatchResult::CreateBsScoreJson(const string score[4])
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_h_1"] 	= score[0];//半场 half
	jobj["score_h_2"] 	= score[1];
	jobj["score_f_1"] 	= score[2];//全场 full
	jobj["score_f_2"] 	= score[3];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//生成其他比分json
string CMatchResult::CreateOpScoreJson(const string score[4])
{
	Json::Value jobj;
	Json::FastWriter jw;
	string strJson;

	jobj["score_h_1"] 	= score[0];//半场 half
	jobj["score_h_2"] 	= score[1];
	jobj["score_f_1"] 	= score[2];//全场 full
	jobj["score_f_2"] 	= score[3];

	strJson = jw.write(jobj);
	CPublic::RemoveChar(strJson, '\n');
	return strJson;
}

//处理篮球赛果数据
void CMatchResult::DealBkResult(MatchResultList &mrList)
{
	string strHtml;
	string strDateTime = "2013-01-01";
	StringVector strVecId;
	StringMap strGidHtmlMap;

	//下载足球普通赛果
	if (!DownloadBallResult(BALL_BK, BET_RES, strDateTime, strHtml))
	{
		return;
	}
	//处理赛果
	ParseBkResult(strHtml, mrList);
}

//匹配篮球赛事
void CMatchResult::ParseBkResult(const string &strHtml, MatchResultList &mrList)
{
	StringVector strMatchVec, strVecTime, strVecScore, strVecTeam, strVecId;
	string strTitle;
	MatchResult *mRes;
	uint sz;

	//匹配所有赛事块
	CRegex::GetFtResMatch(strHtml, strMatchVec);
	for (StringVector::iterator it = strMatchVec.begin(); it != strMatchVec.end(); ++it)
	{
		CRegex::GetBkResMatchTitle(*it, strTitle);
		CRegex::GetBkResMatchId(*it, strVecId);
		CRegex::GetBkResTime(*it, strVecTime);
		CRegex::GetBkResTeam(*it, strVecTeam);
		CRegex::GetBkResScore(*it, strVecScore);

		//必须要相等，否则崩溃
		sz = strVecId.size();
		if (strTitle.empty()
			|| strVecTime.size() != sz*2
			|| strVecTeam.size() != sz*2
			|| strVecScore.size() != sz*16)
		{
			continue;
		}

		for (uint i = 0; i < sz; i++)
		{
			mRes = new MatchResult;
			mRes->iBallType		= BALL_BK;
			mRes->strMatchId	= strVecId[i];
			mRes->strMatchTitle	= strTitle;
			mRes->strDate		= strVecTime[i*2+0];
			mRes->strTime		= strVecTime[i*2+1];
			mRes->strTeam[0]	= strVecTeam[i*2+0];
			mRes->strTeam[1]	= strVecTeam[i*2+1];
			for (int j = 0; j < 16; j++)
			{
				mRes->strMoreScore[j] = strVecScore[i*16+j];
			}
			mRes->iTime			= ConvertTimeToSec(mRes->strDate, mRes->strTime);
			mrList.push_back(mRes);
		}
	}
}

//赛果数据生成sql命令
string CMatchResult::CreateSqlCmd(const MatchResult *mRes)
{
	BallType iBallType;
	string strCmd, strScore, strMoreScore;
	char buf[256];
	const string strBallTypeArray[] =
	{
		"FT", "BK", "TN", "VB", "BS", "OP"
	};

	iBallType = mRes->iBallType;
	strScore.clear();
	strMoreScore.clear();//只有足球存在更多比分
	switch (iBallType)
	{
	case BALL_FT:
		strScore = CreateFtScoreJson(mRes->strScore);
		strMoreScore = CreateFtMoreScoreJson(mRes->strMoreScore, mRes->bMore);
		break;
	case BALL_BK:
		strScore = CreateBkScoreJson(mRes->strMoreScore);
		break;
	case BALL_TN:
		strScore = CreateTnScoreJson(mRes->strMoreScore);
		break;
	case BALL_VB:
		strScore = CreateVbScoreJson(mRes->strMoreScore);
		break;
	case BALL_BS:
		strScore = CreateBsScoreJson(mRes->strScore);
		break;
	case BALL_OP:
		strScore = CreateOpScoreJson(mRes->strScore);
		break;
	default:
		break;
	}

	strCmd = "replace into match_result(match_id, match_date, match_time, ball_type, title, "
	"master_name, guset_name, score, more_score, cover_date) values('";

	strCmd += mRes->strMatchId + "', '" + mRes->strDate + "', '" + mRes->strTime + "', '"
			+ strBallTypeArray[mRes->iBallType] + "', '" + mRes->strMatchTitle + "', '"
			+ mRes->strTeam[0] + "', '" + mRes->strTeam[1] + "', '" + strScore + "', '" + strMoreScore + "', ";
	sprintf(buf, "%lu)", mRes->iTime);
	strCmd += buf;

	return strCmd;
}

//更新赛果到数据库
void CMatchResult::UpdateToSql(const MatchResultList &mrList)
{
	string strCmd;
	CMysql mysql;
	SqlInfo sqlInfo;
	CSettings *pSet = CSettings::Instance();

	pSet->GetSqlInfo(sqlInfo);

	if (!mysql.Init())
	{
		return;
	}

	if (!mysql.Connect(sqlInfo.ipAddr, sqlInfo.user, sqlInfo.passwd, sqlInfo.dbname, sqlInfo.port))
	{
		return;
	}

	for (MatchResultList::const_iterator it = mrList.begin(); it != mrList.end(); ++it)
	{
		strCmd = CreateSqlCmd(*it);
		mysql.Query(strCmd);
//		cout << "error:" << mysql.GetSqlError() << endl;
	}
	mysql.Close();
}

//释放赛事
void CMatchResult::FreeMatchResult(MatchResultList &mrList)
{
	for (MatchResultList::iterator it = mrList.begin(); it != mrList.end(); ++it)
	{
		delete *it;
	}
	mrList.clear();
}

//寻找足球结果
MatchResult *CMatchResult::FindMatchResult(MatchResultList &mrList, const string &id)
{
	MatchResult *mRes = NULL;
	for (MatchResultList::iterator it = mrList.begin(); it != mrList.end(); ++it)
	{
		mRes = *it;
		if (id == mRes->strMatchId)
		{
			break;
		}
		else
		{
			mRes = NULL;
		}
	}
	return mRes;
}

