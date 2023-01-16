.PHONY: all clean

all: mutex.bin

mutex.bin: mutex.o
	gcc -o $@ $^ -pthread

solutions: mutex_solution.bin

mutex_solution.bin: mutex_solution.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
