

CC ?= gcc -std=gnu99
SHARED := -fPIC --shared
LIB ?= libs
OBJ ?= obj
CFLAGS = -O2 -Wall
AR = ar rcs
RM = rm -rf

CFILENAME = kmp

all: lib
.PHONY: test clean libdynamic libstatic lib

$(LIB):
	mkdir $(LIB)

$(OBJ):
	mkdir $(OBJ)

define CFILEOBJECT
  $(OBJ)/$(strip $(1)).o: $(1).c | $(OBJ)
	$(CC) $(CFLAGS) -c -o $$@ $$<
endef


$(foreach v, $(CFILENAME), $(eval $(call CFILEOBJECT, $(v))))

lib: libdynamic libstatic


libdynamic: | $(LIB)
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libkmp.so $(foreach v, $(CFILENAME), $(v).c)

libstatic: $(foreach v, $(CFILENAME), $(OBJ)/$(v).o) | $(LIB) $(OBJ)
	$(AR) $(LIB)/libkmp.a $(foreach v, $(CFILENAME), $(OBJ)/$(v).o)

test:
	$(CC) $(CFLAGS) -o kmp-test main.c $(foreach v, $(CFILENAME), $(v).c)

clean:
	$(RM) kmp-test $(LIB)/* $(OBJ)/*
