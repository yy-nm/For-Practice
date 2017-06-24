
/*
* getiplist.c: parse hostname to ip, support ipv4 and ipv6.
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

#include "getiplist.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


static bool InitWindowsSocket()
{
#ifdef _WIN32

	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData = { 0 };
	// WinSock init
	int ret = WSAStartup(wVersionRequested, &wsaData);

	if (ret != 0)
	{
		//WSAStartup() failed!
		return false;
	}

	// Confirm the WinSock DLL version 2.2
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		//Invalid WinSock version
		return false;
	}

	//WinSock init success!
#endif
	return true;
}

static void ClearWindowsSocket()
{
#ifdef _WIN32
	WSACleanup();
#endif
}


int get_ip_via_hostname(const char *host, char **ips[], int *count_of_ips)
{
	return get_ip_via_hostname_with_alloctor(host, ips, count_of_ips, malloc, free);
}

int get_ip_via_hostname_with_alloctor(const char *host, char **ips[], int *count_of_ips, alloctor alloc, freer fr)
{
	if (host == NULL || ips == NULL || count_of_ips == NULL || alloc == NULL || fr == NULL)
		return ERR_ARGS;
	
	struct addrinfo *result = NULL;
	char **ip = NULL;
	*ips = ip;
	*count_of_ips = 0;

	int ret = get_ip_address_via_hostname(host, &result);
	if (ret != OK)
		return ret;

	if (result == NULL)
	{
		return OK;
	}

	int count = 0;
	struct addrinfo *paddr;
	for (paddr = result; paddr != NULL; paddr = paddr->ai_next)
		count++;
	ip = (char **)alloc(count * sizeof(*ip));
	if (*ip == NULL)
		return ERR_ALLOCTOR;

	char buff[NI_MAXHOST];
	int i = 0;
	for (paddr = result; paddr != NULL; paddr = paddr->ai_next)
	{
		memset(buff, 0, sizeof(buff));
		ret = getnameinfo(paddr->ai_addr, paddr->ai_addrlen, buff, sizeof(buff), NULL, 0, NI_NUMERICHOST);
		if (ret)
		{
			// for free 
			while (--i >= 0)
			{
				fr(ip[i]);
			}
			fr(ip);
			free_ip_address(result);
			return ERR_GET_IP;
		}
		ip[i] = alloc(strlen(buff) + 1);
		if (ip[i] == NULL)
		{
			// for free 
			while (--i >= 0)
			{
				fr(ip[i]);
			}
			fr(ip);
			free_ip_address(result);
			return ERR_ALLOCTOR;
		}
		memcpy(ip[i], buff, strlen(buff));
		ip[i][strlen(buff)] = '\0';
		i++;
	}
	*ips = ip;
	*count_of_ips = count;
	free_ip_address(result);
	return OK;
}

int get_ip_address_via_hostname(const char *host, struct addrinfo **presult)
{
	if (host == NULL || presult == NULL)
		return ERR_ARGS;

	InitWindowsSocket();

	struct addrinfo info;
	memset(&info, 0, sizeof(info));
	info.ai_family = AF_UNSPEC; // support ipv4 and ipv6
	info.ai_socktype = SOCK_DGRAM;

	int ret = getaddrinfo(host, NULL, &info, presult);
	if (ret)
	{
		return ERR_PARSE_HOST;
	}

	return OK;
}

void free_ip_address(struct addrinfo *addr)
{
	if (addr != NULL)
		freeaddrinfo(addr);
}