

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "md5.h"




int tohex(const char *input, const int len_input, char *output, const int len_output)
{
	if (len_output < len_input * 2)
		return -1;

	char low;
	char high;
	int i;
	for (i = 0; i < len_input; i++) {
		high = 0xf & input[i];
		low = (0xf0 & input[i]) >> 4;
		if (low >= 10)
			output[i * 2] = 'a' + low - 10;
		else
			output[i * 2] = '0' + low;
		if (high >= 10)
			output[i * 2 + 1] = 'a' + high - 10;
		else
			output[i * 2 + 1] = '0' + high;
	}

	return 0;
}

#define LEN_BUF 4 * 1024 * 1024
int main(int argc, char **argv) {

	int i = 0;
	int fd = 0;
	long sz;
	struct stat st;
	char buf[LEN_BUF];
	char result[16];
	char show[33];
	long off;
	int len = LEN_BUF;
	/*struct sha1_context c;*/
	struct md5_context c;
	for (i = 1; i < argc; i++) {
		fd = open(argv[i], O_RDONLY);
		if (fd < 0 ) {
			perror("open file error");
			continue;
		}
		fstat(fd, &st);
		sz = st.st_size;
		off = 0;
		/*sha1_init(&c);*/
		md5_init(&c);
		sz = len;
		while (sz == len) {
			sz = read(fd, buf, len);
			if (sz < 0) {
				perror("read file error");
				goto err;
			}
			/*sha1_update(&c, buf, sz);*/
			md5_update(&c, buf, sz);
		}

		/*sha1_final(&c, result, sizeof(result));*/
		md5_final(&c, result, sizeof(result));
		tohex(result, sizeof(result), show, sizeof(show));
		show[32] = '\0';
		printf("%s\t%s\n", show, argv[i]);
err:
		close(fd);
	}
	
	return 0;
}
