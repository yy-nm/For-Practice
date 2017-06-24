
#ifndef _TESTSUITE_H_
#define _TESTSUITE_H_

#ifdef __cplusplus
extern "C" {
#endif

	const int TestSuiteCount = 8;
	const char *TestSuite[] = {
		"",
		"a",
		"abc",
		"message digest",
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"The quick brown fox jumps over the lazy dog",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
	};
	const char *TestSuiteResult[] = {
		"d41d8cd98f00b204e9800998ecf8427e",
		"0cc175b9c0f1b6a831c399e269772661",
		"900150983cd24fb0d6963f7d28e17f72",
		"f96b697d7cb7938d525a2f31aaf161d0",
		"c3fcd3d76192e4007dfb496cca67e13b",
		"d174ab98d277d9f5a5611c2c9f419d9f",
		"9e107d9d372bb6826bd81d3542a419d6",
		"57edf4a22be3c955ac49da2e2107b67a",
	};

#ifdef __cplusplus
}
#endif

#endif
