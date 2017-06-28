#include <CCurl.h>
#include <CPublic.h>

//模拟浏览器
//const char *CCurl::m_UserAgent = "Mozilla/5.0 (Windows NT 5.1; rv:17.0) Gecko/20100101 Firefox/17.0";
const char *CCurl::m_UserAgent = "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E)";

CCurl::CCurl()
{
	m_pCurl = NULL;
	m_pConnectionList = NULL;
}

CCurl::~CCurl()
{
	CloseCurl();
    RemoveCookieFile();
    DEBUG_DELETEPRINT
}

bool CCurl::Init()
{
	if (!m_pCurl)
	{
		m_pCurl = curl_easy_init();
		if (!m_pCurl)
		{
			fprintf(stderr, "curl_easy_init: %s\n", strerror(errno));
			return false;
		}
		if (!m_pConnectionList)
        {
            //长连接
            m_pConnectionList = curl_slist_append(m_pConnectionList, "Connection: keep-alive");
        }
	}

	return true;
}

//在curl使用期间，修改cookie，对curl不起作用
bool CCurl::ReInit()
{
    CloseCurl();
    return Init();
}

bool CCurl::Get(const string &visitUrl, string &htmlStr, string *errStr)
{
    if (!m_pCurl) return false;

    CURLcode code;

    Reset();
    htmlStr.clear();
    curl_easy_setopt(m_pCurl, CURLOPT_URL, visitUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEJAR, m_strCookiesFileName.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, m_strCookiesFileName.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteToMem);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &htmlStr);
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pConnectionList);
    curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, m_UserAgent);
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CURL_GET_TIMEOUT);
    /*当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出*/
    curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");
    //debug
//    curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, CurlDebug);//打印完整的调试信息
//    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);//打印调试信息
//    curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 1);//打印http头

    //proxy
    #if 0
    curl_easy_setopt(m_pCurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
    curl_easy_setopt(m_pCurl, CURLOPT_PROXY, "socks5://127.0.0.1");
    curl_easy_setopt(m_pCurl, CURLOPT_PROXYPORT, 10000L);
//    curl_easy_setopt(m_pCurl, CURLOPT_PROXY, "http://127.0.0.1:10000");
//    curl_easy_setopt(m_pCurl, CURLOPT_PROXYUSERPWD, "user_name:password");
    #endif

    code = curl_easy_perform(m_pCurl);
    if (CURLE_OK != code)
    {
        fprintf(stderr, "curl_easy_perform: %s\n", curl_easy_strerror(code));
		if (errStr)
		{
			*errStr = curl_easy_strerror(code);
		}
		return false;
    }

    return true;
}

bool CCurl::Get(const string &visitUrl, FILE *fp, string *errStr)
{
    if (!m_pCurl) return false;

	CURLcode code;

    Reset();
    curl_easy_setopt(m_pCurl, CURLOPT_URL, visitUrl.c_str());
//    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
//    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
//    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEJAR, m_strCookiesFileName.c_str());
//    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, m_strCookiesFileName.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteToFile);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pConnectionList);
    curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, m_UserAgent);
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CURL_GET_TIMEOUT);
    curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");
//    curl_easy_setopt(m_pCurl, CURLOPT_REFERER, "http://w151.hg3088.com/app/member/");
//    curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, CurlDebug);//打印完整的调试信息
//    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);//打印调试信息
	#if 0
	curl_easy_setopt(m_pCurl, CURLOPT_PROXYUSERPWD, "user_name:password");
	#endif

	code = curl_easy_perform(m_pCurl);
	if (CURLE_OK != code)
	{
		fprintf(stderr, "curl_easy_perform: %s\n", curl_easy_strerror(code));
		if (errStr)
		{
			*errStr = curl_easy_strerror(code);
		}
		return false;
	}

	return true;
}

bool CCurl::Post(const string &actionUrl, const string &postData, string &htmlStr, string *errStr)
{
    if (!m_pCurl) return false;

	CURLcode code;

    Reset();
    htmlStr.clear();
    curl_easy_setopt(m_pCurl, CURLOPT_URL, actionUrl.c_str());
//    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
//    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
//    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEJAR, m_strCookiesFileName.c_str());
//    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, m_strCookiesFileName.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteToMem);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &htmlStr);
    curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_POST, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pConnectionList);
    curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, m_UserAgent);
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, CURL_POST_TIMEOUT);
    curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");
//    curl_easy_setopt(m_pCurl, CURLOPT_REFERER, "http://hg3088.com/app/member/FT_future/body_var.php?rtype=re&uid=2c97324m10220238l44383795&langx=zh-cn&mtype=4&page_no=0");
//    curl_easy_setopt(m_pCurl, CURLOPT_DEBUGFUNCTION, CurlDebug);//打印完整的调试信息
//    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);//打印调试信息
	#if 0
	curl_easy_setopt(m_pCurl, CURLOPT_PROXYUSERPWD, "user_name:password");
	#endif

	code = curl_easy_perform(m_pCurl);
	if (CURLE_OK != code)
	{
		fprintf(stderr, "curl_easy_perform: %s\n", curl_easy_strerror(code));
		if (errStr)
		{
			errStr->clear();
			errStr->append(curl_easy_strerror(code));
		}
		return false;
	}

	return true;
}

void CCurl::Reset()
{
    if (m_pCurl)
    {
    	curl_easy_reset(m_pCurl);
    }
}

void CCurl::CloseCurl()
{
    if (m_pCurl)
    {
        curl_easy_cleanup(m_pCurl);
        m_pCurl = NULL;
    }
    if (m_pConnectionList)
    {
        curl_slist_free_all(m_pConnectionList);
        m_pConnectionList = NULL;
    }
}

void CCurl::SetCookieFileName(const string &filename)
{
    //directory
    CPublic::CreateDir(COOKIE_DIR);

    //file
    m_strCookiesFileName = COOKIE_DIR + filename;
    m_strCookieName = filename;//记录文件名称

	//清空或删除文件
	//验证用户的主帐号不删除cookie
	if ('_' != filename[0])
    {
        truncate(m_strCookiesFileName.c_str(), 0);
    }
}

string CCurl::GetCookieFileName()
{
    return m_strCookiesFileName;
}

void CCurl::RemoveCookieFile()
{
    //验证用户的主帐号不删除cookie
    if (!m_strCookieName.empty() && '_' != m_strCookieName[0])
    {
        CPublic::RemoveFile(m_strCookiesFileName);
    }
}

bool CCurl::GlobalInit(long flags)
{
	CURLcode code = curl_global_init(flags);
	if (CURLE_OK != code)
	{
		fprintf(stderr, "curl_global_init: %s\n", curl_easy_strerror(code));
		return false;
	}

	return true;
}

void CCurl::GlobalCleanup()
{
	curl_global_cleanup();
}

size_t CCurl::WriteToMem(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string *str = static_cast<string *>(userdata);
	str->append((char *)ptr, size*nmemb);
	return size*nmemb;
}

size_t CCurl::WriteToFile(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return fwrite(ptr, size, nmemb, (FILE *)userdata);
}

int CCurl::CurlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)
{
    if(itype == CURLINFO_TEXT)
    {
        printf("[TEXT]: %s\n", pData);
    }
    else if(itype == CURLINFO_HEADER_IN)
    {
        printf("[HEADER_IN]: %s\n", pData);
    }
    else if(itype == CURLINFO_HEADER_OUT)
    {
        printf("[HEADER_OUT]: %s\n", pData);
    }
    else if(itype == CURLINFO_DATA_IN)
    {
        printf("[DATA_IN]: %s\n", pData);
    }
    else if(itype == CURLINFO_DATA_OUT)
    {
        printf("[DATA_OUT]: %s\n", pData);
    }
    return 0;
}

int CCurl::HexToI(const char *s)
{
    int ch, value;

    ch = s[0];
    if (isupper(ch))
    {
        ch = tolower(ch);
    }
    value = ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10)) * 16;

    ch = s[1];
    if (isupper(ch))
    {
        ch = tolower(ch);
    }
    value += (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10);

    return value;
}

string CCurl::UrlEncode(const string &src)
{
    char const *in_str = src.c_str();
    int in_str_len = strlen(in_str);
    string out_str;
    register unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    unsigned char hexchars[] = "0123456789ABCDEF";

    from = (unsigned char *)in_str;
    end = (unsigned char *)in_str + in_str_len;
    start = to = (unsigned char *) malloc(3*in_str_len+1);

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
        } else if ((c < '0' && c != '-' && c != '.') ||
            (c < 'A' && c > '9') ||
            (c > 'Z' && c < 'a' && c != '_') ||
            (c > 'z')) {
                to[0] = '%';
                to[1] = hexchars[c >> 4];
                to[2] = hexchars[c & 15];
                to += 3;
        } else {
            *to++ = c;
        }
    }
    *to = 0;

    out_str = (char *) start;
    free(start);
    return out_str;
}

string CCurl::UrlDecode(const string &src)
{
    char const *in_str = src.c_str();
    int in_str_len = strlen(in_str);
    string out_str;
    char *str;

    str = strdup(in_str);
    char *dest = str;
    char *data = str;

    while (in_str_len--) {
        if (*data == '+') {
            *dest = ' ';
        }
        else if (*data == '%' && in_str_len >= 2 && isxdigit((int) *(data + 1))
            && isxdigit((int) *(data + 2))) {
                *dest = (char) HexToI(data + 1);
                data += 2;
                in_str_len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    out_str = str;
    free(str);
    return out_str;
}
