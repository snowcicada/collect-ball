#pragma once
/*
该头文件只提供给采集类使用,不能重复包含
*/

//各种球类型
static const char *g_pBallTypeArray[] =
{
	"FT", "BK", "TN", "VB", "BS", "OP"
};

//各种投注类型
static const char *g_pBetTypeArray[] =
{
	//res暂时用不到
	"r", "pd", "t", "f", "all", "pr", "p3", "fs", "result", "result_fs", "result_sfs", "result_sp", "re", "rpd", "rt", "rf"
};

//滚球 今日 早盘
static const char *g_pTimeTypeArray[] =
{
	"browse", "browse", "future"
};

//语言
//static const char *g_pLangTypeArray[] =
//{
//	"zh-cn", "zh-tw", "en-us"
//};

//log
//static const char *g_pBallArrZh[] =
//{
//	"足球", "篮球", "网球", "排球", "棒球", "其他"
//};
//
//static const char *g_pBetArrZh[] =
//{
//	"独赢&让球&让分&让盘&大小&单双", "波胆&赛盘投注", "总入球", "半场/全场", "让分", "冠军", "综合过关", "综合过关", "让分"
//};
//
//static const char *g_pTimeArrZh[] =
//{
//	"今日赛事", "滚球", "早盘"
//};

//static const char *g_pLangArrZh[] =
//{
//	"中文简体", "中文繁体", "英文"
//};

//今日赛事
//static int g_pTodayArray[][3] =
//{
//	//1.足球
//	{BALL_FT, BET_R,	TIME_TODAY},
//	{BALL_FT, BET_PD, 	TIME_TODAY},
//	{BALL_FT, BET_T, 	TIME_TODAY},
//	{BALL_FT, BET_F, 	TIME_TODAY},
//	{BALL_FT, BET_P3, 	TIME_TODAY},
//	{BALL_FT, BET_FS, 	TIME_TODAY},
//	//2.篮球/美式足球/橄榄球
//	{BALL_BK, BET_ALL,	TIME_TODAY},
//	{BALL_BK, BET_PR,	TIME_TODAY},
//	{BALL_BK, BET_FS,	TIME_TODAY},
//	//3.网球
//	{BALL_TN, BET_R,	TIME_TODAY},
//	{BALL_TN, BET_PD,	TIME_TODAY},
//	{BALL_TN, BET_PR,	TIME_TODAY},
//	{BALL_TN, BET_FS,	TIME_TODAY},
//	//4.排球/羽毛球/乒乓球
//	{BALL_VB, BET_R,	TIME_TODAY},
//	{BALL_VB, BET_PD,	TIME_TODAY},
//	{BALL_VB, BET_PR,	TIME_TODAY},
//	{BALL_VB, BET_FS,	TIME_TODAY},
//	//5.棒球
//	{BALL_BS, BET_R,	TIME_TODAY},
//	{BALL_BS, BET_PR,	TIME_TODAY},
//	{BALL_BS, BET_FS,	TIME_TODAY},
//	//6.其他
//	{BALL_OP, BET_R,	TIME_TODAY},
//	{BALL_OP, BET_P3,	TIME_TODAY},
//	{BALL_OP, BET_FS,	TIME_TODAY}
//};
//
////早盘
//static int g_pFutureArray[][3] =
//{
//	//1.足球
//	{BALL_FT, BET_R,	TIME_FUTURE},
//	{BALL_FT, BET_PD, 	TIME_FUTURE},
//	{BALL_FT, BET_T, 	TIME_FUTURE},
//	{BALL_FT, BET_F, 	TIME_FUTURE},
//	{BALL_FT, BET_P3, 	TIME_FUTURE},
//	{BALL_FT, BET_FS, 	TIME_FUTURE},
//	//2.篮球/美式足球/橄榄球
//	{BALL_BK, BET_ALL,	TIME_FUTURE},
//	{BALL_BK, BET_PR,	TIME_FUTURE},
//	{BALL_BK, BET_FS,	TIME_FUTURE},
//	//3.网球
//	{BALL_TN, BET_R,	TIME_FUTURE},
//	{BALL_TN, BET_PD,	TIME_FUTURE},
//	{BALL_TN, BET_PR,	TIME_FUTURE},
//	{BALL_TN, BET_FS,	TIME_FUTURE},
//	//4.排球/羽毛球/乒乓球
//	{BALL_VB, BET_R,	TIME_FUTURE},
//	{BALL_VB, BET_PD,	TIME_FUTURE},
//	{BALL_VB, BET_PR,	TIME_FUTURE},
//	{BALL_VB, BET_FS,	TIME_FUTURE},
//	//5.棒球
//	{BALL_BS, BET_R,	TIME_FUTURE},
//	{BALL_BS, BET_PR,	TIME_FUTURE},
//	{BALL_BS, BET_FS,	TIME_FUTURE},
//	//6.其他
//	{BALL_OP, BET_R,	TIME_FUTURE},
//	{BALL_OP, BET_P3,	TIME_FUTURE},
//	{BALL_OP, BET_FS,	TIME_FUTURE}
//};
//
////滚球
//static int g_pCurrentArray[][3] =
//{
//	//1.足球
//	{BALL_FT, BET_RE,	TIME_CURRENT},
//	{BALL_FT, BET_RPD,	TIME_CURRENT},
//	{BALL_FT, BET_RT,	TIME_CURRENT},
//	{BALL_FT, BET_RF,	TIME_CURRENT},
//	//2.篮球/美式足球/橄榄球
//	{BALL_BK, BET_RE,	TIME_CURRENT},
//	//3.网球
//	{BALL_TN, BET_RE,	TIME_CURRENT},
//	//4.排球/羽毛球/乒乓球
//	{BALL_VB, BET_RE,	TIME_CURRENT},
//	//5.棒球
//	{BALL_BS, BET_RE,	TIME_CURRENT},
//	//6.其他
//	{BALL_OP, BET_RE,	TIME_CURRENT}
//};
//

