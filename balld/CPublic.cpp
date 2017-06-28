#include <CPublic.h>

unsigned long CPublic::atoul(const char *nptr)
{
	unsigned long ul = 0;
	char *p = (char *)nptr;
	while (*p >= '0' && *p <= '9')
	{
		ul = ul * 10 + *p - '0';
		p++;
	}
	return ul;
}

void CPublic::CreateDir(const string &pathname, mode_t mode)
{
	if (!IsExistsFile(pathname.c_str()))
    {
        char cmd[256];
        sprintf(cmd, "mkdir -p %s", pathname.c_str());
        system(cmd);
    }
}

void CPublic::RemoveFile(const string &filepath)
{
    if (IsExistsFile(filepath.c_str()))
    {
        remove(filepath.c_str());
    }
}

string CPublic::GetCurrentDate()
{
	char	   buf[128];
	time_t	   timep;
	struct tm *ptm;
	timep = time(NULL);
	ptm = localtime(&timep);
	sprintf(buf, "%04d%02d%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
	return string(buf);
}

string CPublic::GetCurrentDateTime()
{
	char	   buf[128];
	time_t	   timep;
	struct tm *ptm;
	timep = time(NULL);
	ptm = localtime(&timep);
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	return string(buf);
}

string CPublic::GetCurrentFileTime()
{
    char	   buf[128];
	time_t	   timep;
	struct tm *ptm;
	timep = time(NULL);
	ptm = localtime(&timep);
	sprintf(buf, "%04d%02d%02d%02d%02d%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	return string(buf);
}

int CPublic::GetCurrentMinute()
{
    time_t	   timep;
	struct tm *ptm;
	timep = time(NULL);
	ptm = localtime(&timep);
	return ptm->tm_min;
}

int CPublic::MinuteInterval(int iOld, int iNew)
{
    //分钟数范围:0-59
    //分钟数可能出现到情况
    //1  -> 6
    //58 -> 2
    //2  -> 2
    int interval = 0;
    if (iOld < iNew)
    {
        interval = iNew - iOld;
    }
    else if (iOld > iNew)
    {
        interval = (60-iOld) + iNew;
    }
    else
    {
        interval = 0;
    }
    return interval;
}

//string CPublic::GenUuid()
//{
//	char buf[37];
//	uuid_t uuid;
//	uuid_generate(uuid);
//	sprintf(buf, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
//			uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
//			uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
//	return string(buf);
//}


void CPublic::RemoveChar(string &str, char ch)
{
	string::size_type pos;
	while ((pos = str.find(ch, 0)) != string::npos)
	{
		str.erase(pos, 1);
	}
}

void CPublic::RemoveStr(StringVector &vec, const string &str)
{
    StringVector::iterator it = vec.begin();
    while (it != vec.end())
    {
        if ((*it).find(str, 0) != string::npos)
        {
            it = vec.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

string CPublic::FillLogStr(const string &siteid, const string &step, const string &err, bool ok, unsigned long tid)
{
	char buf[512];
	string error;
	if (ok)
	{
		error = "success";
	}
	else if (err.empty())
	{
		error = "failed";
	}
	else
	{
		error = err;
	}

	snprintf(buf, sizeof(buf), "[%s] %lu %s %s %s\n",
			GetCurrentDateTime().c_str(), tid, siteid.c_str(), step.c_str(), error.c_str());
	return string(buf);
}

StringVector CPublic::Split(const string &str, char delim)
{
    string substring;
    string::size_type start = 0, index = 0;
    StringVector rlst;

    do
    {
        index = str.find_first_of(delim, start);
        if (index != string::npos)
        {
            substring = str.substr(start, index - start);
            if (!substring.empty())
            {
                rlst.push_back(CPublic::Trim(substring));
            }
            start = str.find_first_not_of(delim, index);
            if (string::npos == start) break;
        }
        else
        {
            substring = str.substr(start, string::npos);
            if (!substring.empty())
            {
                rlst.push_back(CPublic::Trim(substring));
            }

        }
    } while(index != string::npos);

    return rlst;
}

string CPublic::Trim(const string &str)
{
    string cp(str);
    string::size_type spos = 0, epos = 0, len = 0, pos = 0;

    len = cp.size();
    for (pos = 0; pos < len; pos++)
    {
        if (' ' != cp[pos] && '\t' != cp[pos])
        {
            spos = pos;
            break;
        }
    }

    for (pos = len - 1; pos >= 0; pos--)
    {
        if (' ' != cp[pos] && '\t' != cp[pos])
        {
            epos = pos;
            break;
        }
    }

    return cp.substr(spos, epos - spos + 1);
}

bool CPublic::IsExistsFile(const char *path)
{
    return (0 == access(path, F_OK));
}

bool CPublic::IsExistsFile(const string &path)
{
    return IsExistsFile(path.c_str());
}

string &CPublic::ToUpper(string &str)
{
	for (string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = toupper(*it);
	}
	return str;
}

string &CPublic::ToLower(string &str)
{
	for (string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = tolower(*it);
	}
	return str;
}

bool CPublic::ContainSubstr(const string &src, const string &sub, bool sensitive)
{
    bool ok;

    //大小写敏感
    if (sensitive)
    {
        ok = (string::npos != src.find(sub));
    }
    else
    {
    	string lSrc = src;
    	string lSub = sub;
		ToLower(lSrc);
		ToLower(lSub);
        ok = (string::npos != lSrc.find(lSub));
    }

    return ok;
}

bool CPublic::ContainSubch(const string &src, char ch)
{
    return (string::npos != src.find(ch));
}

bool CPublic::StartWith(const string &str, const string &sub)
{
	//若sub的长度大于str，肯定不符合条件
	if (str.empty() || sub.empty() || str.size() < sub.size())
	{
		return false;
	}
	char *pStr = (char *)str.c_str();
	char *pSub = (char *)sub.c_str();
	uint len = 0;
	while (*pStr && *pSub && *pStr++ == *pSub++)
	{
		len++;
	}
	if (sub.size() != len)
	{
		return false;
	}
	return true;
}

bool CPublic::StartWith(const string &str, char ch)
{
	if (str.empty())
	{
		return false;
	}
	return (ch == str[0]);
}

bool CPublic::EndWith(const string &str, const string &sub)
{
	//若sub的长度大于str，肯定不符合条件
	if (str.empty() || sub.empty() || str.size() < sub.size())
	{
		return false;
	}
	char *pStr = (char *)str.c_str() + str.size() - 1;
	char *pSub = (char *)sub.c_str() + sub.size() - 1;
	uint len = 0;
	while (*pStr && *pSub && *pStr-- == *pSub--)
	{
		len++;
	}
	if (sub.size() != len)
	{
		return false;
	}
	return true;
}

bool CPublic::EndWith(const string &str, char ch)
{
	if (str.empty())
	{
		return false;
	}
	return (ch == *(str.end()-1));
}

int CPublic::CountSubStr(const string &src, const string &sub)
{
    int count = 0;
    string::size_type index = 0;

    while (string::npos != (index = src.find(sub, index)))
    {
        index += sub.size();
        count++;
    }

    return count;
}

//[group] => group
string CPublic::GetSubStr(const string &str, char sch, char ech)
{
    string::size_type spos = 0, epos = 0;

    spos = str.find(sch);
    epos = str.find(ech);

    if (string::npos != spos && string::npos != epos)
    {
        if (++spos <= --epos)
        {
            return str.substr(spos, epos - spos + 1);
        }
    }
    return "";
}

bool CPublic::HasKey(const StringMap &strMap, const string &key)
{
    return (strMap.end() != strMap.find(key));
}

bool CPublic::WriteToFile(const string &fileName, const string &data)
{
    FILE *fp = fopen(fileName.c_str(), "w");
    if (!fp)
    {
        return false;
    }

    if (1 != fwrite(data.c_str(), data.size(), 1, fp))
    {
        fclose(fp);
        return false;
    }
    fclose(fp);

    return true;
}

bool CPublic::ReadFromFile(const string &fileName, string &data)
{
    long  size;
    FILE *fp;

    fp = fopen(fileName.c_str(), "r");
    if (!fp)
    {
        return false;
    }

    if (-1 == fseek(fp, 0, SEEK_END))
    {
        fclose(fp);
        return false;
    }

    size = ftell(fp);
    rewind(fp);
    if (-1 == size)
    {
        fclose(fp);
        return false;
    }

    char *buf = new char[size+1];
    fread(buf, size+1, 1, fp);
    data = string(buf, size);
    delete [] buf;
    fclose(fp);

    return true;
}

bool CPublic::CopyFile(const string &srcFile, const string &dstFile)
{
    string strTmp;
    if (!ReadFromFile(srcFile, strTmp))
    {
        return false;
    }
    if (!WriteToFile(dstFile, strTmp))
    {
        return false;
    }
    return true;
}

