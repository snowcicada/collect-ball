#pragma once
#include <stdhdr.h>

class CTcpSocket
{
public:
    CTcpSocket();
    ~CTcpSocket();

    bool ConnectToHost(const string &ip, unsigned short port);
    void Close();
    bool Send(const string &msg);
    bool Recv(string &msg);
    string GetError() {return m_strError;}

private:
    int m_iSock;
    string m_strIp;
    string m_strError;
    unsigned short m_iPort;
    struct sockaddr_in m_SockAddr;
};
