#ifndef _TESTVECTORS_H_
#define _TESTVECTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

const int cTestVectorCount = 11;

const char *cTestVectorOrigin[] = {
	"babcbabcabcaabcabcabcacabc",
	"Hello world",
	"Hello world",
	"Hello world",
	"Hello world",
	"Hello world",
	"Hello world",
	"svertices vertices",
	"FFS :svertices :vertices",
	"cabadccabcdab",
	"aaaaaaaaaaaaaab",
};

const char *cTestVectorSub[] = {
	"abcabcacab",
	"",
	"",
	"Hello",
	"world",
	"o wo",
	"not",
	"vertices",
	":vertices",
	"ccabcdab",
	"aaaaaaab"
};

const int cTestVectorResult[] = {
	15, -1, -1, 0, 6, 4, -1, 1, 15, 5, 7
};

#ifdef __cplusplus
}
#endif

#endif // _TESTVECTORS_H_
