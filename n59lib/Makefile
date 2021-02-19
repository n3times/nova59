all:
	make lib
	make tests

lib:
	gcc -Wall -c src/*.c
	ar rcs libn59.a *.o

tests:
	make lib

clean:
	rm *.o *.a tmp a.out
