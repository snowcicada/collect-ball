#pragma once

#include <CBall.h>

class CFootBall : public CBall
{
public:
	CFootBall();
	CFootBall(uint type);
	~CFootBall();
	//采集
	virtual bool Collect();
	//独赢
	void	ParseDuying(const BallData &data, MatchBallList &lst);
	//波胆
	void	ParseBodan(const BallData &data, MatchBallList &lst);
	//总入球
	void	ParseTotal(const BallData &data, MatchBallList &lst);
	//半全场
	void	ParseHalfFull(const BallData &data, MatchBallList &lst);
	//综合过关
	void	ParsePass(const BallData &data, MatchBallList &lst);
	//生成独赢赔率json
	string	CreateDyOddsJson(const string &mh, const string &mc, const string &mn,
								const string &hmh, const string &hmc, const string &hmn);
	//生成让球数json
	string	CreateRqNumJson(StringMap &kvmap);
	//生成让球赔率json
	string	CreateRqOddsJson(const string &rh, const string &rc, const string &hrh, const string &hrc);
	//生成大小数json
	string	CreateDxNumJson(StringMap &kvmap);
	//生成大小赔率json
	string	CreateDxOddsJson(const string &ouh, const string &ouc, const string &houh, const string &houc);
	//生成波胆json
	string	CreateBodanJson(StringMap &kvmap);
	//生成总入球json
	string	CreateTotalJson(StringMap &kvmap);
	//生成半全场json
	string	CreateHalfFullJson(StringMap &kvmap);

private:
	//从链表搜索gid相同的数据
	MatchBall	*FindMatchBall(MatchBallList &lst, const string &gid);
	//更新到数据库
	void		UpdateToSql(const MatchBallList &lst);
	//释放内存
	void		FreeMatchBall(MatchBallList &lst);
};

typedef void (CFootBall::*PParse)(const BallData &data, MatchBallList &lst);

