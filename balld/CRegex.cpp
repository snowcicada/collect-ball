#include <CRegex.h>
#include <CPublic.h>

bool CRegex::Match(const string &stuff, const string &pattern, StringVector &resVec)
{
    size_t pos;
	size_t length;
	pcrepp::Pcre reg(pattern, PCRE_CASELESS);

    resVec.clear();
	pos = 0;
	length = stuff.length();
	while (pos < length)
	{
		if (!reg.search(stuff, pos))
		{
		    break;
		}
        pos = reg.get_match_end();
        resVec.push_back(reg[0]);
	}

    return !resVec.empty();
}

bool CRegex::Match2(const string &stuff, const string &pattern, StringVector &resVec)
{
	size_t pos;
	size_t length;
	pcrepp::Pcre reg(pattern, PCRE_CASELESS);

    resVec.clear();
	pos = 0;
	length = stuff.length();
	while (pos < length)
	{
		if (!reg.search(stuff, pos))
		{
		    break;
		}
        pos = reg.get_match_end();
        resVec.push_back(reg[0]);
        resVec.push_back(reg[1]);
	}

    return !resVec.empty();
}

bool CRegex::Match(const string &stuff, const string &pattern, string &result)
{
    pcrepp::Pcre reg(pattern, PCRE_CASELESS);

	if (!reg.search(stuff))
	{
	    return false;
	}
    result = reg[0];

    return true;
}

bool CRegex::MatchContext(const string &stuff, const string &pattern, StringVector &resVec)
{
    size_t pos, len;
	int epos, spos;
	pcrepp::Pcre reg(pattern, PCRE_CASELESS);

    resVec.clear();
	pos = 0;
	len = stuff.length();
	while (pos < len)
	{
		if (!reg.search(stuff, pos))
		{
		    break;
		}
        pos = reg.get_match_end();
        spos = reg.get_match_start();
        epos = reg.get_match_end();
        resVec.push_back(stuff.substr(spos, epos - spos + 1));
	}
	return !resVec.empty();
}

bool CRegex::GetHiddenLine(const string &stuff, const string &pattern, StringVector &resVec)
{
    size_t pos;
	size_t length;
	int epos, spos;
	pcrepp::Pcre reg(pattern, PCRE_CASELESS);

	pos = 0;
	length = stuff.length();
	while (pos < length)
	{
		if (reg.search(stuff, pos))
		{
			pos = reg.get_match_end();
			spos = reg.get_match_start();
			epos = reg.get_match_end();
			resVec.push_back(stuff.substr(spos, epos - spos + 1));
		}
		else
		{
			break;
		}
	}

    return !resVec.empty();
}

bool CRegex::GetHiddenKeyValue(const string &html, StringMap &kvMap)
{
	size_t pos;
	size_t length;
	/*
	正则分成几部分:
	<input[\s\S]+?
	name[\s\S]+?
	'([^']*?)'
	[\s\S]+?
	value[\s\S]
	'([^']*?)'>
	*/
	pcrepp::Pcre reg("<input[\\s\\S]*?name[\\s\\S]*?'([^']*?)'[\\s\\S]*?value[\\s\\S]'([^']*?)'>", PCRE_CASELESS);//case insensitive search

	kvMap.clear();
	pos = 0;
	length = html.length();
	while (pos < length)
	{
		if (!reg.search(html, pos))
		{
		    break;
		}
        pos = reg.get_match_end();
        if (reg.matches() >= 2)
		{
			kvMap[reg[0]] = reg[1];
		}
	}

    return !kvMap.empty();
}

bool CRegex::GetUrl(const string &html, string &url)
{
	/*
	分成几部分:
	<form[\s\S]+?
	action[^']+?
	'([^']*?)'
	*/
	return Match(html, "<form[\\s\\S]*?action[^']*?'([^']*?)'", url);
}

bool CRegex::GetError(const string &html, string &err)
{
	return Match(html, "alert[\\s\\S]*?'([^']*?)'", err);
}

bool CRegex::GetArray(const string &html, StringList &lst)
{
    size_t pos;
	size_t length;
	//Array\\(([\\s\\S]*?)\\);  前面可能会匹配到空
	//Array([^;]*?);
	//Array([^;]{10,});  遇到;号会异常
	//Array()不匹配
	pcrepp::Pcre reg("Array\\(([\\s\\S]*?)\\);");

    lst.clear();
	pos = 0;
	length = html.length();
	while (pos < length)
	{
		if (!reg.search(html, pos))
		{
		    break;
		}
        pos = reg.get_match_end() + 2;
        if (reg.matches() != 0 && !reg[0].empty())
        {
            lst.push_back(reg[0]);
        }
	}

    return !lst.empty();
}

bool CRegex::GetField(const string &str, StringVector &vec)
{
	size_t pos;
	size_t length;
	pcrepp::Pcre reg("'([^']*?)'", PCRE_CASELESS);//case insensitive search

    vec.clear();
	pos = 0;
	length = str.length();
	while (pos < length)
	{
		if (!reg.search(str, pos))
		{
		    break;
		}
        pos = reg.get_match_end()+1;
        vec.push_back(reg[0]);
	}

	return !vec.empty();
//	return Match(str, "'([^']*?)',", vec);
}

bool CRegex::GetPage(const string &html, int &page)
{
	string strRes;

	page = 0;
	if (!Match(html, "parent.t_page=(\\d+);", strRes))
	{
		return false;
	}

	page = atoi(strRes.c_str());

	return true;
}

bool CRegex::IsMatch(const string &stuff, const string &pattern)
{
	pcrepp::Pcre reg(pattern, PCRE_CASELESS);//case insensitive search
	return reg.search(stuff, 0);
}

bool CRegex::GetDateTime(const string &stuff, string &date, string &time, bool *bRb)
{
	pcrepp::Pcre reg;
	string strPattern;
	bool isRunBall;

	date.clear();
	time.clear();

	isRunBall = IsMatch(stuff, ".*Running.*");

	//设置滚球变量
	if (bRb)
	{
		*bRb = isRunBall;
	}

	if (isRunBall)
	{
		strPattern = "(.*)<br>(.*)<br>";
	}
	else
	{
		strPattern = "(.*)<br>(.*)";
	}

	reg = strPattern;
	if (!reg.search(stuff))
	{
		return false;
	}
	if (reg.matches() != 2)
	{
		return false;
	}
	date = reg[0];
	time = reg[1];
	return true;
}

/*****足球赛果*****/
//匹配一个赛事信息块
bool CRegex::GetFtResMatch(const string &html, StringVector &strVec)
{
	//添加一个<!--table>,为了匹配最后一段出来
	string strStuff = html + "<!--table>";
	return Match(strStuff, "<!----------------------->([\\s\\S]*?)<!--table>", strVec);
}

//匹配赛果里面存在更多选项的id
bool CRegex::GetFtResMatchIdMore(const string &html, StringVector &strVec)
{
	return Match(html, "TR_\\d+_(\\d+)\"[\\s\\S]*?more_btn", strVec);
}

//匹配更多里面的内容
bool CRegex::GetFtResMoreScore(const string &html, StringVector &strVec)
{
	return Match(html, "GameRtype.*?Array\\(.*?,'(.*?)'\\);", strVec);
}

//匹配赛事id
bool CRegex::GetFtResMatchId(const string &src, StringVector &strVec)
{
	return Match(src, "TR_\\d+_(\\d+)\"", strVec);
}

//匹配赛事标题
bool CRegex::GetFtResMatchTitle(const string &src, string &title)
{
	return Match(src, "leg_bar\">(.*?)</span>", title);
}

//匹配比赛时间
bool CRegex::GetFtResTime(const string &src, StringVector &strVec)
{
	return Match2(src, "time\">(.*?)<br>(.*?)</td>", strVec);
}

//匹配赛事队伍
bool CRegex::GetFtResTeam(const string &src, StringVector &strVec)
{
	return Match2(src, "team_c_ft\">([^&]*?)&[\\s\\S]*?team_h_ft\">([^&]*?)&.*?</td>", strVec);
}

//匹配比赛结果
bool CRegex::GetFtResScore(const string &src, StringVector &strVec)
{
	size_t pos;
	size_t length;
	string strPattern = "TR_1[\\s\\S]*?hr_main_ft.*?<span.*?>(.*?)<[\\s\\S]*?hr_main_ft.*?"
						"<span.*?>(.*?)<[\\s\\S]*?TR_2[\\s\\S]*?full_main_ft.*?<span.*?>(.*?)"
						"<[\\s\\S]*?full_main_ft.*?<span.*?>(.*?)<";
	pcrepp::Pcre reg(strPattern, PCRE_CASELESS);

    strVec.clear();
	pos = 0;
	length = src.length();
	while (pos < length)
	{
		if (!reg.search(src, pos))
		{
		    break;
		}
        pos = reg.get_match_end();
        if (reg.matches() == 4)
		{
			for (int i = 0; i < 4; i++)
			{
				strVec.push_back(reg[i]);
			}
		}
	}

	//必须是4的倍数
	return (strVec.size()%4 == 0);
}

/*****篮球赛果*****/
//匹配一个赛事信息块
bool CRegex::GetBkResMatch(const string &html, StringVector &strVec)
{
	return Match(html, "<!--table>([\\s\\S]*?)<!--/table-->", strVec);
}

//匹配赛事id
bool CRegex::GetBkResMatchId(const string &src, StringVector &strVec)
{
	return Match(src, "TR_\\d+_(\\d+)\"", strVec);
}

//匹配赛事标题
bool CRegex::GetBkResMatchTitle(const string &src, string &title)
{
	return Match(src, "leg_bar\">(.*?)</span>", title);
}

//匹配比赛时间
bool CRegex::GetBkResTime(const string &src, StringVector &strVec)
{
	return Match2(src, "time\">(.*?)<br>(.*?)</td>", strVec);
}

//匹配赛事队伍
bool CRegex::GetBkResTeam(const string &src, StringVector &strVec)
{
	return Match2(src, "team_c\">([^&]*?)&[\\s\\S]*?team_h\">([^&]*?)&.*?</td>", strVec);
}

//匹配比赛结果
bool CRegex::GetBkResScore(const string &src, StringVector &strVec)
{
	size_t pos;
	size_t length;
	string strPattern = "TR_1[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?TR_2"
						"[\\s\\S]*?[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?"
						"TR_3[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?TR_4"
						"[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?TR_5[\\s\\S]*?"
						"hr_main.*?>(.*?)<[\\s\\S]*?hr_main.*?>(.*?)<[\\s\\S]*?TR_6[\\s\\S]*?"
						"hr_main.*?>(.*?)<[\\s\\S]*?hr_main.*?>(.*?)<[\\s\\S]*?TR_7[\\s\\S]*?"
						"<strong>(.*?)<[\\s\\S]*?<strong>(.*?)<[\\s\\S]*?TR_8[\\s\\S]*?full_main.*?"
						">(.*?)<[\\s\\S]*?full_main.*?>(.*?)<";
	pcrepp::Pcre reg(strPattern, PCRE_CASELESS);

    strVec.clear();
	pos = 0;
	length = src.length();
	while (pos < length)
	{
		if (!reg.search(src, pos))
		{
		    break;
		}
        pos = reg.get_match_end();
        if (reg.matches() == 16)
		{
			for (int i = 0; i < 16; i++)
			{
				strVec.push_back(reg[i]);
			}
		}
	}

	//必须是16的倍数
	return (strVec.size()%16 == 0);
}


