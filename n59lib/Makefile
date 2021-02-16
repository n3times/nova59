all:
	make lib
	make tests

lib:
	gcc -Wall -c src/number/*.c
	ar rcs libn59.a *.o

tests:
	make lib
