all: shower.bin task_dispatch.bin stoplight.bin sieve.bin

clean:

	rm -f *.bin *.o

shower.bin: shower.c
	gcc -ggdb -pthread shower.c -o shower.bin

task_dispatch.bin: task_dispatch.c
	gcc -ggdb -pthread task_dispatch.c -o task_dispatch.bin

stoplight.bin: stoplight.c
	gcc -ggdb -pthread stoplight.c -o stoplight.bin

sieve.bin: sieve.c
	gcc -ggdb -pthread sieve.c -o sieve.bin
