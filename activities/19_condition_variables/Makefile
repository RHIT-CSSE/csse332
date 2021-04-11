.PHONY: all clean

all: mutex.bin condwait.bin prodcons.bin

mutex.bin: mutex.o
	gcc -o $@ $^ -pthread

condwait.bin: condwait.o
	gcc -o $@ $^ -pthread

prodcons.bin: prodcons.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
