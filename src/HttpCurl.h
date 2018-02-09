
#pragma once
#include <string>
#include <curl/curl.h>
#include <pthread.h>
#include <openssl/crypto.h>
static pthread_mutex_t* lockarray;

class CHttpCurl
{
	public:
		CHttpCurl()
		{
			m_curlCode = CURLE_OK;
			m_extra = NULL;
		}
		virtual ~CHttpCurl()
		{
		}
	public:
		static void lock_callback(int mode, int type, char *file, int line)
		{
			(void)file;
			(void)line;
			if (mode & CRYPTO_LOCK) {
				pthread_mutex_lock(&(lockarray[type]));
			}
			else {
				pthread_mutex_unlock(&(lockarray[type]));
			}
		}
		static unsigned long thread_id(void)
		{
			unsigned long ret;
			ret = (unsigned long)pthread_self();
			return(ret);
		}
		static void init_locks()
		{
			lockarray = (pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
			for(int i = 0; i < CRYPTO_num_locks(); i++) 
			{
				pthread_mutex_init(&(lockarray[i]),NULL);
			}
			CRYPTO_set_id_callback((unsigned long (*)())thread_id);
			CRYPTO_set_locking_callback((void (*)(int, int, const char*, int))lock_callback);
		}
		static void kill_locks()
		{
			CRYPTO_set_locking_callback(NULL);
			for(int i = 0; i < CRYPTO_num_locks(); i++)
			{
				pthread_mutex_destroy(&(lockarray[i]));
			}
			OPENSSL_free(lockarray);
		}
		static size_t rev_data(void *ptr, size_t size, size_t nmemb, void *stream)
		{
			CHttpCurl* pHttp = (CHttpCurl*)stream;
			size_t len = size * nmemb;
			std::string response;
			if(ptr && len)
			{
				response.assign((char*)ptr,len);
			}
			pHttp->Response(response);
			return size * nmemb;
		}
		static CURLcode CurlGlobalInit()
		{
			CURLcode m_curlCode = curl_global_init(CURL_GLOBAL_ALL);
			init_locks();
			return m_curlCode;
		}
		static void CurlGlobalCleanUp()
		{
			kill_locks();
			curl_global_cleanup();
		}
		CURLcode Post(std::string& url,std::string& fileds);
		CURLcode Get(std::string& url);
		//return CURLcode 
		CURLcode GetCURLcode();
		void SetExtra(void* extra);
		void* GetExtra();
		virtual void Response(std::string& response) = 0;
	private:
		CURLcode m_curlCode;
		CURL* m_curl;
		void* m_extra;
};
