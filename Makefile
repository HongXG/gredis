# gredis Makefile

SOURCE = $(wildcard $(addprefix  ./src/*, .cpp))
OBJ := $(patsubst %cpp, %o, $(SOURCE))

EXAMPLESOURCE = $(wildcard $(addprefix  ./examples/*, .cpp))
EXAMPLEOBJ := $(patsubst %cpp, %o, $(EXAMPLESOURCE))
EXAMPLES := $(basename $(EXAMPLEOBJ))

TESTSOURCE = $(wildcard $(addprefix  ./tests/*, .cpp))
TESTOBJ := $(patsubst %cpp, %o, $(TESTSOURCE))
TESTS := $(basename $(TESTOBJ))

LIBNAME=libgredis

LIBRARYS_DYNIC = hiredis pthread
LIBRARYS_STATIC = gredis

LINK_LIBRARYS = -Wl,-dn $(addprefix -l, $(LIBRARYS_STATIC)) -Wl,-dy $(addprefix -l, $(LIBRARYS_DYNIC))

GREDIS_MAJOR=0
GREDIS_MINOR=11

# Fallback to gcc when $CC is not in $PATH.
CC:=g++
OPTIMIZATION?=-O0
WARNINGS=-Wall -W -Wwrite-strings
DEBUG?= -g -ggdb
REAL_CFLAGS=$(OPTIMIZATION) -fPIC $(CFLAGS) $(WARNINGS) $(DEBUG) $(ARCH)
REAL_LDFLAGS=$(LDFLAGS) $(ARCH)

DYLIBSUFFIX=so
STLIBSUFFIX=a
DYLIB_MINOR_NAME=$(LIBNAME).$(DYLIBSUFFIX).$(GREDIS_MAJOR).$(GREDIS_MINOR)
DYLIB_MAJOR_NAME=$(LIBNAME).$(DYLIBSUFFIX).$(GREDIS_MAJOR)
DYLIBNAME=$(LIBNAME).$(DYLIBSUFFIX)
DYLIB_MAKE_CMD=$(CC) -shared -Wl,-soname,$(DYLIB_MINOR_NAME) -o $(DYLIBNAME) $(LDFLAGS)
STLIBNAME=$(LIBNAME).$(STLIBSUFFIX)
STLIB_MAKE_CMD=ar rcs $(STLIBNAME)

# Platform-specific overrides
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),SunOS)
  REAL_LDFLAGS+= -ldl -lnsl -lsocket
  DYLIB_MAKE_CMD=$(CC) -G -o $(DYLIBNAME) -h $(DYLIB_MINOR_NAME) $(LDFLAGS)
  INSTALL= cp -r
endif
ifeq ($(uname_S),Darwin)
  DYLIBSUFFIX=dylib
  DYLIB_MINOR_NAME=$(LIBNAME).$(GREDIS_MAJOR).$(GREDIS_MINOR).$(DYLIBSUFFIX)
  DYLIB_MAJOR_NAME=$(LIBNAME).$(GREDIS_MAJOR).$(DYLIBSUFFIX)
  DYLIB_MAKE_CMD=$(CC) -shared -Wl,-install_name,$(DYLIB_MINOR_NAME) -o $(DYLIBNAME) $(LDFLAGS)
endif

all: $(DYLIBNAME) $(STLIBNAME)

$(DYLIBNAME): $(OBJ)
	$(DYLIB_MAKE_CMD) $(OBJ)

$(STLIBNAME): $(OBJ)
	$(STLIB_MAKE_CMD) $(OBJ)

%.o: %.cpp
	$(CC) $(REAL_CFLAGS) -c $< -o $@

dynamic: $(DYLIBNAME)
static: $(STLIBNAME)

examples:all $(EXAMPLES)
%: %.cpp $(STLIBNAME)
	$(CC) -o $@ $< $(REAL_CFLAGS) $(REAL_LDFLAGS) -I./src -L./ $(LINK_LIBRARYS)

tests:all $(TESTS)
%: %.cpp $(STLIBNAME)
	$(CC) -o $@ $< $(REAL_CFLAGS) $(REAL_LDFLAGS)  -I./src -L. $(LINK_LIBRARYS) -lgtest

clean:
	rm -f $(OBJ) $(EXAMPLEOBJ) $(TESTOBJ) $(DYLIBNAME) $(STLIBNAME) $(TESTS) $(EXAMPLES)

dep:
	$(CC) -MM $(SOURCE)

# Installation related variables and target
PREFIX?=/usr/local
INSTALL_INCLUDE_PATH= $(PREFIX)/include/gredis
INSTALL_LIBRARY_PATH= $(PREFIX)/lib

ifeq ($(uname_S),SunOS)
  INSTALL?= cp -r
endif

INSTALL?= cp -a

install: $(DYLIBNAME) $(STLIBNAME)
	mkdir -p $(INSTALL_INCLUDE_PATH) $(INSTALL_LIBRARY_PATH)
	$(INSTALL) src/gRedisClient.h $(INSTALL_INCLUDE_PATH)
	$(INSTALL) $(DYLIBNAME) $(INSTALL_LIBRARY_PATH)/$(DYLIB_MINOR_NAME)
	cd $(INSTALL_LIBRARY_PATH) && ln -sf $(DYLIB_MINOR_NAME) $(DYLIB_MAJOR_NAME)
	cd $(INSTALL_LIBRARY_PATH) && ln -sf $(DYLIB_MAJOR_NAME) $(DYLIBNAME)
	$(INSTALL) $(STLIBNAME) $(INSTALL_LIBRARY_PATH)

32bit:
	@echo ""
	@echo "WARNING: if this fails under Linux you probably need to install libc6-dev-i386"
	@echo ""
	$(MAKE) CFLAGS="-m32" LDFLAGS="-m32"

gprof:
	$(MAKE) CFLAGS="-pg" LDFLAGS="-pg"


.PHONY: all test check clean dep install 32bit gprof gcov noopt
