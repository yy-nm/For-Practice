

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include "sha224.h"
#include "tohex.h"


#define PAGE_SZ 4 * 1024
#define LEN_BUF 64 * PAGE_SZ
int main(int argc, char **argv) {

	int i = 0;
	int fd = 0;
	long sz;
	char buf[LEN_BUF];
	char result[28];
	char show[57];
	int len = LEN_BUF;
	struct sha224_context c;
	for (i = 1; i < argc; i++) {
		fd = open(argv[i], O_RDONLY);
		if (fd < 0 ) {
			perror("open file error");
			continue;
		}
		sha224_init(&c);
		sz = len;
		while (sz == len) {
			sz = read(fd, buf, len);
			if (sz < 0) {
				perror("read file error");
				goto err;
			}
			sha224_update(&c, buf, sz);
		}

		sha224_final(&c, result, sizeof(result));
		tohex(result, sizeof(result), show, sizeof(show));
		show[56] = '\0';
		printf("%s\t%s\n", show, argv[i]);
err:
		close(fd);
	}
	
	return 0;
}
