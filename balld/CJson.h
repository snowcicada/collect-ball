#pragma once
#include <stdhdr.h>

class CJson
{
public:
    CJson();
    virtual ~CJson();

    static bool         IsValidJson(const string &data);//简单的判断
    static int          Count(const char *str, const char ch);//计算ch到个数
    static void         ParseJsonData(const string &jsondata, StringList &lst);
    static bool         ParseStringToJsonValue(const string &data, Json::Value &value);
    static void         GetJsonKeys(const Json::Value &val, StringVector &vec);
    static bool         JsonToMap(const string &jsonStr, StringMap &jsonMap);
    static bool			JsonArrToList(const string &jsonStr, StringList &lst);
    static string       JsonValueToStringEx(const Json::Value &value);//把所有json数据转化成string
    static string       JsonValueToString(const Json::Value &value);
    static int          JsonValueToInt(const Json::Value &value);
    static void			CreateJsonArray(const StringList &lst, string &data);
    static string       JsonTest();
};
