all: lib app

debug:
	gcc -Wall -c src/*.c
	mkdir -p libs
	ar rcs libs/libn59.a *.o
	rm -f *.o

lib:
	mkdir -p libs
	gcc -Wall -I include -c -DNDEBUG src/*.c
	ar rcs libs/libn59.a *.o
	rm -f *.o

app: lib
	mkdir -p bin
	gcc -Iinclude -lncurses -Llibs -ln59 -Wall app/sample.c -o bin/app

clean:
	rm -rf libs
	rm -rf bin
