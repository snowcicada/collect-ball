#pragma once
#include <stdhdr.h>
#include <CCollectThread.h>

class CTcpServer
{
public:
    CTcpServer();
    virtual ~CTcpServer();

    bool        Listen(unsigned short port);
    bool        EpollLoop();
    bool        SetNonBlocking(int fd);
    void        Exit();

private:
    bool        Socket(unsigned short port);
    bool        Bind();
    void        DealRecvData(EpollData *pData);
    void        DealSendData(int fd, const string &data);

    bool        EpollCreate(int size);
    bool        EpollWait(int timeout, int &nfds);
    bool        EpollCtlAdd(unsigned int events, EpollData *pData);
    bool        EpollCtlDel(int fd);
    bool        EpollCtlMOD(unsigned int events, EpollData *pData);
    bool        EtHandleAccept();
    void        EtHandleRecv(EpollData *pData);
    void        EtHandleSend(EpollData *pData);
    struct epoll_event *EpollEvent(int index);
    EpollData  *CreateEpData(int fd);

    string      GetTaskkey(const string &data);//��ȡΨһ��
    bool		GetCmdType(const string &str, string &data, CmdType &type);
    string		CreateCmdTypeString(CmdType type);
    string		CreateEncryptResult(CmdType type, bool res);
    string		CreateEncryptResult(CmdType type, const string &res);
    bool		GetUserFromJsonData(const string &data, StringMap &upMap);
    bool		GetUrlFromJsonData(const string &data, StringVector &vec);
	bool		GetSqlFromJsonData(const string &data, string &sqlAddr, string &sqlUser, string &sqlPasswd);

    bool        RegEpollFd(int fd, unsigned int events);
    void        UnregEpollFd(int fd);
    void        UnregAllEpollFd();
    void        CleanTimeoutFd();

private:
    int 				m_iSock;
    int 				m_iEpollFd;
    int					m_iMaxEvents;
    ushort			  	m_iPort;
    EpollFdMap      	m_EpollFdMap;
    struct sockaddr_in 	m_SockAddr;
    struct epoll_event 	m_Event;//�����¼�
    struct epoll_event 	m_EventArray[MAX_EVENTS];
    bool 				m_bRunning;//���б�־:true���� falseֹͣ
    CCollectThread		m_CltTodayThread;//�������²ɼ��߳�
    CCollectThread		m_CltCurrentThread;//����ɼ��߳�
    CCollectThread		m_CltFutureThread;//���̲ɼ��߳�
};



