all:
	gcc -Wall -I../libn59/src -c -DNDEBUG src/*.c
	ar rcs ../libs/libs59.a *.o
	mkdir -p ../libs

debug:
	gcc -Wall -I../libn59/src -c src/*.c
	mkdir -p ../libs
	ar rcs ../libs/libs59.a *.o

clean:
	rm *.o
	rm -f tmp a.out
