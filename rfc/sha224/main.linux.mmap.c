

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "sha224.h"
#include "tohex.h"


int main(int argc, char **argv) {
	int i = 0;
	int fd = 0;
	size_t sz;
	struct stat st;
	char *buf;
	char result[28];
	char show[57];
	long off;
	struct sha224_context c;
	for (i = 1; i < argc; i++) {
		fd = open(argv[i], O_RDONLY);
		if (fd < 0 ) {
			perror("open file error");
			continue;
		}
		fstat(fd, &st);
		sz = st.st_size;
		off = 0;
		sha224_init(&c);
		buf = NULL;
		buf = mmap(buf, sz, PROT_READ, MAP_SHARED, fd, off);
		if (buf == MAP_FAILED) {
			perror("read file error");
			goto err;
		}
		sha224_update(&c, buf, sz);
		sha224_final(&c, result, sizeof(result));
		tohex(result, sizeof(result), show, sizeof(show));
		show[56] = '\0';
		printf("%s\t%s\n", show, argv[i]);
err:
		munmap(buf, sz);
		close(fd);
	}
	
	return 0;
}
