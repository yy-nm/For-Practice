

/*
* getiplist.h: parse hostname to ip, support ipv4 and ipv6.
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2017 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
* 
* reference:
*	https://msdn.microsoft.com/en-us/library/windows/desktop/ms738630(v=vs.85).aspx
*	https://msdn.microsoft.com/en-us/library/windows/desktop/ms738520(v=vs.85).aspx
*	https://msdn.microsoft.com/en-us/library/windows/desktop/ms738639(v=vs.85).aspx
*	http://man7.org/linux/man-pages/man3/getnameinfo.3.html
*	http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/

#ifndef __GETIPLIST_H__


#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#elif __APPLE__
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

	enum HostResult {
		OK = 0,
		ERR_ARGS,
		ERR_PARSE_HOST,
		ERR_ALLOCTOR,
		ERR_GET_IP,
	};

	typedef void* (*alloctor)(size_t sz);
	typedef void(*freer)(void *);

	int get_ip_via_hostname(const char *host, char **ips[], int *count_of_ips);
	int get_ip_via_hostname_with_alloctor(const char *host, char **ips[], int *count_of_ips, alloctor alloc, freer fr);

	int get_ip_address_via_hostname(const char *host, struct addrinfo **presult);
	void free_ip_address(struct addrinfo *addr);

#ifdef __cplusplus
}
#endif

#endif // !__GETIPLIST_H__
