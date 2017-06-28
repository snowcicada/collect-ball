#pragma once

/**数据库对应结构体[**/
typedef struct
{
	string	strMatchId;//运动项目ID	gid
	string	strMatchHalfId;//运动项目半场ID	hgid
	string	strMatchName;//运动项目名称	league
	int		iMasterId;//主队球队ID	gnum_h
	string	strMasterName;//主队球队名称	team_h
	int		iGuestId;//客队ID	gnum_c
	string	strGuestName;//客队名称	team_c
	string	strMatchDate;//开赛日期	datetime日期
	string	strMatchTime;//开赛时间	datetime时间 24小时
	long	iCoverDate;//比赛时间	转换time_t
	int		iMatchCurTime;//当前比赛时间	滚球-timer上下半场时间
	string	strScore;//结果比分 json 累加
	int		iMatchType;//比赛类型	datetime日期字段后面内容 <font color=red>Running Ball</font> 代表有滚球
	string	strDy;//独赢赔率 json 更新
	string	strRqnum;//让球数 json 更新
	string	strRq;//让球赔率 json 更新
	string	strDxnum;//大小数 json 更新
	string	strDx;//大小赔率 json 更新
	string	strDan;//单赔率	更新
	string	strShuang;//双赔率	更新
	string	strRlj;//红黄牌角球等	json 获取结果的时候更新数据库 json存储所有数据
	string	strBodan;//波胆 json 更新
	string	strZrq;//总入球 json 更新
	string	strBqc;//半场/全场 json 更新
	string	strMatchShowType;//全场strong
	string	strMatchHrShowType;//半场strong
	int		iMasterRedcard;//主队红牌
	int		iGuestRedcard;//客队红牌

	string	strRpNum;//让盘数
	string	strRp;//让盘赔率
	string	strRfNum;//让分数
	string	strRf;//让分赔率
	string	strSp;//赛盘投注
}MatchBall;
typedef list<MatchBall *>	MatchBallList;
/**数据库对应结构体]**/

/**赛果[**/
/*
通用结构体:
足球FT:半场全场4个比分  更多有16个比分(最先进球 最后进球 最先越位 最后越位 最先替补球员 最后替补球员 有失球 没有失球 第一颗角球 最后一颗角球 第一张卡 最后一张卡 第一张黄卡 最后一张黄卡 会进球 不会进球)
篮球BK:16个比分(第1节 第2节 第3节 第4节 上半 下半 加时 全场)
网球TN:14个比分({GAME_TN_1} {GAME_TN_2} {GAME_TN_3} {GAME_TN_4} {GAME_TN_5} {GAME_TN_6} 完赛(局))
排球VB:6个比分({GAME_VB_1} {GAME_VB_2} 完赛(局))
棒球BS:半场全场4个比分
其他OP:半场全场4个比分
*/
typedef struct
{
	BallType iBallType;//FT BK TN VB BS OP
	string	strMatchId;
	string	strMatchTitle;//赛事名称/标题
	string	strDate;//日期
	string	strTime;//时间
	string	strTeam[2];//主客队伍名称
	string	strScore[4];//半全场比分
	string	strMoreScore[16];//不是半全场比分都保存在这里
	time_t	iTime;//时间戳
	bool	bMore;//是否存在更多比分,只用于足球
}MatchResult;
typedef list<MatchResult *> MatchResultList;
/**赛果]**/

