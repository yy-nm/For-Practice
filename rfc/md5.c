


// accord to rfc 1321 implements md5
// author: mardyu<michealyxd@hotmail.com>

#include <string.h>

#define LEN_MODER 0x40
#define LEN_MASK (LEN_MODER - 1)

#define LEN_LEFT 56

#define WORD_A 0x67452301
#define WORD_B 0xefcdab89
#define WORD_C 0x98badcfe
#define WORD_D 0x10325476

#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G(X, Y, Z) (((X) & (Z)) | ((~(Z)) & (Y)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | (~(Z))))

unsigned int T[] =
{ 0,
0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x2441453, 0xD8A1E681, 0xE7D3FBC8,
0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x4881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
};

#define ROTATING_SHIFT(x, s) (((x) << (s)) | ((x) >> (32 - (s))))

#define ROUND_1(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + F(b, c, d) + x[k] + T[i]), s)

#define ROUND_2(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + G(b, c, d) + x[k] + T[i]), s)

#define ROUND_3(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + H(b, c, d) + x[k] + T[i]), s)

#define ROUND_4(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + I(b, c, d) + x[k] + T[i]), s)


static void _append_padding(
	const char *input, const int len_input
	, char *pad, int len_padding
	, int *len_in
	, int *len_pad
	)
{
	int left_bit = len_input & LEN_MASK;
	*len_in = len_input - left_bit;
	int padding_bit = left_bit > LEN_LEFT ? LEN_MODER + LEN_LEFT - left_bit : LEN_LEFT - left_bit;

	for (int i = 0; i < len_input - *len_in; i++) {
		pad[i] = input[*len_in + i];
	}
	
	int index_start = len_input - *len_in;
	for (int i = 0; i < padding_bit; i++) {
		if (0 == i)
			pad[index_start + i] = 0x80;
		else
			pad[index_start + i] = 0;
	}
	*len_pad = padding_bit + left_bit;
}

static void _append_length(char *pad, unsigned long long length)
{
	length <<= 3;
	unsigned int low = length & 0xffffffff;
	unsigned int high = length >> 32;

	int *pad_length = (int *)pad;
	*pad_length = low;
	pad_length++;
	*pad_length = high;
}

static void _process(
	const char *input, const int len_input
	, const char *padding, const int len_pad
	, int *A, int *B, int *C, int *D)
{
	unsigned int a = WORD_A;
	unsigned int b = WORD_B;
	unsigned int c = WORD_C;
	unsigned int d = WORD_D;

	unsigned int X[16];
	unsigned int *in = (unsigned int *)input;
	for (int i = 0; (i + 1) * LEN_MODER <= len_input; i += LEN_MODER) {
		unsigned int aa = a;
		unsigned int bb = b;
		unsigned int cc = c;
		unsigned int dd = d;

		in = (unsigned int *)(input + i);
		for (int j = 0; j < 16; j++)
			X[j] = in[j];

		ROUND_1(a, b, c, d,		0,	7,		1,	X);
		ROUND_1(d, a, b, c,		1,	12,		2,	X);
		ROUND_1(c, d, a, b,		2,	17,		3,	X);
		ROUND_1(b, c, d, a,		3,	22,		4,	X);
		ROUND_1(a, b, c, d,		4,	7,		5,	X);
		ROUND_1(d, a, b, c,		5,	12,		6,	X);
		ROUND_1(c, d, a, b,		6,	17,		7,	X);
		ROUND_1(b, c, d, a,		7,	22,		8,	X);
		ROUND_1(a, b, c, d,		8,	7,		9,	X);
		ROUND_1(d, a, b, c,		9,	12,		10,	X);
		ROUND_1(c, d, a, b,		10,	17,		11,	X);
		ROUND_1(b, c, d, a,		11,	22,		12,	X);
		ROUND_1(a, b, c, d,		12,	7,		13,	X);
		ROUND_1(d, a, b, c,		13,	12,		14,	X);
		ROUND_1(c, d, a, b,		14,	17,		15,	X);
		ROUND_1(b, c, d, a,		15,	22,		16,	X);

		ROUND_2(a, b, c, d, 1, 5, 17, X);
		ROUND_2(d, a, b, c, 6, 9, 18, X);
		ROUND_2(c, d, a, b, 11, 14, 19, X);
		ROUND_2(b, c, d, a, 0, 20, 20, X);
		ROUND_2(a, b, c, d, 5, 5, 21, X);
		ROUND_2(d, a, b, c, 10, 9, 22, X);
		ROUND_2(c, d, a, b, 15, 14, 23, X);
		ROUND_2(b, c, d, a, 4, 20, 24, X);
		ROUND_2(a, b, c, d, 9, 5, 25, X);
		ROUND_2(d, a, b, c, 14, 9, 26, X);
		ROUND_2(c, d, a, b, 3, 14, 27, X);
		ROUND_2(b, c, d, a, 8, 20, 28, X);
		ROUND_2(a, b, c, d, 13, 5, 29, X);
		ROUND_2(d, a, b, c, 2, 9, 30, X);
		ROUND_2(c, d, a, b, 7, 14, 31, X);
		ROUND_2(b, c, d, a, 12, 20, 32, X);

		ROUND_3(a, b, c, d, 5, 4, 33, X);
		ROUND_3(d, a, b, c, 8, 11, 34, X);
		ROUND_3(c, d, a, b, 11, 16, 35, X);
		ROUND_3(b, c, d, a, 14, 23, 36, X);
		ROUND_3(a, b, c, d, 1, 4, 37, X);
		ROUND_3(d, a, b, c, 4, 11, 38, X);
		ROUND_3(c, d, a, b, 7, 16, 39, X);
		ROUND_3(b, c, d, a, 10, 23, 40, X);
		ROUND_3(a, b, c, d, 13, 4, 41, X);
		ROUND_3(d, a, b, c, 0, 11, 42, X);
		ROUND_3(c, d, a, b, 3, 16, 43, X);
		ROUND_3(b, c, d, a, 6, 23, 44, X);
		ROUND_3(a, b, c, d, 9, 4, 45, X);
		ROUND_3(d, a, b, c, 12, 11, 46, X);
		ROUND_3(c, d, a, b, 15, 16, 47, X);
		ROUND_3(b, c, d, a, 2, 23, 48, X);

		ROUND_4(a, b, c, d, 0, 6, 49, X);
		ROUND_4(d, a, b, c, 7, 10, 50, X);
		ROUND_4(c, d, a, b, 14, 15, 51, X);
		ROUND_4(b, c, d, a, 5, 21, 52, X);
		ROUND_4(a, b, c, d, 12, 6, 53, X);
		ROUND_4(d, a, b, c, 3, 10, 54, X);
		ROUND_4(c, d, a, b, 10, 15, 55, X);
		ROUND_4(b, c, d, a, 1, 21, 56, X);
		ROUND_4(a, b, c, d, 8, 6, 57, X);
		ROUND_4(d, a, b, c, 15, 10, 58, X);
		ROUND_4(c, d, a, b, 6, 15, 59, X);
		ROUND_4(b, c, d, a, 13, 21, 60, X);
		ROUND_4(a, b, c, d, 4, 6, 61, X);
		ROUND_4(d, a, b, c, 11, 10, 62, X);
		ROUND_4(c, d, a, b, 2, 15, 63, X);
		ROUND_4(b, c, d, a, 9, 21, 64, X);

		a += aa;
		b += bb;
		c += cc;
		d += dd;
	}
	
	for (int i = 0; (i + 1) * LEN_MODER <= len_pad; i += LEN_MODER) {
		int aa = a;
		int bb = b;
		int cc = c;
		int dd = d;

		in = (unsigned int *)(padding + i);
		for (int j = 0; j < 16; j++)
			X[j] = in[j];

		ROUND_1(a, b, c, d, 0, 7, 1, X);
		ROUND_1(d, a, b, c, 1, 12, 2, X);
		ROUND_1(c, d, a, b, 2, 17, 3, X);
		ROUND_1(b, c, d, a, 3, 22, 4, X);
		ROUND_1(a, b, c, d, 4, 7, 5, X);
		ROUND_1(d, a, b, c, 5, 12, 6, X);
		ROUND_1(c, d, a, b, 6, 17, 7, X);
		ROUND_1(b, c, d, a, 7, 22, 8, X);
		ROUND_1(a, b, c, d, 8, 7, 9, X);
		ROUND_1(d, a, b, c, 9, 12, 10, X);
		ROUND_1(c, d, a, b, 10, 17, 11, X);
		ROUND_1(b, c, d, a, 11, 22, 12, X);
		ROUND_1(a, b, c, d, 12, 7, 13, X);
		ROUND_1(d, a, b, c, 13, 12, 14, X);
		ROUND_1(c, d, a, b, 14, 17, 15, X);
		ROUND_1(b, c, d, a, 15, 22, 16, X);

		ROUND_2(a, b, c, d, 1, 5, 17, X);
		ROUND_2(d, a, b, c, 6, 9, 18, X);
		ROUND_2(c, d, a, b, 11, 14, 19, X);
		ROUND_2(b, c, d, a, 0, 20, 20, X);
		ROUND_2(a, b, c, d, 5, 5, 21, X);
		ROUND_2(d, a, b, c, 10, 9, 22, X);
		ROUND_2(c, d, a, b, 15, 14, 23, X);
		ROUND_2(b, c, d, a, 4, 20, 24, X);
		ROUND_2(a, b, c, d, 9, 5, 25, X);
		ROUND_2(d, a, b, c, 14, 9, 26, X);
		ROUND_2(c, d, a, b, 3, 14, 27, X);
		ROUND_2(b, c, d, a, 8, 20, 28, X);
		ROUND_2(a, b, c, d, 13, 5, 29, X);
		ROUND_2(d, a, b, c, 2, 9, 30, X);
		ROUND_2(c, d, a, b, 7, 14, 31, X);
		ROUND_2(b, c, d, a, 12, 20, 32, X);

		ROUND_3(a, b, c, d, 5, 4, 33, X);
		ROUND_3(d, a, b, c, 8, 11, 34, X);
		ROUND_3(c, d, a, b, 11, 16, 35, X);
		ROUND_3(b, c, d, a, 14, 23, 36, X);
		ROUND_3(a, b, c, d, 1, 4, 37, X);
		ROUND_3(d, a, b, c, 4, 11, 38, X);
		ROUND_3(c, d, a, b, 7, 16, 39, X);
		ROUND_3(b, c, d, a, 10, 23, 40, X);
		ROUND_3(a, b, c, d, 13, 4, 41, X);
		ROUND_3(d, a, b, c, 0, 11, 42, X);
		ROUND_3(c, d, a, b, 3, 16, 43, X);
		ROUND_3(b, c, d, a, 6, 23, 44, X);
		ROUND_3(a, b, c, d, 9, 4, 45, X);
		ROUND_3(d, a, b, c, 12, 11, 46, X);
		ROUND_3(c, d, a, b, 15, 16, 47, X);
		ROUND_3(b, c, d, a, 2, 23, 48, X);

		ROUND_4(a, b, c, d, 0, 6, 49, X);
		ROUND_4(d, a, b, c, 7, 10, 50, X);
		ROUND_4(c, d, a, b, 14, 15, 51, X);
		ROUND_4(b, c, d, a, 5, 21, 52, X);
		ROUND_4(a, b, c, d, 12, 6, 53, X);
		ROUND_4(d, a, b, c, 3, 10, 54, X);
		ROUND_4(c, d, a, b, 10, 15, 55, X);
		ROUND_4(b, c, d, a, 1, 21, 56, X);
		ROUND_4(a, b, c, d, 8, 6, 57, X);
		ROUND_4(d, a, b, c, 15, 10, 58, X);
		ROUND_4(c, d, a, b, 6, 15, 59, X);
		ROUND_4(b, c, d, a, 13, 21, 60, X);
		ROUND_4(a, b, c, d, 4, 6, 61, X);
		ROUND_4(d, a, b, c, 11, 10, 62, X);
		ROUND_4(c, d, a, b, 2, 15, 63, X);
		ROUND_4(b, c, d, a, 9, 21, 64, X);

		a += aa;
		b += bb;
		c += cc;
		d += dd;
	}

	*A = a;
	*B = b;
	*C = c;
	*D = d;
}



int md5(const char *input, const int len_input, char *output, const int len_output)
{
	if (len_output < 16)
		return -1;

	char pad[LEN_MODER * 2];
	int len_pad = LEN_MODER * 2;
	memset(pad, 0, len_pad);
	int len_in = len_input;

	char *result = output;
	_append_padding(input, len_in, pad, len_pad, &len_in, &len_pad);
	_append_length(pad + len_pad, len_input);
	_process(input, len_in, pad, len_pad > LEN_MODER ? 2 * LEN_MODER : LEN_MODER
		, result, result + 4, result + 8, result + 12);

	return 0;
}

int tohex(const char *input, const int len_input, char *output, const int len_output)
{
	if (len_output < len_input * 2)
		return -1;

	char low;
	char high;
	for (int i = 0; i < len_input; i++) {
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