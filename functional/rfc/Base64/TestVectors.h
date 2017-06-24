
#ifndef _TESTVECTORS_H_
#define _TESTVECTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

	const int TestVectorsCount = 7;
	const char *TestVectors[] = {
		"",
		"f",
		"fo",
		"foo",
		"foob",
		"fooba",
		"foobar",
	};
	const char *TestVectorsResult[] = {
		"",
		"Zg==",
		"Zm8=",
		"Zm9v",
		"Zm9vYg==",
		"Zm9vYmE=",
		"Zm9vYmFy",
	};

#ifdef __cplusplus
}
#endif

#endif
