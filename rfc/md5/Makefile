

all: mmap readblock
.PHONY: test clean libdynamic libstatic

mmap:
	gcc -Wall -o md5-mmap -O3 main.linux.mmap.c md5.c

readblock:
	gcc -Wall -o md5-readblock -O3 main.linux.read.c md5.c

libdynamic:
	gcc -shared -o libmd5.so -fPIC md5.c

libstatic:
	gcc -c -o md5.o md5.c
	ar rcs libmd5.a md5.o

test:
	gcc -Wall -o md5-test -O3 main.c md5.c tohex.c

clean:
	rm -rf md5-test md5-mmap md5-readblock libmd5.so libmd5.a md5.o
