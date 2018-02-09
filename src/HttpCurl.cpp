#include "HttpCurl.h"


CURLcode CHttpCurl::Post(std::string& url,std::string& fileds)
{
	m_curl = curl_easy_init();
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(m_curl, CURLOPT_URL,url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS,fileds.c_str());
	if(url.find("https://") != std::string::npos)
	{
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3L);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3L);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, rev_data);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	m_curlCode = curl_easy_perform(m_curl);
	curl_easy_cleanup(m_curl);
	return m_curlCode;
}


CURLcode CHttpCurl::Get(std::string& url)
{
	m_curl = curl_easy_init();
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(m_curl, CURLOPT_URL,url.c_str());
	if(url.find("https://") != std::string::npos)
	{
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3L);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3L);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, rev_data);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	m_curlCode = curl_easy_perform(m_curl);
	curl_easy_cleanup(m_curl);
	return m_curlCode;
}
//return CURLcode 
CURLcode CHttpCurl::GetCURLcode()
{
	return m_curlCode;
}


void CHttpCurl::SetExtra(void* extra)
{
	m_extra = extra;
}


void* CHttpCurl::GetExtra()
{
	return m_extra;
}

