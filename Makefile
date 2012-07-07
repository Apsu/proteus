CC=c++
CFLAGS=-O3 -lpthread -D_GNU_SOURCE -D_REENTRANT -D_THREAD_SAFE
DFLAGS= -g -lpthread -D_GNU_SOURCE -D_REENTRANT _THREAD_SAFE
CFILES=*.cpp

all: build

build:
	@echo "Building Proteus..."
	@${CC} ${CFLAGS} ${CFILES} -o proteus
	@strip --strip-unneeded proteus
	@chmod 500 proteus

debug:
	@echo "Building Proteus with debugging..."
	@${CC} ${DFLAGS} ${CFILES} -o proteus
	@chmod 500 proteus
