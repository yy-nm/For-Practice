


CC ?= gcc -std=gnu99
SHARED := -fPIC --shared
LIB ?= libs
OBJ ?= obj
CFLAGS = -O2 -Wall
AR = ar rcs
RM = rm -rf

all: mmap
.PHONY: test clean libdynamic libstatic lib

$(LIB):
	mkdir $(LIB)

$(OBJ):
	mkdir $(OBJ)

mmap:
	$(CC) $(CFLAGS) -o sha224 main.linux.mmap.c sha224.c tohex.c

array:
	$(CC) $(CFLAGS) -o sha224 main.linux.array.c sha224.c tohex.c

lib: libdynamic libstatic

libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libsha224.so sha224.c

libstatic: | $(LIB) $(OBJ)
	$(CC) $(CFLAGS) -c -o $(OBJ)/sha224.o sha224.c
	$(AR) $(LIB)/libsha224.a $(OBJ)/sha224.o

test:
	$(CC) $(CFLAGS) -o sha224-test main.c sha224.c tohex.c

clean:
	$(RM) sha224-test sha224 $(LIB)/* $(OBJ)/*
