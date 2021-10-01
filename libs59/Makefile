all:
	gcc -Wall -I../libn59/include -c -DNDEBUG src/*.c
	mkdir -p libs
	ar rcs libs/libs59.a *.o

debug:
	gcc -Wall -I../libn59/include -c src/*.c
	mkdir -p libs
	ar rcs libs/libs59.a *.o

clean:
	rm -f *.o
	rm -f tmp a.out
	rm -rf libs
