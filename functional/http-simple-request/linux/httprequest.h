
/*
* httprequest.h: use curl to do simple http request action.
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2017 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*
* reference:
*	curl: https://curl.haxx.se/libcurl/
*	curl source code: "https://curl.haxx.se/dlwiz/?type=source&os=-"
*	curl exmple: https://curl.haxx.se/libcurl/c/example.html
*/


#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__

#include <stdio.h>

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#endif // _WIN32

#ifdef __cplusplus
extern "C" {
#endif

	enum SIMPLE_HTTP_REQUEST_ERROR
	{
		OK = 0,
		ERR_ARGS,
		ERR_INIT,
		ERR_REQUEST,
	};

	// return how much program recv data
	typedef size_t (*func_simple_net_recvdata)(void *buffer, size_t sz);
	typedef size_t (*func_simple_net_senddata)(void *buffer, size_t sz);

	// http get method
	int simple_http_request(const char *url);
	int simple_http_request_getdata(const char *url, func_simple_net_recvdata func);

	// http post method
	int simple_http_request_post(const char *url, const char *args);
	int simple_http_request_post_getdata(const char *url, const char *args, func_simple_net_recvdata func);


#ifdef __cplusplus
}
#endif


#endif