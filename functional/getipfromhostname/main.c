

#include <stdio.h>
#include <stdlib.h>

#include "getiplist.h"


int main()
{
	char **ips = NULL;
	int count = 0;
	int i;

	int ret = get_ip_via_hostname(
		//"ipv6.test-ipv6.com"
		//"ipv4.test-ipv6.com"
		"baidu.com"
		//"google.com"
		//"ipv6.google.com"
		, &ips, &count);
	if (ret)
	{
		printf("parse fail: %d\n", ret);
	}
	else
	{
		for (i = 0; i < count; i++)
		{
			printf("%d: IP: %s\n", i + 1, ips[i]);
			free(ips[i]);
			ips[i] = NULL;
		}
		free(ips);
	}

	return 0;
}