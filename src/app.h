#ifndef APP_H
#define APP_H

#include <string>

#include <curl/curl.h>
#include <pcre.h>

#include "config.h"

class App
{
public:
	App();
	virtual ~App();

	inline std::string lastError() const
	{ return _error;}

	inline std::string ip() const
	{ return _ip; }

	bool setConfig(const Config &cfg);

	bool refreshIP();

private:
	static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);

private:
	Config _config;
	std::string _error;

	CURL *_curl;
	char *_curl_error;

	pcre *_re;
	pcre_extra *_re_extra;

	std::string _ip;
};

#endif