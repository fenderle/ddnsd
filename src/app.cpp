#include "app.h"

#include <string.h>
#include <sstream>
#include <iostream>

App::App() :
    _curl(0),
    _curl_error(0),
    _re(0),
    _re_extra(0)
{
}

App::~App()
{
    if (_curl != 0)
        curl_easy_cleanup(_curl);
    if (_curl_error != 0)
        free(_curl_error);

    if (_re != 0)
        pcre_free(_re);
    if (_re_extra != 0)
        pcre_free(_re_extra);
}

bool App::setConfig(const Config &cfg)
{
    /* copy config settings */
    _config = cfg;

    /* compile new regex, before that free old one */
    if (_re_extra != 0) {
        pcre_free(_re_extra);
        _re_extra = 0;
    }

    if (_re != 0) {
        pcre_free(_re);
        _re = 0;
    }

    /* compile and optimize regex */
    const char *error;
    int offset;

    _re = pcre_compile(_config.regex().c_str(), PCRE_MULTILINE, &error, &offset, 0);
    if (_re == 0) {
        std::stringstream s;
        s << "Failed to compile regex: " << error << " at offset " << offset;
        _error = s.str();
        return false;
    }

    _re_extra = pcre_study(_re, 0, &error);
    if (_re_extra == 0) {
        pcre_free(_re);
        _re = 0;

        std::stringstream s;
        s << "Failed to optimize regex: " << error;
        _error = s.str();
        return false;
    }

    return true;
}

bool App::refreshIP()
{
    /* alloc curl error buffer */
    if (_curl_error == 0) {
        _curl_error = reinterpret_cast<char *>(calloc(CURL_ERROR_SIZE, sizeof(char)));
        if (_curl_error == 0) {
            _error = "calloc() failed";
            return false;
        }
    }

    /* setup curl session */
    if (_curl == 0) {
        _curl = curl_easy_init();
        if (_curl == 0) {
            _error = "Failed to initialize curl session";
            return false;
        }

        curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _curl_error);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writer);
    }

    /* request the reflector page, data will be in buffer after perform */
    std::string buffer;
    CURLcode code;

    curl_easy_setopt(_curl, CURLOPT_URL, _config.url().c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);

    code = curl_easy_perform(_curl);
    if (code != CURLE_OK) {
        std::stringstream s;
        s << "Failed to retrieve reflector URL: " << _curl_error;
        _error = s.str();
        return false;
    }

    /* match regex against buffer */
    const char *str = buffer.c_str();
    unsigned int offset = 0;
    unsigned int len = strlen(str);
    int ovector[32];

    while (offset < len)
    {
        int rc = pcre_exec(_re, _re_extra, str, len, offset, 0, ovector, sizeof(ovector));
        if (rc < 0)
            break;

        if (rc > 1) {
            /* the first capture is expected to be the ip */
            _ip = std::string(str + ovector[2], ovector[3] - ovector[2]);
            break;
        }

        offset = ovector[1];
    }

    return true;
}

int App::writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
    if (buffer == 0)
        return 0;

    buffer->append(data, size * nmemb);
    return size * nmemb;
}

