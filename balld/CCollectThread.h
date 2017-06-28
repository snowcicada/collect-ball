#pragma once

#include <stdhdr.h>
#include <CThread.h>

class CCollectThread : public CThread
{
public:
	CCollectThread();
	~CCollectThread();
	//滚球 今日赛事 早盘
	void	SetCltTimeType(TimeType type);
	//设置采集周期
	void	SetCltPeriod(time_t sec);

protected:
	virtual void Run();

private:
	//线程核心处理
	void	RunCore();
	//处理系统维护状态
	bool	DealSysMaintain(const string &url);
	//处理登陆
	bool	DealLogin(UserInfo *pui, const string &url);
    //是否在维护
    bool    IsMaintain(const string &url);
	//登陆到网站
	bool	Login(const string &url, const string &user, const string &passwd, string &html);
	//解析登陆成功后的页面,得到相应的网址和uid
	bool	ParseHtml(const string &html, UserInfo *pui);
	//根据各种类型生成特定的网址
	string 	CreateUrl(const string &baseUrl, const string &uid, const string &lang, const string &mtype, BallType ballType, BetType betType, TimeType timeType, int page);
	//采集数据
	bool	CollectAllBallData(UserInfo *pui);
	//循环采集每种类型的数据
	bool	CollectBallData(UserInfo *pui, BallType ballType, BetType betType, TimeType timeType, StringList &lst);
	//从指定网址采集数据
	bool	CollectData(const string &url, StringList &lst, int *page=NULL);
	//写入数据库
	bool	WriteToSql(BallType ballType, BetType betType, TimeType timeType, const string &data);
	bool	WriteToSql(BallType ballType, BetType betType, TimeType timeType, const StringList &data);
	//连接表单域
	void	LinkFieldMap(StringMap &kvMap, string &str);
	//获取网页的错误信息
	int		GetHtmlError(const string &html);
	//删除字符串中网页的元素:<br> <font xx>xxx</font>
	void	RemoveSub(string &str);

private:
	TimeType	m_iCltTimeType;
	time_t		m_iCltPeriod;
};

