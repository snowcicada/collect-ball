#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
//#include <uuid/uuid.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/resource.h>
#include <syslog.h>
#include <mysql/mysql.h>

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <CCondition.h>
#include <CMutex.h>
#include <CFlagMutex.h>
#include <CAes.h>
#include <CMd5.h>
#include <CTimeout.h>
#include <CIniFile.h>
#include <json/json.h>
#include <pcre++.h>

using namespace std;

class CJob;
class CThread;
class CWorkerThread;
class CCurl;
class CTcpServer;
class CTcpSocket;
class CTimeout;

typedef map<string, string>     StringMap;
typedef map<string, float>		StrFloatmap;
typedef vector<string>          StringVector;
typedef list<string>            StringList;

//命令操作类型
typedef enum
{
	CMD_NONE,	//无
	CMD_START,	//开始采集
	CMD_STOP,	//停止采集
	CMD_QUIT,	//退出程序
	CMD_HEART,	//心跳测试
	CMD_ADDUSER,//添加用户信息
	CMD_ADDURL,	//添加网址
	CMD_SET,	//设置信息
	CMD_VERSION	//获取版本
}CmdType;

//球类型 6
typedef enum
{
	BALL_FT,	//足球 football
	BALL_BK,	//篮球 美式足球 橄榄球 basketball
	BALL_TN,	//网球 tennis ball
	BALL_VB,	//排球 羽毛球 乒乓球 volleyball
	BALL_BS,	//棒球 baseball
	BALL_OP		//其他
}BallType;

//投注类型
typedef enum
{
	BET_R,			//独赢 ＆ 让球 ＆ 大小 & 单 / 双
	BET_PD,			//波胆
	BET_T,			//总入球
	BET_F,			//半场 / 全场
	BET_ALL,		//让分 ＆ 大小 ＆ 单 / 双
	BET_PR,			//篮球/美式足球/橄榄球/网球/排球/羽毛球/乒乓球/棒球的综合过关
	BET_P3,			//足球/其他的综合过关
	BET_FS,			//冠军

	//以下为赛果类型
	BET_RES,		//普通赛果
	BET_FSRES,		//冠军赛果
	BET_SFSRES,		//特殊冠军赛果
	BET_RESSP,		//普通赛果更多选项

	//以下为滚球的投注类型
	BET_RE,			//独赢 ＆ 让球 ＆ 大小 & 单 / 双
	BET_RPD,		//波胆
	BET_RT,			//总入球
	BET_RF			//半场 / 全场
}BetType;

//BET_P3,		//足球/其他的综合过关
//BET_PR,		//篮球/美式足球/橄榄球/网球/排球/羽毛球/乒乓球/棒球的综合过关

//时间类型 2
typedef enum
{
	TIME_TODAY,		//browse 今日
	TIME_CURRENT,	//browse 滚球
	TIME_FUTURE,	//future 明天 早盘
	TIME_NONE
}TimeType;

//全局语言类型宏
#define UID_LANG	"zh-cn" //zh-cn zh-tw en-us

/**球玩法信息[**/
typedef list<StringVector *> VecList;
typedef struct
{
	StringVector	FieldVec;//字段
	VecList			DataVecList;//数据
}BallData;
/**球玩法信息]**/

/**冠军信息[**/
//玩法的结果
typedef struct
{
//	string	FsNum;//情况 FS01
	string	Res;//结果
	string	Odds;//赔率
}MatchRes;
typedef map<string, MatchRes *> MatchResMap;

//赛事里面的玩法
typedef struct
{
	string		Gid;//组id
	string 		ModeName;//玩法名称
	string 		MatchDateTime;//开赛日期
	MatchResMap	ResMap;//结果集合
}MatchMode;
typedef list<MatchMode *>	MatchModeList;

//赛事
typedef struct
{
//	string 			MatchName;//赛事名称
	string 			MatchDateTime;//赛事开始日期
	MatchModeList	modeList;//玩法链表
}MatchChampion;
typedef map<string, MatchChampion *> MatchChampionMap;
/**冠军信息]**/

//网站的错误信息宏
#define ERR_OK		 	0	//成功
#define ERR_UPWRONG		1	//用户名或密码不正确
#define ERR_EDITPASSWD	2	//提示修改密码
#define ERR_USERBAN		3	//用户被停用
#define ERR_LOGOUT		4	//用户被登出,uid过期
#define ERR_NOTFOUND	5	//404,说明该网站正在维护
#define ERR_SYSTEMERROR	6	//System error
#define ERR_UNKNOWN		10	//未知错误

//采集周期
#define CLT_TODAY_TO	60	//今日赛事
#define CLT_CURRENT_TO	20	//滚球
#define CLT_FUTURE_TO	60	//早盘

//用户信息
typedef struct
{
    string  user;       //用户名
    string  passwd;     //密码
    string  url;        //二级域名网址
    string  uid;        //登陆的唯一id
    string  langx;      //语言类型
    string  mtype;      //采集数据的网址会使用到
    string  today_gmt;  //登陆日期
    string  chghasLogin;//操作,暂时不用
    int		userState;	//正常 禁用 密码错误
    bool    reLogin;    //是否重新登陆
    CMutex	mutex;		//锁
}UserInfo;
typedef map<string, UserInfo*>  UserInfoMap;

//数据库
typedef struct
{
	string	ipAddr;
	string	user;
	string	passwd;
	string	dbname;
	uint	port;
}SqlInfo;

//epoll数据
typedef struct
{
    int         fd;     //描述符
    string      strRecv;//接收的数据
    string      strSend;//发送的数据
    CTimeout    timeout;//超时类
}EpollData;

typedef map<int, EpollData *>   EpollFdMap;//保存条件到epoll事件到fd和ptr

#define SOCKET_PORT         6000   //端口号
#define MAX_RECVSIZE        512000  //最大接收数据大小
#define SOCKET_TIMEOUT      20      //超时时间:20s
#define TCP_PACK_SIZE       1460    //tcp包最大包长
#define CURL_GET_TIMEOUT    20L     //curl get超时时间
#define CURL_POST_TIMEOUT   20L      //curl post超时时间
#define COOKIE_DIR          "/var/webhelper/cookies/"

#define MAX_EVENTS          1024        //最大接收到事件数
#define MAX_LISTEN          SOMAXCONN   //最大同时连接数
#define MSG_FINISHED        '#'			//一个TCP包到结束标志

#define MSG_QUIT            "_app_quit_"    //程序退出标志
#define MSG_RUN             "_app_run_"     //正在运行

//版本
#define APP_VERSION         "balld-v1.0-2013-08-27"

#define DEBUG_PRINT         printf("file:%s function:%s line:%d\n", __FILE__, __FUNCTION__, __LINE__);
#define DEBUG_DELETEPRINT   //printf("delete %s\n", __FUNCTION__);

//结构体
#include <ballstruct.h>

/*
控制协议:
balld命令通道控制协议:
命令格式:
开始采集:
{
	cmd : start
	data: 空
}

停止采集:
{
	cmd : stop
	data: 空
}

退出程序:
{
	cmd : quit
	data: 空
}

心跳测试:
{
	cmd : heart
	data: start/stop
}
若没有响应，说明服务端没有开启

添加N个用户信息:
{
	cmd : adduser
	data:
	{
		user_passwd_1 : 用户名|密码
		user_passwd_2 : 用户名|密码
		user_passwd_3 : 用户名|密码
		...
		user_passwd_n : 用户名|密码
	}
}

添加N个网址:
{
	cmd : addurl
	data:
	{
		url_1 : 网址1
		url_2 : 网址2
		url_3 : 网址3
		...
		url_n : 网址n
	}
}

设置信息:
{
	cmd : set
	data:
	{
		sqladdr		: 数据库地址
		sqluser		: 数据库用户名
		sqlpasswd	: 数据库密码
	}
}

//获取版本信息:
{
	cmd : version
	data: 空
}

命令响应:
2种情况:
1.
客户端发送过来的数据错误:
dataerror

2.命令的处理结果
{
	cmd : 命令
	data:
	{
		res : yes/no/信息
	}
}


通信数据均采用AES加密方式：
AES#
*/
