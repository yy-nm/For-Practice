#ifndef _TESTVECTORS_H_
#define _TESTVECTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

const int cTestVectorCount = 8;

const char *cTestVectorOrigin[] = {
	"Hello world",
	"Hello world",
	"Hello world",
	"Hello world",
	"Hello world",
	"Hello world",
	"svertices vertices",
	"FFS :svertices :vertices",
};

const char *cTestVectorSub[] = {
	"",
	"",
	"Hello",
	"world",
	"o wo",
	"not",
	"vertices",
	":vertices",
};

const int cTestVectorResult[] = {
	-1, -1, 0, 6, 4, -1, 1, 15
};

#ifdef __cplusplus
}
#endif

#endif // _TESTVECTORS_H_
