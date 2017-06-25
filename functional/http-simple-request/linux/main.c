

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "httprequest.h"


size_t recv_data(void *buffer, size_t sz)
{
	char *data = (char *)malloc(sz + 1);
	if (NULL == data)
		return sz;
	memcpy(data, buffer, sz);
	data[sz] = '\0';
	printf("recv data: %s", data);
	free(data);

	return sz;
}

int main(void)
{
	const char *test_curl_get = "http://httpbin.org/get?hehe=1";
	const char *test_curl_post = "http://httpbin.org/post";
	const char *test_curl_post_args = "hrhr=4&lala=5";
	int ret;

	ret = simple_http_request_getdata("https://www.baidu.com/", recv_data);
	printf("simple_http_request_getdata ret: %d\n", ret);

	ret = simple_http_request_getdata(test_curl_get, recv_data);
	printf("simple_http_request_getdata ret: %d\n", ret);

	ret = simple_http_request_post_getdata(test_curl_post, test_curl_post_args, recv_data);
	printf("simple_http_request_post_getdata ret: %d\n", ret);	

	return 0;
}