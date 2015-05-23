#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
	Config()
	{
		_url = "http://canihazip.com/s";
		_regex = "(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})";
	}

	inline std::string url()
	{ return _url; }

	inline std::string regex()
	{ return _regex; }

private:
	std::string _url;
	std::string _regex;
};

#endif