#pragma once
#include <stdhdr.h>

class CPublic
{
public:
    static unsigned long    atoul(const char *nptr);
    static void             CreateDir(const string &pathname, mode_t mode = 0777);
    static void             RemoveFile(const string &filepath);
    static string           GetCurrentDate();
    static string           GetCurrentDateTime();
    static string           GetCurrentFileTime();
    static int              GetCurrentMinute();
    static int              MinuteInterval(int iOld, int iNew);
//    static string           GenUuid();
    static void             RemoveChar(string &str, char ch);
    static void             RemoveStr(StringVector &vec, const string &str);
    static string           FillLogStr(const string &siteid, const string &step, const string &err, bool ok, unsigned long tid);
    static StringVector     Split(const string &str, char delim);
    static string           Trim(const string &str);
    static bool             IsExistsFile(const char *path);
    static bool             IsExistsFile(const string &path);
    static string           &ToUpper(string &str);
    static string           &ToLower(string &str);
    static bool             ContainSubstr(const string &src, const string &sub, bool sensitive=true);
    static bool             ContainSubch(const string &src, char ch);
    static bool				StartWith(const string &str, const string &sub);
    static bool				StartWith(const string &str, char ch);
	static bool				EndWith(const string &str, const string &sub);
	static bool				EndWith(const string &str, char ch);
    static int              CountSubStr(const string &src, const string &sub);
    static string           GetSubStr(const string &str, char sch, char ech);
    static bool             HasKey(const StringMap &strMap, const string &key);
    static bool             WriteToFile(const string &fileName, const string &data);
    static bool             ReadFromFile(const string &fileName, string &data);
    static bool             CopyFile(const string &srcFile, const string &dstFile);
};
