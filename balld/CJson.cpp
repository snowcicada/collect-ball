#include <CJson.h>
#include <CPublic.h>

CJson::CJson()
{
}

CJson::~CJson()
{
}

bool CJson::IsValidJson(const string &data)
{
    if (data.empty())
    {
        return false;
    }
    const char *ptr = data.c_str();
    return Count(ptr, '{') == Count(ptr, '}') &&
           Count(ptr, '[') == Count(ptr, ']') &&
           Count(ptr, '"')%2 == 0;
}

int CJson::Count(const char *str, const char ch)
{
    int num = 0;
    char *p = (char *)str;
    while (*p)
    {
        if (ch ==*p)
        {
            ++num;
        }
        ++p;
    }
    return num;
}

void CJson::ParseJsonData(const string &jsondata, StringList &lst)
{
	Json::Reader		jsonReader;
	Json::FastWriter	jsonWriter;
	Json::Value			jsonValue;
	Json::Value 		tmpJson;
	string				tmpStr;
	StringVector		keyVec;

	if (!ParseStringToJsonValue(jsondata, jsonValue))
    {
        return;
    }

    printf("%s\n\n", jsonValue.toStyledString().c_str());
	GetJsonKeys(jsonValue, keyVec);
	for (StringVector::iterator it = keyVec.begin(); it != keyVec.end(); ++it)
	{
		tmpStr.clear();
		tmpJson.clear();

		tmpJson[*it] = jsonValue[*it];
		tmpStr = jsonWriter.write(tmpJson);
		//将该站点的value和key再包装成一个对象，转换成string型，添加到lst
		lst.push_back(tmpStr);
	}
}

bool CJson::ParseStringToJsonValue(const string &data, Json::Value &value)
{
    if (!IsValidJson(data))
    {
        return false;
    }
    Json::Reader jsonReader;
    value.clear();
    return jsonReader.parse(data, value);
}

void CJson::GetJsonKeys(const Json::Value &val, StringVector &vec)
{
    vec = val.getMemberNames();
}

bool CJson::JsonToMap(const string &jsonStr, StringMap &jsonMap)
{
    Json::Value jsonValue;
    StringVector keyVec;

	if (!ParseStringToJsonValue(jsonStr, jsonValue))
    {
        return false;
    }

	GetJsonKeys(jsonValue, keyVec);
	jsonMap.clear();
	for (StringVector::iterator it = keyVec.begin(); it != keyVec.end(); ++it)
    {
        jsonMap[*it] = JsonValueToStringEx(jsonValue[*it]);
    }

    return true;
}

bool CJson::JsonArrToList(const string &jsonStr, StringList &lst)
{
	Json::Value jv;
	Json::Reader jr;
	int size;

	if (!jr.parse(jsonStr, jv) || !jv.isArray())
	{
		return false;
	}

	lst.clear();
	size = jv.size();
	for (int i = 0; i < size; i++)
	{
		lst.push_back(CJson::JsonValueToStringEx(jv[i]));
	}

	return true;
}

string CJson::JsonValueToStringEx(const Json::Value &value)
{
    char buf[256];
	string str;

	switch (value.type())
	{
	case Json::intValue:
		sprintf(buf, "%d", value.asInt());
		str.assign(buf);
		break;
	case Json::uintValue:
		sprintf(buf, "%u", value.asUInt());
		str.assign(buf);
		break;
	case Json::realValue:
		sprintf(buf, "%f", value.asDouble());
		str.assign(buf);
		break;
	case Json::stringValue:
		str = value.asString();
		break;
	case Json::arrayValue:
		{
			Json::FastWriter jw;
			str = jw.write(value);
		}
		break;
	case Json::objectValue:
		{
			Json::FastWriter jw;
			str = jw.write(value);
		}
		break;
	default:
		break;
	}

	return str;
}

string CJson::JsonValueToString(const Json::Value &value)
{
    if (!value.isString())
    {
        return string();
    }
    return value.asString();
}

int CJson::JsonValueToInt(const Json::Value &value)
{
    if (!value.isInt())
    {
        return -1;
    }
    return value.asInt();
}

//生成数组
void CJson::CreateJsonArray(const StringList &lst, string &data)
{
	Json::Value jv;
	Json::FastWriter jw;

	if (lst.empty())
	{
		data = "";
		return;
	}

	for (StringList::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		jv.append(*it);
	}
	data = jw.write(jv);
}

//数组不会改变原有的顺序
string CJson::JsonTest()
{
    Json::Value jsonValue;
    Json::Value jsonArr;
    Json::FastWriter jsonWriter;
    Json::Reader jr;
    Json::Value jv;

//    jsonValue["arr"].append("aa");
//    jsonValue["arr"].append("bb");
//    jsonValue["arr"].append("cc");
    jsonValue.append("cc");
    jsonValue.append("aa");
    jsonValue.append("12");
    jsonValue.append("bb");
//    jsonValue["checkout"] = 0;

    jr.parse(jsonWriter.write(jsonValue), jv);
//    if (jv.isArray())
//	{
//		cout << "array" << endl;
//	}
//    Json::Value jarr = jv["arr"];
    int sz = jv.size();
    cout << sz << endl;
    for (int i = 0; i < sz; i++)
	{
		cout << jv[i] <<endl;
	}

    return jsonWriter.write(jsonValue);

    //return jsonValue.toStyledString();
}

