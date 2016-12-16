


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
	$(CC) $(CFLAGS) -o sha384 main.linux.mmap.c sha384.c tohex.c

array:
	$(CC) $(CFLAGS) -o sha384 main.linux.array.c sha384.c tohex.c

lib: libdynamic libstatic

libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libsha384.so sha384.c

libstatic: | $(LIB) $(OBJ)
	$(CC) $(CFLAGS) -c -o $(OBJ)/sha384.o sha384.c
	$(AR) $(LIB)/libsha384.a $(OBJ)/sha384.o

test:
	$(CC) $(CFLAGS) -o sha384-test main.c sha384.c tohex.c

clean:
	$(RM) sha384-test sha384 $(LIB)/* $(OBJ)/*
