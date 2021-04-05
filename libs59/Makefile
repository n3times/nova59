all:
	gcc -Wall -I../libn59/src -c -DNDEBUG src/*.c
	ar rcs libs59.a *.o

debug:
	gcc -Wall -I../libn59/src -c src/*.c
	ar rcs libs59.a *.o

clean:
	rm *.o *.a tmp a.out
