

CC ?= gcc -std=gnu99
SHARED := -fPIC --shared
LIB ?= libs
OBJ ?= obj
CFLAGS = -O2 -Wall -I./curl-source/include -L./curl-source/lib/.libs/
AR = ar rcs
RM = rm -rf
CURL_LIB = -lcurl
CURL_DEP_LIB = -lssl -lcrypto -lrt -lidn -lpthread -lldap
CURL_S_LIB = ./curl-source/lib/.libs/libcurl.a

CFILENAME = httprequest

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
	$(MAKE) -C curl-source
	$(CC) $(SHARED) $(CFLAGS) -o $(LIB)/libhttprequest.so $(foreach v, $(CFILENAME), $(v).c) $(CURL_LIB) $(CURL_DEP_LIB)

libstatic: $(foreach v, $(CFILENAME), $(OBJ)/$(v).o) | $(LIB) $(OBJ)
	$(MAKE) -C curl-source
	$(AR) $(LIB)/libhttprequest.a $(foreach v, $(CFILENAME), $(OBJ)/$(v).o) $(CURL_S_LIB)

test:
	$(MAKE) -C curl-source
	$(CC) $(CFLAGS) -o httprequest-test main.c $(foreach v, $(CFILENAME), $(v).c) $(CURL_LIB) $(CURL_DEP_LIB)

clean:
	$(MAKE) -C curl-source clean
	$(RM) httprequest-test $(LIB)/* $(OBJ)/*
