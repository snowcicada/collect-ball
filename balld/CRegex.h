#pragma once
#include <stdhdr.h>

class CRegex
{
public:
    static bool Match(const string &stuff, const string &pattern, StringVector &resVec);
    static bool Match2(const string &stuff, const string &pattern, StringVector &resVec);
    static bool Match(const string &stuff, const string &pattern, string &result);
    static bool MatchContext(const string &stuff, const string &pattern, StringVector &resVec);
    static bool GetHiddenLine(const string &stuff, const string &pattern, StringVector &resVec);
    static bool GetHiddenKeyValue(const string &html, StringMap &kvMap);
    static bool GetUrl(const string &html, string &url);
    static bool GetError(const string &html, string &err);
    static bool GetArray(const string &html, StringList &lst);
    static bool GetField(const string &str, StringVector &vec);
    static bool GetPage(const string &html, int &page);
    static bool IsMatch(const string &stuff, const string &pattern);
    static bool GetDateTime(const string &stuff, string &date, string &time, bool *bRb=NULL);

    /*****足球赛果*****/
    //匹配一个赛事信息块
    static bool GetFtResMatch(const string &html, StringVector &strVec);
    //匹配赛果里面存在更多选项的id
    static bool GetFtResMatchIdMore(const string &html, StringVector &strVec);
    //匹配更多里面的内容
    static bool GetFtResMoreScore(const string &html, StringVector &strVec);
    //匹配赛事id
    static bool GetFtResMatchId(const string &src, StringVector &strVec);
    //匹配赛事标题
    static bool GetFtResMatchTitle(const string &src, string &title);
    //匹配比赛时间
    static bool GetFtResTime(const string &src, StringVector &strVec);
    //匹配赛事队伍
    static bool GetFtResTeam(const string &src, StringVector &strVec);
    //匹配比赛结果
    static bool GetFtResScore(const string &src, StringVector &strVec);

    /*****篮球赛果*****/
    //匹配一个赛事信息块
	static bool GetBkResMatch(const string &html, StringVector &strVec);
	//匹配赛事id
	static bool GetBkResMatchId(const string &src, StringVector &strVec);
	//匹配赛事标题
	static bool GetBkResMatchTitle(const string &src, string &title);
	//匹配比赛时间
	static bool GetBkResTime(const string &src, StringVector &strVec);
	//匹配赛事队伍
	static bool GetBkResTeam(const string &src, StringVector &strVec);
	//匹配比赛结果
    static bool GetBkResScore(const string &src, StringVector &strVec);
};
