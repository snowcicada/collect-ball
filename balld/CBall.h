#pragma once

#include <stdhdr.h>

class CBall
{
public:
	CBall();
	CBall(uint type);
	virtual ~CBall();
	//采集
	virtual bool Collect();
	//设置模式:滚球 今日 早盘
	void	SetTimeType(uint type);
	//生成网址
	string	CreateUrl(const string &baseUrl, const string &uid, const string &mtype,
						const string &datetime, const string &gid, BallType ballType,
						BetType betType, int page=0);
	//下载页面
	bool	DownloadPage(const string &url, string &html);
	//下载所有页面
	bool	DownloadBallAllPage(BallType ballType, BetType betType, BallData &ballData);
	//下载冠军
	bool	DownloadBallChampion(BallType ballType, MatchChampionMap &mcMap);
	//下载赛果
	bool	DownloadBallResult(BallType ballType, BetType betType, const string &datetime, string &html);
	//下载赛果更多页面
	bool	DownloadBallResultMore(BallType ballType, const string &gid, string &html);
	//匹配Array内容到BallData
	void	ParseArray(const StringList &htmlList, BallData &ballData);
	void	ParseArray(const string &html, BallData &ballData);
	//匹配冠军结果
	void	ParseChampion(const string &html, MatchChampionMap &mcMap);
	//匹配赛果
	void	ParseResult(const string &html);
	//获取登陆uid
	bool	GetLoginUid(string &uid, string &baseUrl, string &mtype);
	//设置uid无效
	void	SetUidInvalid(const string &uid);
	//打印冠军结果
	void	PrintChampion(const MatchChampionMap &mcMap);
	//释放冠军结果的空间
	void	FreeChampion(MatchChampionMap &mcMap);
	//转换时间为24小时
	string	ConvertTimeTo24(const string &strTime);
	//转换时间为时间戳
	time_t	ConvertTimeToSec(const string &strDate, const string &strTime);
	//获取美国时间的前一天, bToday:true今天 false昨天
	string	GetUsaDate(bool bToday);
	//释放空间
	void	FreeBallData(BallData &ballData);

protected:
	uint	m_iTimeType;

private:
	//获取网页错误
	int		GetHtmlError(const string &html);
};

