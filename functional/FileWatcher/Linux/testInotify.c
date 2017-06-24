/*
* testInotify.c: a demo for use inotify to watch the change of file in linux
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/inotify.h>

void GetMask(int32_t mask);

int main(int argc, char **args)
{
	if (argc <= 1)
	{
		printf("no folder to watch!\n");
		return 1;
	}

	int fd = inotify_init();
	if (-1 == fd) {
		perror("init error\n");
		return 2;
	}
	int i;
	int ret;
	int32_t mask = IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE
		| IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF
		| IN_MOVED_FROM | IN_MOVED_TO | IN_OPEN;
	for (i = 0; i < argc; i++) {
		ret = inotify_add_watch(fd, args[i], IN_ALL_EVENTS);
		if (-1 == ret) {
			perror("add_watch error\n");
			return 3;
		}
	}

	struct inotify_event *ev;
	char buff[1024];
	while (true) {
		memset(buff, 0, sizeof(buff));
		ret = read(fd, buff, sizeof(buff));
		if (-1 == ret) {
			perror("read error\n");
			return 4;
		}
		buff[ret] = '\0';
		ev = (struct inotify_event *)&buff;
		printf("filename: %d\n", ev->len);
		GetMask(ev->mask);
		printf("watch fd: %d, mask: %d, cookie: %d, name: %s\n"
				, ev->wd, ev->mask, ev->cookie, ev->name);
	}

	return 0;
}

void GetMask(int32_t mask)
{
	if (mask & IN_ACCESS) {
		printf("IN_ACCESS\n");
	}
	if (mask & IN_ATTRIB) {
		printf("IN_ATTRIB\n");
	}
	if (mask & IN_CLOSE_WRITE) {
		printf("IN_CLOSE_WRITE\n");
	}
	if (mask & IN_CLOSE_NOWRITE) {
		printf("IN_CLOSE_NOWRITE\n");
	}
	if (mask & IN_CREATE) {
		printf("IN_CREATE\n");
	}
	if (mask & IN_DELETE) {
		printf("IN_DELETE\n");
	}
	if (mask & IN_DELETE_SELF) {
		printf("IN_DELETE_SELF\n");
	}
	if (mask & IN_MODIFY) {
		printf("IN_MODIFY\n");
	}
	if (mask & IN_MOVE_SELF) {
		printf("IN_MOVE_SELF\n");
	}
	if (mask & IN_MOVED_FROM) {
		printf("IN_MOVED_FROM\n");
	}
	if (mask & IN_MOVED_TO) {
		printf("IN_MOVED_TO\n");
	}
	if (mask & IN_OPEN) {
		printf("IN_OPEN\n");
	}
}
