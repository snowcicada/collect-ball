#include <CTcpSocket.h>

CTcpSocket::CTcpSocket()
{
    m_iSock = 0;
    memset(&m_SockAddr, 0, sizeof(m_SockAddr));
}

CTcpSocket::~CTcpSocket()
{
    Close();
}

bool CTcpSocket::ConnectToHost(const string &ip, unsigned short port)
{
    struct timeval timeout;
    string strFunc;

    m_iSock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_iSock)
    {
        m_iSock = 0;
        strFunc = "socket";
        goto run_failed;
    }

    //设置接收和发送超时时间为20秒
    timeout.tv_sec  = 20;
    timeout.tv_usec = 0;
    if (-1 == setsockopt(m_iSock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)))
    {
        strFunc = "setsockopt_SO_SNDTIMEO";
        goto run_failed;
    }

    timeout.tv_sec  = 20;
    timeout.tv_usec = 0;
    if (-1 == setsockopt(m_iSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
    {
        strFunc = "setsockopt_SO_RCVTIMEO";
        goto run_failed;
    }

    m_SockAddr.sin_family       = AF_INET;
    m_SockAddr.sin_port         = htons(port);
    m_SockAddr.sin_addr.s_addr  = inet_addr(ip.c_str());
    if (-1 == connect(m_iSock, (struct sockaddr *)&m_SockAddr, sizeof(struct sockaddr_in)))
    {
        strFunc = "connect";
        goto run_failed;
    }

    m_strIp = ip;
    m_iPort = port;

    return true;

run_failed:
    if (m_iSock)
    {
        close(m_iSock);
        m_iSock = 0;
    }

    char buf[256];
    sprintf(buf, "%s error:%s", strFunc.c_str(), strerror(errno));
    m_strError = buf;

    return false;
}

void CTcpSocket::Close()
{
    if (m_iSock)
    {
        close(m_iSock);
        m_iSock = 0;
        memset(&m_SockAddr, 0, sizeof(m_SockAddr));
    }
}

bool CTcpSocket::Send(const string &msg)
{
    if (!m_iSock || msg.empty())
    {
        return false;
    }

    int nsend = send(m_iSock, msg.c_str(), msg.size(), 0);
    if (nsend != (int)msg.size())
    {
        return false;
    }
    return true;
}

bool CTcpSocket::Recv(string &msg)
{
    if (!m_iSock)
    {
        return false;
    }

    char buf[TCP_PACK_SIZE+1];
    int nrecv;

    msg.clear();
    while (1)
    {
        nrecv = recv(m_iSock, buf, TCP_PACK_SIZE, 0);
        if (-1 == nrecv)
        {
            if (EAGAIN == errno)
            {
                printf("[Error:Login timeout.]=>[file:%s function:%s line:%d]\n", __FILE__, __FUNCTION__, __LINE__);
                syslog(LOG_INFO, "[Error:Login timeout.]=>[file:%s function:%s line:%d]\n", __FILE__, __FUNCTION__, __LINE__);
            }
            else
            {
                printf("[Error:Login failed: %s, msg.size=%u.]=>[file:%s function:%s line:%d]\n", strerror(errno), (uint)(msg.size()), __FILE__, __FUNCTION__, __LINE__);
                syslog(LOG_INFO, "[Error:Login failed: %s, msg.size=%u.]=>[file:%s function:%s line:%d]", strerror(errno), (uint)(msg.size()), __FILE__, __FUNCTION__, __LINE__);
            }
            return false;
        }
        else if (0 == nrecv)
        {
            //对方已关闭sock
            Close();
            break;
        }
        else
        {
            msg += string(buf, nrecv);
        }
    }
    return true;
}
