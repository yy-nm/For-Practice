// author: Mard Yu<michealyxd@hotmail.com>
// accord to rfc4648

#include <string.h>

static char const * const base_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static char const * const base_str_url = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
static char const pad = '=';

#define BASE64_UNIT_LEN 4
#define BASE64_UNIT_LEN_MASK (BASE64_UNIT_LEN - 1)
#define BASE64_CEIL(num) ((num + BASE64_UNIT_LEN_MASK) & ~BASE64_UNIT_LEN_MASK)

int base64_encode(const char *in, const int len_in, char *out, const int len_out)
{
	return _base64_encode(in, len_in, out, len_out, base_str);
}

int base64_url_encode(const char *in, const int len_in, char *out, const int len_out)
{
	return _base64_encode(in, len_in, out, len_out, base_str_url);
}

int base64_encode_length(const int len)
{
	return BASE64_CEIL((len * 8) / 6);
}

static int _base64_encode(const char *in, const int len_in
							, char *out, const int len_out
							, const char *base_ab)
{
	if (base64_encode_length(len_in) > len_out)
		return -1;
	const unsigned char *i = in;
	unsigned char *o = out;
	int len_i = len_in;

	unsigned int base = 0;
	int len = 0;
	while (len_i >= 3) {
		base = 0;
		base |= (*i++) << 16;
		base |= (*i++) << 8;
		base |= *i++;
		o[len++] = base_ab[(base >> 18) & 0x3f];
		o[len++] = base_ab[(base >> 12) & 0x3f];
		o[len++] = base_ab[(base >> 6) & 0x3f];
		o[len++] = base_ab[base & 0x3f];

		len_i -= 3;
	}
	if (len_i == 0)
		return len;

	base = 0;
	for (int j = 0; j < len_i; j++) {
		base <<= 8;
		base |= *i++;
	}
	for (int j = len_i; j < 3; j++) {
		base <<= 8;
		base |= 0;
	}

	for (int j = 0; j < 4; j++) {
		o[len + 3 - j] = base_ab[base & 0x3f];
		base >>= 6;
	}

	int len_pad = 4 - len_i - 1;
	for (int j = 0; j < len_pad; j++) {
		o[len + len_i + j + 1] = pad;
	}

	return len + 4;
}

int base64_decode(const char *in, const int len_in, char *out, const int len_out)
{
	return _base64_decode(in, len_in, out, len_out, base_str);
}

int base64_url_decode(const char *in, const int len_in, char *out, const int len_out)
{
	return _base64_decode(in, len_in, out, len_out, base_str_url);
}

int base64_decode_length(const int len)
{
	return (len * 6 + 7) / 8;
}

static int _base64_decode(const char *in, const int len_in
							, char *out, const int len_out
							, const char *base_ab)
{
	if (base64_decode_length(len_in) > len_out)
		return -1;
	const unsigned char *i = in;
	unsigned char *o = out;
	int len_i = len_in;

	unsigned int base = 0;
	int len = 0;
	char *loc = NULL;
	while (len_i > 4) {
		base = 0;
		for (int j = 0; j < 4; j++) {
			loc = strchr(base_ab, *i++);
			if (loc == NULL || loc - base_ab == 64)
				return -2;
			base <<= 6;
			base |= (loc - base_ab);
		}

		o[len++] = (unsigned char)((base >> 16) & 0xff);
		o[len++] = (unsigned char)((base >> 8) & 0xff);
		o[len++] = (unsigned char)(base & 0xff);
		len_i -= 4;
	}

	if (0 == len_i)
		return len;

	for (int j = 0; j < len_i; j++) {
		loc = strchr(base_ab, *i++);
		if (loc == NULL)
			return -2;
		if (loc - base_ab == 64) {
			len_i = j;
			break;
		}
		base <<= 6;
		base |= (loc - base_ab);
	}
	if (0 == len_i)
		return len;

	for (int j = len_i; j < 4; j++) {
		base <<= 6;
		base |= 0;
	}

	int correction = 0;
	switch (len_i)
	{
	case 4:
	case 3:
		o[len + 2] = (unsigned char)(base & 0xff);
	case 2:
		o[len + 1] = (unsigned char)((base >> 8) & 0xff);
		correction = -1;
	case 1:
		o[len] = (unsigned char)((base >> 16) & 0xff);
		break;
	}

	return len + len_i + correction;
}
