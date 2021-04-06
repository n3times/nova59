all:
	make -C libn59
	make -C libs59

debug:
	make debug -C libn59
	make debug -C libs59

clean:
	make clean -C libn59
	make clean -C libs59
	rm -rf libs
