#include <CMysql.h>

CMysql::CMysql()
{
	m_pMysqlRes = NULL;
	m_MysqlRow = NULL;
	bInit = false;
}

CMysql::~CMysql()
{
}

bool CMysql::Init()
{
	if (mysql_init(&m_Mysql))
	{
		bInit = true;
		return true;
	}
	return NULL;
}

void CMysql::Close()
{
	if (bInit)
	{
		mysql_close(&m_Mysql);
		bInit = false;
	}
}

bool CMysql::Connect(const string &host, const string &user, const string &pwd, const string &db, uint port)
{
	if (!mysql_real_connect(&m_Mysql, host.c_str(), user.c_str(), pwd.c_str(), db.c_str(), port, NULL, 0))
	{
		return false;
	}
	Query("set names 'utf8'");
	return true;
}

bool CMysql::Query(const string &cmd)
{
	return (0 == mysql_query(&m_Mysql, cmd.c_str()));
}

bool CMysql::GetResult()
{
	m_pMysqlRes = mysql_store_result(&m_Mysql);
	return (NULL != m_pMysqlRes);
}

bool CMysql::NextRow()
{
	m_MysqlRow = NULL;
	if (m_pMysqlRes)
	{
		m_MysqlRow = mysql_fetch_row(m_pMysqlRes);
		if (m_MysqlRow)
		{
			return true;
		}
	}
	return false;
}

string CMysql::ValueString(int idx)
{
	if (!m_MysqlRow)
	{
		return "";
	}
	return string(m_MysqlRow[idx]);
}

void CMysql::FreeResult()
{
	if (m_pMysqlRes)
	{
		mysql_free_result(m_pMysqlRes);
		m_pMysqlRes = NULL;
	}
}

uint CMysql::GetRowCount()
{
	if (!m_pMysqlRes)
	{
		return 0;
	}
	return (uint)mysql_num_rows(m_pMysqlRes);
}

uint CMysql::GetColCount()
{
	if (!m_pMysqlRes)
	{
		return 0;
	}
	return (uint)mysql_num_fields(m_pMysqlRes);
}

string CMysql::GetSqlError()
{
	return mysql_error(&m_Mysql);
}

bool CMysql::GlobalInit()
{
	return (0 == mysql_library_init(0, NULL, NULL));
}

void CMysql::GlobalEnd()
{
	mysql_library_end();
}

void CMysql::ThreadEnd()
{
	mysql_thread_end();
}

