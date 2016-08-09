

all: mmap readblock
.PHONY: test clean

mmap:
	gcc -o sha1-mmap -O3 main.linux.mmap.c sha1.c 

readblock: 
	gcc -o sha1-readblock -O3 main.linux.read.c sha1.c

test:
	gcc -o sha1-test -O3 main.c sha1.c 

clean:
	rm -rf sha1-test sha1-mmap sha1-readblock
