#pragma once
#include <CBall.h>

class CMatchResult : public CBall
{
public:
	CMatchResult();
	~CMatchResult();
	//采集
	virtual bool Collect();
	//处理足球赛果数据
	void	DealFtResult(MatchResultList &mrList);
	//匹配足球赛事
	void	ParseFtResult(const string &strHtml, MatchResultList &mrList);
	//匹配足球赛事的更多,htmlMap保存着id和html
	void	ParseFtResultMore(const StringMap &htmlMap, MatchResultList &mrList);
	//生成足球比分json
	string	CreateFtScoreJson(const string score[4]);
	//生成足球更多结果json
	string	CreateFtMoreScoreJson(const string moreScore[16], bool bMore);
	//生成篮球比分json
	string	CreateBkScoreJson(const string score[16]);
	//生成网球比分json,前面14个数据
	string	CreateTnScoreJson(const string score[16]);
	//生成排球比分json,前面6个数据
	string	CreateVbScoreJson(const string score[16]);
	//生成棒球比分json
	string	CreateBsScoreJson(const string score[4]);
	//生成其他比分json
	string	CreateOpScoreJson(const string score[4]);

	//处理篮球赛果数据
	void	DealBkResult(MatchResultList &mrList);
	//匹配篮球赛事
	void	ParseBkResult(const string &strHtml, MatchResultList &mrList);

	//赛果数据生成sql命令
	string	CreateSqlCmd(const MatchResult *mRes);
	//更新赛果到数据库
	void	UpdateToSql(const MatchResultList &mrList);
	//释放赛事
	void	FreeMatchResult(MatchResultList &mrList);
	//寻找结果
	MatchResult *FindMatchResult(MatchResultList &mrList, const string &id);
};
