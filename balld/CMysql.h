#pragma once

#include <stdhdr.h>

class CMysql
{
public:
	CMysql();
	~CMysql();

	bool	Init();
	void	Close();
	bool	Connect(const string &host, const string &user, const string &pwd, const string &db, uint port);
	bool	Query(const string &cmd);
	bool	GetResult();
	bool	NextRow();
	string	ValueString(int idx);
	void	FreeResult();
	uint	GetRowCount();
	uint	GetColCount();
	string	GetSqlError();
	static bool GlobalInit();
	static void GlobalEnd();
	static void ThreadEnd();

private:
	MYSQL		m_Mysql;
	MYSQL_RES	*m_pMysqlRes;
	MYSQL_ROW	m_MysqlRow;
	bool		bInit;
};

