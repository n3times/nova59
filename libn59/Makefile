all:
	gcc -Wall -c -DNDEBUG src/*.c
	mkdir -p ../libs
	ar rcs ../libs/libn59.a *.o

debug:
	gcc -Wall -c src/*.c
	mkdir -p ../libs
	ar rcs ../libs/libn59.a *.o

clean:
	rm *.o
	rm -f tmp a.out
