


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
	$(CC) $(CFLAGS) -o sha512 main.linux.mmap.c sha512.c tohex.c

array:
	$(CC) $(CFLAGS) -o sha512 main.linux.array.c sha512.c tohex.c

lib: libdynamic libstatic

libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libsha512.so sha512.c

libstatic: | $(LIB) $(OBJ)
	$(CC) $(CFLAGS) -c -o $(OBJ)/sha512.o sha512.c
	$(AR) $(LIB)/libsha512.a $(OBJ)/sha512.o

test:
	$(CC) $(CFLAGS) -o sha512-test main.c sha512.c tohex.c

clean:
	$(RM) sha512-test sha512 $(LIB)/* $(OBJ)/*
