all: shell.bin donothing.bin

shell.bin: shell.o
	gcc -o shell.bin shell.o

shell.o: shell.c
	gcc -c shell.c

donothing.bin: donothing.o
	gcc -o donothing.bin donothing.o

donothing.o: donothing.c
	gcc -c donothing.c

.PHONY:clean all
clean:
	rm -rf *.o *.bin
