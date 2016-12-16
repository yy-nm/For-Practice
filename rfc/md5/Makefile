

CC ?= gcc -std=gnu99
SHARED := -fPIC --shared
LIB ?= libs
OBJ ?= obj
CFLAGS = -O2 -Wall
AR = ar rcs
RM = rm -rf

all: mmap
.PHONY: test clean libdynamic libstatic lib readblock array

$(LIB):
	mkdir $(LIB)

$(OBJ):
	mkdir $(OBJ)

mmap:
	$(CC) $(CFLAGS) -o md5 main.linux.mmap.c md5.c

readblock:
	$(CC) $(CFLAGS) -o md5-read main.linux.read.c md5.c

array:
	$(CC) $(CFLAGS) -o md5-array main.linux.array.c md5.c

lib: libdynamic libstatic

libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libmd5.so md5.c

libstatic: | $(LIB) $(OBJ)
	$(CC) $(CFLAGS) -c -o $(OBJ)/md5.o md5.c
	$(AR) $(LIB)/libmd5.a $(OBJ)/md5.o

test:
	$(CC) $(CFLAGS) -o md5-test main.c md5.c tohex.c

clean:
	$(RM) md5-test md5 md5-read md5-array $(LIB)/* $(OBJ)/*
