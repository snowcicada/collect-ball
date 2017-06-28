#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <stdlib.h>

using namespace std;

typedef vector<string>      StringVector;
typedef map<string, string> StringMap;

typedef struct
{
	string 		strGroup;
	StringMap 	strKvMap;
}IniGroup;

typedef list<IniGroup *> IniGrpList;

class CIniFile
{
public:
    CIniFile();
    CIniFile(const string &fileName);
    ~CIniFile();

    //设置配置文件路径
    void        SetFileName(const string &fileName);
    string      GetFileName();

    //创建组
    bool        BeginGroup(const string &grp);
    void        EndGroup();

    //设置key-value: int string (只支持2种常用类型)
    void        SetValue(const string &key, int val);
    void        SetValue(const string &key, const string &val);
    int         GetValueToInt(const string &key);
    string      GetValueToStr(const string &key);
    StringVector GetAllKeys();

    //操作key
    bool        Contains(const string &key);
    void        Remove(const string &key);
    void        Clear();


private:
    IniGroup   *GetKvMap(const string &grp);
    void        ReadFromFile();
    void        WriteToFile();

private:
    string      m_strFileName;
    IniGroup   *m_pIniGrp;//当前正在使用的组
    IniGrpList  m_GrpList;
};
