.PHONY: all clean

all: condwait.bin barrier.bin

%.bin: %.o
	gcc -o $@ $^ -pthread

solutions: condwait_solution.bin barrier_solution.bin

clean:
	rm -f *.o *.bin
