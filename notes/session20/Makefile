.PHONY: all clean

all: condwait.bin

condwait.bin: condwait.o
	gcc -o $@ $^ -pthread

solutions: condwait_solution.bin

condwait_solution.bin: condwait_solution.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
