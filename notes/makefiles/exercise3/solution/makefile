
program.o: program.c
	gcc -c program.c

.DEFAULT_GOAL:= program.bin
program.bin: program.o
	gcc -o program.bin program.o

.PHONY: clean
clean:
	rm -rf *.o *.bin
