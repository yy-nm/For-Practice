
/*
* httprequest.c: use curl to do simple http request action.
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


#include "httprequest.h"

#include <stdio.h>

#include "curl/curl.h"

#ifndef SIMPLE_HTTP_REQUEST_GET_TIMEOUT
#define SIMPLE_HTTP_REQUEST_GET_TIMEOUT 5
#endif // !SIMPLE_HTTP_REQUEST_GET_TIMEOUT



static size_t recv_nothing(void *buffer, size_t sz)
{
	return sz;
}

static size_t curl_writer(void *buffer, size_t sz, size_t nmemb, void *args)
{
	size_t totalsz = sz * nmemb;
	size_t nowsz = 0;
	if (NULL == args)
		return totalsz;

	func_simple_net_recvdata *func = (func_simple_net_recvdata*)args;
	if (NULL == func)
		return totalsz;

	do {
		nowsz += (*func)((char *)buffer + nowsz, totalsz - nowsz);
	} while (nowsz < totalsz);

	return totalsz;
}

int simple_http_request(const char *url)
{
	return simple_http_request_getdata(url, recv_nothing);
}

int simple_http_request_getdata(const char *url, func_simple_net_recvdata func)
{
	if (NULL == url)
		return ERR_ARGS;

	CURL *curl;
	CURLcode ret;

	curl = curl_easy_init();
	if (NULL == curl)
		return ERR_INIT;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, SIMPLE_HTTP_REQUEST_GET_TIMEOUT);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &func);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	// avoid curl send signal when timeout in multi-thread environment
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1l);

	// for debug
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (ret == CURLE_OK)
		return OK;
	return ERR_REQUEST;
}

int simple_http_request_post(const char *url, const char *args)
{
	return simple_http_request_post_getdata(url, args, recv_nothing);
}

int simple_http_request_post_getdata(const char *url, const char *args, func_simple_net_recvdata func)
{
	if (NULL == url)
		return ERR_ARGS;

	CURL *curl;
	CURLcode ret;

	curl = curl_easy_init();
	if (NULL == curl)
		return ERR_INIT;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, SIMPLE_HTTP_REQUEST_GET_TIMEOUT);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &func);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	// avoid curl send signal when timeout in multi-thread environment
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1l);

	// for debug
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	if (NULL != args)
	{
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, args);
	}

	ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (ret == CURLE_OK)
		return OK;
	return ERR_REQUEST;
}