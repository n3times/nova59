all:
	gcc -Wall -c -DNDEBUG src/*.c
	ar rcs libn59.a *.o

debug:
	gcc -Wall -c src/*.c
	ar rcs libn59.a *.o

clean:
	rm *.o *.a tmp a.out
