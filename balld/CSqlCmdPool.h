#pragma once

#include <stdhdr.h>

class CSqlCmdPool
{
private:
	CSqlCmdPool();
	~CSqlCmdPool();

public:
	static CSqlCmdPool *Instance();
	static void			Destroy();

	//添加/导出足球数据
//	void	PushFootBall(FootBallList &ftList);
//	bool	PopFootBall(FootBallList &ftList);

//	//添加Ballata数据
//	void	AppendBallData(const BallData &bd);
//	//添加MatchChampionMap数据
//	void	AppendMatchChampionMap(const MatchChampionMap &mcMap);
//	void	PushCmd(const string &cmd);
//	bool	PopCmd(StringList &cmdList);

private:
//	FootBallList	m_FootBallList;
//	CMutex			m_FootBallMutex;
//	CMutex		m_CmdMutex;
	StringList	m_strCmdList;
	static CSqlCmdPool *m_pInstance;
	static CMutex		m_SingleMutex;

	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (CSqlCmdPool::m_pInstance)
			{
				delete CSqlCmdPool::m_pInstance;
			}
		}
	};
	static CGarbo m_Garbo;
};
