all: matmult.bin

matmult.bin: matmult.c
	gcc -O3 -o matmult.bin matmult.c

.PHONY: clean all
clean:
	rm -rf *.o *.bin
