

all: mmap readblock
.PHONY: test clean

mmap:
	gcc -o md5-mmap -O3 main.linux.mmap.c md5.c 

readblock: 
	gcc -o md5-readblock -O3 main.linux.read.c md5.c

test:
	gcc -o md5-test -O3 main.c md5.c tohex.c

clean:
	rm -rf md5-test md5-mmap md5-readblock
