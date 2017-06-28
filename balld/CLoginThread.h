#pragma once

#include <stdhdr.h>
#include <CThread.h>

class CLoginThread : public CThread
{
public:
	CLoginThread();
	~CLoginThread();
	//添加用户信息
	void	AddUserPasswd(const StringMap &upMap);
	//获取已登陆的uid
	string	GetLoginUid();
	//设置uid无效
	void	SetUidInvalid(const string &uid);

protected:
	virtual void Run();

private:
	//uid是否过期
	bool	IsUidOutOfDate(const string &uid);

private:
	string	m_strLoginUid;
};
