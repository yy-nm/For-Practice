


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
	$(CC) $(CFLAGS) -o sha256 main.linux.mmap.c sha256.c tohex.c

array:
	$(CC) $(CFLAGS) -o sha256 main.linux.array.c sha256.c tohex.c

lib: libdynamic libstatic

libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libsha256.so sha256.c

libstatic: | $(LIB) $(OBJ)
	$(CC) $(CFLAGS) -c -o $(OBJ)/sha256.o sha256.c
	$(AR) $(LIB)/libsha256.a $(OBJ)/sha256.o

test:
	$(CC) $(CFLAGS) -o sha256-test main.c sha256.c tohex.c

clean:
	$(RM) sha256-test sha256 $(LIB)/* $(OBJ)/*
