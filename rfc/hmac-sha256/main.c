#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "hmac-sha256.h"
#include "tohex.h"
#include "testvector.h"


#define LEN_BUF 32

int main(int argc, char *argv[])
{
	char buf[LEN_BUF * 2 + 1];
	char buf_mid[LEN_BUF];
	int i;
	int ret;
	for (i = 0; i < testvector_count; i++) {
		ret = hmac_sha256(testvector_key[i], strlen(testvector_key[i]), testvector_data[i], strlen(testvector_data[i]), buf_mid, sizeof(buf_mid));
		if (ret) {
			printf("calc key: \"%s\", data: \"%s\" hmac-sha256 fail, error code: %d\n", "testvector_key[i]", "testvector_data[i]", ret);
			continue;
		}
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		if (strcmp(buf, testvector_result[i]) == 0) {
			printf("calc key: \"%s\", data: \"%s\" hmac-sha256 success, result: \"%s\"\n", "testvector_key[i]", "testvector_data[i]", buf);
		} else {
			printf("calc key: \"%s\", data: \"%s\" hmac-sha256 success, but not match right one: \n\"%s\"\n result: \n\"%s\"\n"
				   , "testvector_key[i]", "testvector_data[i]", testvector_result[i], buf);
		}
	}
	return 0;
}
