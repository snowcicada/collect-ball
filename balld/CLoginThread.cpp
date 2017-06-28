#include <CLoginThread.h>

CLoginThread::CLoginThread()
{

}

CLoginThread::~CLoginThread()
{

}

//添加用户信息
void CLoginThread::AddUserPasswd(const StringMap &upMap)
{

}

//获取已登陆的uid
string CLoginThread::GetLoginUid()
{
	return "";
}

//设置uid无效
void CLoginThread::SetUidInvalid(const string &uid)
{

}

void CLoginThread::Run()
{

}

//uid是否过期
bool CLoginThread::IsUidOutOfDate(const string &uid)
{
	return true;
}
