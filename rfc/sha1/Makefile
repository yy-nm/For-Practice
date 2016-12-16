


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
	$(CC) $(CFLAGS) -o sha1 main.linux.mmap.c sha1.c

readblock:
	$(CC) $(CFLAGS) -o sha1-read main.linux.read.c sha1.c

array:
	$(CC) $(CFLAGS) -o sha1-array main.linux.array.c sha1.c

lib: libdynamic libstatic

libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libsha1.so sha1.c

libstatic: | $(LIB) $(OBJ)
	$(CC) $(CFLAGS) -c -o $(OBJ)/sha1.o sha1.c
	$(AR) $(LIB)/libsha1.a $(OBJ)/sha1.o

test:
	$(CC) $(CFLAGS) -o sha1-test main.c sha1.c

clean:
	$(RM) sha1-test sha1 sha1-read sha1-array $(LIB)/* $(OBJ)/*
