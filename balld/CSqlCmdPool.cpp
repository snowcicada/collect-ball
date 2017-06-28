#include <CSqlCmdPool.h>

CSqlCmdPool 		*CSqlCmdPool::m_pInstance = NULL;
CSqlCmdPool::CGarbo CSqlCmdPool::m_Garbo;
CMutex				CSqlCmdPool::m_SingleMutex;

CSqlCmdPool::CSqlCmdPool()
{

}

CSqlCmdPool::~CSqlCmdPool()
{

}

CSqlCmdPool *CSqlCmdPool::Instance()
{
	m_SingleMutex.Lock();
	if (!m_pInstance)
	{
		m_pInstance = new CSqlCmdPool();
	}
	m_SingleMutex.Unlock();
	return m_pInstance;
}

void CSqlCmdPool::Destroy()
{
	m_SingleMutex.Lock();
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	m_SingleMutex.Unlock();
}

////添加/导出足球数据
//void CSqlCmdPool::PushFootBall(FootBallList &ftList)
//{
////	m_FootBallMutex.Lock();
////	m_FootBallMutex.Unlock();
//}
//
//bool CSqlCmdPool::PopFootBall(FootBallList &ftList)
//{
//	return true;
//}

////添加Ballata数据
//void CSqlCmdPool::AppendBallData(const BallData &bd)
//{
//
//}
//
////添加MatchChampionMap数据
//void CSqlCmdPool::AppendMatchChampionMap(const MatchChampionMap &mcMap)
//{
//
//}

//void CSqlCmdPool::PushCmd(const string &cmd)
//{
//	m_CmdMutex.Lock();
//	m_strCmdList.push_back(cmd);
//	m_CmdMutex.Unlock();
//}
//
//bool CSqlCmdPool::PopCmd(StringList &cmdList)
//{
//	m_CmdMutex.Lock();
//	cmdList.splice(cmdList.end(), m_strCmdList);
//	m_CmdMutex.Unlock();
//	return !cmdList.empty();
//}

