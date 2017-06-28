/************************************************************

*************************************************************/

#include <CSettings.h>
#include <CJson.h>
#include <CTcpServer.h>
#include <CPublic.h>

CTcpServer::CTcpServer()
{
    m_iSock			= 0;
    m_iPort			= 0;
    m_iEpollFd		= 0;
    m_iMaxEvents	= 0;
    m_bRunning		= true;//重要标志

    //创建线程
    if (!m_CltTodayThread.Create() || !m_CltCurrentThread.Create() || !m_CltFutureThread.Create())
	{
		printf("数据采集线程打开失败!\n");
		exit(1);
	}
}

CTcpServer::~CTcpServer()
{
    UnregAllEpollFd();
}

bool CTcpServer::Listen(unsigned short port)
{
    if (!Socket(port))
    {
        return false;
    }
    if (!Bind())
    {
        return false;
    }
    if (-1 == listen(m_iSock, MAX_LISTEN))
    {
        return false;
    }

    //非阻塞
    if (!SetNonBlocking(m_iSock))
    {
        return false;
    }

    return true;
}

/*
此处的epoll使用的是ET模式(边缘模式)
ET模式只会通知一次
LT模式会一直通知，若没有处理epoll通知的事件，每次执行epoll_wait都会重新通知事件，解决办法可以移除epoll
*/
bool CTcpServer::EpollLoop()
{
    if (!m_iSock)
    {
        return false;
    }

    int                  nfds;
    struct epoll_event  *pEv;
    EpollData           *pData;

    if (!EpollCreate(MAX_EVENTS))
    {
        return false;
    }

    if (!RegEpollFd(m_iSock, EPOLLIN | EPOLLET))
    {
        return false;
    }

    while (m_bRunning)
    {
        if (!EpollWait(2000, nfds))
        {
            continue;
        }
        for (int i = 0; i < nfds; i++)
        {
            //判断是否退出
            if (!m_bRunning)
			{
				break;
			}
            pEv = EpollEvent(i);
            pData = (EpollData *)(pEv->data.ptr);
            pData->timeout.UpdateLastTime();//更新访问时间

            //listen new connect
            if (pData->fd == m_iSock)
            {
                if (!EtHandleAccept())
                {
                    continue;
                }
            }//readable
            else if (pEv->events & EPOLLIN)
            {
                EtHandleRecv(pData);
            }//writable
            else if (pEv->events & EPOLLOUT)
            {
                EtHandleSend(pData);
            }
        }

        //处理事件后,清理超时连接
        CleanTimeoutFd();
    }

    return true;
}

bool CTcpServer::SetNonBlocking(int fd)
{
    int opts = fcntl(fd, F_GETFL);
    if (-1 == opts)
    {
        return false;
    }

    opts |= O_NONBLOCK;
    if (-1 == fcntl(fd, F_SETFL, opts))
    {
        return false;
    }

    return true;
}

void CTcpServer::Exit()
{
	m_bRunning = false;
}

bool CTcpServer::Socket(unsigned short port)
{
    int optval;
    struct timeval timeout;

    m_iSock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_iSock)
    {
        m_iSock = 0;
        return false;
    }

    optval = 1;
    if (-1 == setsockopt(m_iSock, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval)))
    {
        goto sock_err;
    }

    //设置接收和发送超时时间为5秒
    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;
    if (-1 == setsockopt(m_iSock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)))
    {
        goto sock_err;
    }

    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;
    if (-1 == setsockopt(m_iSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
    {
        goto sock_err;
    }

    m_iPort = port;

    return true;

sock_err:
    if (m_iSock)
    {
        close(m_iSock);
        m_iSock = 0;
    }
    return false;
}

bool CTcpServer::Bind()
{
    memset(&m_SockAddr, 0, sizeof(struct sockaddr_in));
    m_SockAddr.sin_family      = AF_INET;
    m_SockAddr.sin_port        = htons(m_iPort);
    m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (-1 == bind(m_iSock, (struct sockaddr *)&m_SockAddr, sizeof(struct sockaddr_in)))
    {
        close(m_iSock);
        m_iSock = 0;
        return false;
    }

    return true;
}

void CTcpServer::DealRecvData(EpollData *pData)
{
	CmdType type;
	string data;
	static bool bStartFirst = true;
	static bool bStopFirst = false;
	CSettings *pSet = CSettings::Instance();

    //命令类型
    if (!GetCmdType(pData->strRecv, data, type))
	{
		//失败
		pData->strSend = "dataerror";
		EpollCtlMOD(EPOLLOUT | EPOLLET, pData);
		return;
	}

	//处理命令
	switch (type)
	{
	case CMD_START:
//		if (pSet->IsValidParam())
//		{
			if (bStartFirst)
			{
				bStartFirst = false;
				bStopFirst = true;

//				m_CltTodayThread.SetCltTimeType(TIME_TODAY);
//				m_CltTodayThread.SetCltPeriod(CLT_TODAY_TO);
//				m_CltCurrentThread.SetCltTimeType(TIME_CURRENT);
//				m_CltCurrentThread.SetCltPeriod(CLT_CURRENT_TO);
//				m_CltFutureThread.SetCltTimeType(TIME_FUTURE);
//				m_CltFutureThread.SetCltPeriod(CLT_FUTURE_TO);

//				m_CltTodayThread.Start();
//				m_CltCurrentThread.Start();
//				m_CltFutureThread.Start();
			}

			pData->strSend = CreateEncryptResult(type, true);
//		}
//		else
//		{
//			pData->strSend = CreateEncryptResult(type, false);
//		}
		break;

	case CMD_STOP:
		if (bStopFirst)
		{
//			bStartFirst = true;
//			bStopFirst = false;

//			m_CltTodayThread.Pause();
//			m_CltCurrentThread.Pause();
//			m_CltFutureThread.Pause();
		}

		pData->strSend = CreateEncryptResult(type, true);
		break;

	case CMD_QUIT:
		Exit();//设置退出标志，停止epoll轮寻
		pData->strSend = CreateEncryptResult(type, true);
		EtHandleSend(pData);//退出命令比较特殊，主动调用发送函数
		//退出线程
//		m_CltTodayThread.Quit();
//		m_CltCurrentThread.Quit();
//		m_CltFutureThread.Quit();
		return;
		break;

	case CMD_HEART:
		//判断一个线程就行
		//yes:正在运行
		//no:睡眠
		pData->strSend = CreateEncryptResult(type, !m_CltTodayThread.IsSleep());
		break;

	case CMD_ADDUSER:
		{
			bool ok;
			StringMap upMap;
			ok = GetUserFromJsonData(data, upMap);
			if (ok)
			{
				pSet->AddUserMap(upMap);
			}
			pData->strSend = CreateEncryptResult(type, ok);
		}
		break;

	case CMD_ADDURL:
		{
			bool ok;
			StringVector urlVec;
			ok = GetUrlFromJsonData(data, urlVec);
			if (ok)
			{
				pSet->AddUrlVec(urlVec);
			}
			pData->strSend = CreateEncryptResult(type, ok);
		}
		break;

	case CMD_SET:
		{
			bool ok;
			string addr, user, passwd;
			ok = GetSqlFromJsonData(data, addr, user, passwd);
			if (ok)
			{
//				pSet->SetSqlInfo(addr, user, passwd);
			}
			pData->strSend = CreateEncryptResult(type, ok);
		}
		break;

	case CMD_VERSION:
		pData->strSend = CreateEncryptResult(type, APP_VERSION);
		break;

	case CMD_NONE:
	default:
		break;
	}

	//设置事件
	EpollCtlMOD(EPOLLOUT | EPOLLET, pData);
}

void CTcpServer::DealSendData(int fd, const string &data)
{
    int   nsend;
    int   len;
    char *pBuf;

    len  = data.size();
    pBuf = (char *)(data.c_str());
    while (1)
    {
        errno = 0;
        nsend = send(fd, pBuf, len, 0);
        if (-1 == nsend)
        {
            if (EAGAIN == errno)
            {
                usleep(200);
                continue;
            }
            break;
        }
        else if (nsend == len)//写完
        {
            break;
        }
        else if (nsend < len)//缓冲区未写满
        {
            len  -= nsend;
            pBuf += nsend;
        }
    }

//    printf("send:%s\n", data.c_str());
}

bool CTcpServer::EpollCreate(int size)
{
    m_iMaxEvents = size;
    m_iEpollFd = epoll_create(size);
    if (-1 == m_iEpollFd)
    {
        m_iEpollFd = 0;
        return false;
    }
    return true;
}

bool CTcpServer::EpollWait(int timeout, int &nfds)
{
    nfds = epoll_wait(m_iEpollFd, m_EventArray, m_iMaxEvents, timeout);
    return (-1 != nfds);
}

bool CTcpServer::EpollCtlAdd(unsigned int events, EpollData *pData)
{
    m_Event.events   = events;
    m_Event.data.ptr = pData;
    return (-1 != epoll_ctl(m_iEpollFd, EPOLL_CTL_ADD, pData->fd, &m_Event));
}

bool CTcpServer::EpollCtlDel(int fd)
{
    m_Event.data.fd = fd;
    return (-1 != epoll_ctl(m_iEpollFd, EPOLL_CTL_DEL, fd, &m_Event));
}

bool CTcpServer::EpollCtlMOD(unsigned int events, EpollData *pData)
{
    m_Event.events   = events;
    m_Event.data.ptr = pData;
    return (-1 != epoll_ctl(m_iEpollFd, EPOLL_CTL_MOD, pData->fd, &m_Event));
}

//accept所有连接
bool CTcpServer::EtHandleAccept()
{
    int connfd;
    socklen_t addrlen;
    struct sockaddr_in addrClient;

    while (1)
    {
        errno = 0;
        addrlen = sizeof(struct sockaddr_in);
        connfd = accept(m_iSock, (struct sockaddr *)&addrClient, &addrlen);
        if (-1 != connfd)
        {
            SetNonBlocking(connfd);
            if (!RegEpollFd(connfd, EPOLLIN | EPOLLET))
            {
                return false;
            }
        }
        else
        {
            //处理完成或错误
            if (EAGAIN == errno || EMFILE == errno || ECONNABORTED == errno || EPROTO == errno || EINTR == errno)
            {
                break;
            }
        }
    }

    return true;
}

void CTcpServer::EtHandleRecv(EpollData *pData)
{
    int     nrecv;
    int     fd;
    char    recvbuf[TCP_PACK_SIZE+1];
    bool    bclose;

    bclose = false;
    fd = pData->fd;
    while (1)
    {
        errno = 0;
        nrecv = recv(fd, recvbuf, TCP_PACK_SIZE, 0);
        if (-1 == nrecv)
        {
            //主动关闭
            if (EBADF == errno || ECONNRESET == errno)
            {
                bclose = true;
            }
            else if (EAGAIN == errno)//无数据可读,读取完成
            {
            }
            break;
        }
        else if (0 == nrecv)
        {
            bclose = true;
            break;
        }
        else if (nrecv > 0)
        {
            pData->strRecv.append(recvbuf, nrecv);

            //若接受512k到数据还未结束，则关闭该fd，防止攻击
            if (pData->strRecv.size() > MAX_RECVSIZE)
            {
                UnregEpollFd(fd);
            }
        }
    }

    if (bclose)
    {
        //删除
        UnregEpollFd(fd);
        return;
    }

    //是否以'#'字符结尾
    if (CPublic::EndWith(pData->strRecv, MSG_FINISHED))
    {
    	//删除最后一个标志
    	pData->strRecv.erase(pData->strRecv.end()-1);

		//处理接收到的数据
		DealRecvData(pData);

//    	printf("recv.size=%u\nrecv:\n%s\n", (uint)(pData->strRecv.size()), pData->strRecv.c_str());
    }
    else
	{
		EpollCtlMOD(EPOLLIN | EPOLLET, pData);
	}
}

void CTcpServer::EtHandleSend(EpollData *pData)
{
	DealSendData(pData->fd, pData->strSend);
	UnregEpollFd(pData->fd);
}

struct epoll_event *CTcpServer::EpollEvent(int index)
{
    if (index > m_iMaxEvents-1)
    {
        return NULL;
    }
    return &m_EventArray[index];
}

EpollData *CTcpServer::CreateEpData(int fd)
{
    EpollData *pData = new EpollData();
    pData->fd        = fd;
    pData->strRecv.clear();
    pData->strSend.clear();
    pData->timeout.SetTimeout(SOCKET_TIMEOUT);
    //启动计时
    pData->timeout.StartTimer();
    return pData;
}

string CTcpServer::GetTaskkey(const string &data)
{
    Json::Value jsonValue;
    string str;

    if (!CJson::ParseStringToJsonValue(data, jsonValue))
    {
        return string();
    }
    return CJson::JsonValueToString(jsonValue["getResult"]);
}

//获取命令类型，将data的数据传递出去
bool CTcpServer::GetCmdType(const string &str, string &data, CmdType &type)
{
	string decrypt, cmd;
	StringMap jm;

	//解密
	decrypt = str;
//	if (!CAes::DecryptFromHexDefault(str, decrypt))
//	{
//		return false;
//	}

	//解析
	if (!CJson::JsonToMap(decrypt, jm))
	{
		return false;
	}

	//判断类型
	cmd = jm["cmd"];
	if (cmd.empty())
	{
		return false;
	}

	if ("start" == cmd)
	{
		type = CMD_START;
	}
	else if ("stop" == cmd)
	{
		type = CMD_STOP;
	}
	else if ("quit" == cmd)
	{
		type = CMD_QUIT;
	}
	else if ("heart" == cmd)
	{
		type = CMD_HEART;
	}
	else if ("adduser" == cmd)
	{
		type = CMD_ADDUSER;
	}
	else if ("addurl" == cmd)
	{
		type = CMD_ADDURL;
	}
	else if ("set" == cmd)
	{
		type = CMD_SET;
	}
	else if ("version" == cmd)
	{
		type = CMD_VERSION;
	}
	else
	{
		type = CMD_NONE;
		return false;
	}

	//获取data
	data = jm["data"];

	return true;
}

string CTcpServer::CreateCmdTypeString(CmdType type)
{
	static string arr[] = {"none", "start", "stop", "quit", "heart", "adduser", "addurl", "set", "version"};
	return arr[type];
}

/*
生成加密的结果:
{
	cmd : 命令
	data:
	{
		res : yes/no
	}
}
*/
string CTcpServer::CreateEncryptResult(CmdType type, bool res)
{
	Json::Value jv, jvd;
	Json::FastWriter jw;
	string strRes, strEncryptRes;

	jvd["res"] = res ? "yes" : "no";

	jv["cmd"] = CreateCmdTypeString(type);
	jv["data"] = jvd;

	strRes = jw.write(jv);
//	CAes::EncryptToHexDefault(strRes, strEncryptRes);
	strEncryptRes = strRes;
	return strEncryptRes;
}

/*
生成加密的结果:
{
	cmd : 命令
	data:
	{
		res : 信息
	}
}
*/
string CTcpServer::CreateEncryptResult(CmdType type, const string &res)
{
	Json::Value jv, jvd;
	Json::FastWriter jw;
	string strRes, strEncryptRes;

	jvd["res"] = res;

	jv["cmd"] = CreateCmdTypeString(type);
	jv["data"] = jvd;

	strRes = jw.write(jv);
//	CAes::EncryptToHexDefault(strRes, strEncryptRes);
	strEncryptRes = strRes;
	return strEncryptRes;
}

bool CTcpServer::GetUserFromJsonData(const string &data, StringMap &upMap)
{
	string user, passwd;
	StringList strList;
	StringVector strVec;

	//解析
	if (!CJson::JsonArrToList(data, strList))
	{
		return false;
	}

	//分解出用户名密码,格式为:用户名|密码
	upMap.clear();
	for (StringList::iterator it = strList.begin(); it != strList.end(); ++it)
	{
		strVec = CPublic::Split(*it, '|');
		if (strVec.size() == 2)
		{
			user	= strVec[0];
			passwd	= strVec[1];
			if (!user.empty() && !passwd.empty())
			{
				//添加到容器
				upMap[user] = passwd;
			}
		}
	}
	if (upMap.empty())
	{
		return false;
	}

	return true;
}

bool CTcpServer::GetUrlFromJsonData(const string &data, StringVector &vec)
{
	string user, passwd;
	StringMap jm;
	StringList strList;

	//解析
	if (!CJson::JsonArrToList(data, strList))
	{
		return false;
	}

	//分解出用户名密码,格式为:用户名|密码
	vec.clear();
	for (StringList::iterator it = strList.begin(); it != strList.end(); ++it)
	{
		if (!(*it).empty())
		{
			vec.push_back((*it));
		}
	}
	if (vec.empty())
	{
		return false;
	}

	return true;
}

bool CTcpServer::GetSqlFromJsonData(const string &data, string &sqlAddr, string &sqlUser, string &sqlPasswd)
{
	StringMap jm;

	//解析
	if (!CJson::JsonToMap(data, jm))
	{
		return false;
	}

	//读取数据
	sqlAddr		= jm["sqladdr"];
	sqlUser		= jm["sqluser"];
	sqlPasswd	= jm["sqlpasswd"];
	if (sqlAddr.empty() || sqlUser.empty() || sqlPasswd.empty())
	{
		return false;
	}

	return true;
}

bool CTcpServer::RegEpollFd(int fd, unsigned int events)
{
    EpollData *pData = CreateEpData(fd);
    if (!EpollCtlAdd(events, pData))
    {
        return false;
    }
    m_EpollFdMap[fd] = pData;
    return true;
}

void CTcpServer::UnregEpollFd(int fd)
{
    EpollCtlDel(fd);
    close(fd);

    EpollFdMap::iterator it = m_EpollFdMap.find(fd);
    if (m_EpollFdMap.end() != it)
    {
        delete (EpollData *)(it->second);
        m_EpollFdMap.erase(it);
    }
}

void CTcpServer::UnregAllEpollFd()
{
    //循环释放第一个，直到map为空
    while (!m_EpollFdMap.empty())
    {
        UnregEpollFd(m_EpollFdMap.begin()->first);
    }

    //close epoll fd
    if (m_iEpollFd)
    {
        close(m_iEpollFd);
    }
}

void CTcpServer::CleanTimeoutFd()
{
    int fd;
    EpollData *pData;
    list<int> fdList;

    for (EpollFdMap::iterator it = m_EpollFdMap.begin(); it != m_EpollFdMap.end(); ++it)
    {
        fd = it->first;
        pData = (EpollData *)(it->second);
        //m_iSock(监听sock)不能删除
        if (fd != m_iSock && pData->timeout.IsTimeout())
        {
            fdList.push_back(fd);
        }
    }

    for (list<int>::iterator it = fdList.begin(); it != fdList.end(); ++it)
    {
        UnregEpollFd(*it);
    }
}
