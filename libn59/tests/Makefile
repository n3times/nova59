CC = gcc

SRC := $(wildcard src/*.c)
BIN := $(SRC:src/%.c=%)

all: ${BIN}

%: src/%.c
	mkdir -p bin
	${CC} -Wall -I../include -L../libs -ln59 src/$@.c -o bin/$@

clean:
	rm -rf bin
