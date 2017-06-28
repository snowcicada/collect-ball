#include <CIniFile.h>
#include <CPublic.h>

CIniFile::CIniFile()
{
    m_pIniGrp = NULL;
}

CIniFile::CIniFile(const string &fileName)
{
    m_pIniGrp = NULL;
    m_strFileName = fileName;
}

CIniFile::~CIniFile()
{

}

void CIniFile::SetFileName(const string &fileName)
{
    m_strFileName = fileName;
    ReadFromFile();
}

string CIniFile::GetFileName()
{
    return m_strFileName;
}

bool CIniFile::BeginGroup(const string &grp)
{
    if (m_pIniGrp || grp.empty() || m_strFileName.empty())
    {
        return false;
    }

    //是否存在
    IniGroup *pGrp = GetKvMap(grp);
    if (!pGrp)
    {
        //not exists
        pGrp = new IniGroup();
        pGrp->strGroup = grp;
        m_GrpList.push_back(pGrp);

        //global
        m_pIniGrp = pGrp;
    }
    else
    {
        //exists
        m_pIniGrp = pGrp;
    }

    return true;
}

void CIniFile::EndGroup()
{
    //write to file
    WriteToFile();

    //clear
    m_pIniGrp = NULL;
}

void CIniFile::SetValue(const string &key, int val)
{
    string strKey = CPublic::Trim(key);
    if (!m_pIniGrp || strKey.empty())
    {
        return;
    }
    char buf[12];
    sprintf(buf, "%d", val);
    m_pIniGrp->strKvMap[strKey] = buf;
}

void CIniFile::SetValue(const string &key, const string &val)
{
    string strKey = CPublic::Trim(key);
    if (!m_pIniGrp || strKey.empty())
    {
        return;
    }
    m_pIniGrp->strKvMap[strKey] = CPublic::Trim(val);
}

int CIniFile::GetValueToInt(const string &key)
{
    if (!m_pIniGrp)
    {
        return 0;
    }

    if (CPublic::HasKey(m_pIniGrp->strKvMap, key))
    {
        string val = m_pIniGrp->strKvMap[key];
        if (!val.empty())
        {
            return atoi(val.c_str());
        }
    }

    return 0;
}

string CIniFile::GetValueToStr(const string &key)
{
    if (!m_pIniGrp)
    {
        return "";
    }
    if (CPublic::HasKey(m_pIniGrp->strKvMap, key))
    {
        return m_pIniGrp->strKvMap[key];
    }
    return "";
}

StringVector CIniFile::GetAllKeys()
{
    if(!m_pIniGrp)
    {
        return StringVector();
    }
    StringVector strVec;
    for (StringMap::iterator it = m_pIniGrp->strKvMap.begin(); it != m_pIniGrp->strKvMap.end(); ++it)
    {
        strVec.push_back(it->first);
    }
    return strVec;
}

bool CIniFile::Contains(const string &key)
{
    if (!m_pIniGrp)
    {
        return false;
    }
    return (m_pIniGrp->strKvMap.end() != m_pIniGrp->strKvMap.find(key));
}

void CIniFile::Remove(const string &key)
{
    if (!m_pIniGrp)
    {
        return;
    }
    m_pIniGrp->strKvMap.erase(key);
}

void CIniFile::Clear()
{
    for (IniGrpList::iterator it = m_GrpList.begin(); it != m_GrpList.end(); ++it)
    {
        delete *it;
    }
    m_GrpList.clear();

    WriteToFile();
}

IniGroup *CIniFile::GetKvMap(const string &grp)
{
    IniGroup *pGrp;
    for (IniGrpList::iterator it = m_GrpList.begin(); it != m_GrpList.end(); ++it)
    {
        pGrp = *it;
        if (grp == pGrp->strGroup)
        {
            return pGrp;
        }
    }
    return NULL;
}

void CIniFile::ReadFromFile()
{
    fstream iniFile(m_strFileName.c_str(), ios::in);
    if (iniFile.fail())
    {
        return;
    }
    string key, value;
    char buf[1024];
    string strLine, strTmp;
    StringVector strVec;
    IniGroup *pGrp = NULL;

    m_GrpList.clear();
    while (!iniFile.eof())
    {
        iniFile.getline(buf, sizeof(buf));
        strLine = buf;

        if (CPublic::ContainSubch(strLine, '='))
        {
            if (!pGrp)
            {
                continue;
            }

            strVec = CPublic::Split(buf, '=');
            if (1 == strVec.size())
            {
                key = CPublic::Trim(strVec[0]);
                value = "";
            }
            else if (strVec.size() >= 2)
            {
                key = CPublic::Trim(strVec[0]);
                value = CPublic::Trim(strVec[1]);
            }
            pGrp->strKvMap[key] = value;
        }
        else
        {
            strTmp = CPublic::GetSubStr(strLine, '[', ']');
            if (strTmp.empty())
            {
                continue;
            }

            pGrp = new IniGroup();
            pGrp->strGroup = strTmp;
            m_GrpList.push_back(pGrp);
        }
    }

    iniFile.close();
}

void CIniFile::WriteToFile()
{
    fstream iniFile(m_strFileName.c_str(), ios::out | ios::trunc);
    if (iniFile.fail())
    {
        return;
    }

    IniGroup *pGrp;
    for (IniGrpList::iterator it = m_GrpList.begin(); it != m_GrpList.end(); ++it)
    {
        pGrp = *it;
        iniFile << '[' << pGrp->strGroup << ']' << endl;
        for (StringMap::iterator iter = pGrp->strKvMap.begin(); iter != pGrp->strKvMap.end(); ++iter)
        {
            iniFile << iter->first << '=' << iter->second << endl;
        }
    }

    iniFile.close();
}
