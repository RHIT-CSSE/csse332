.PHONY: all clean

all: condwait.bin thread_ABC.bin

condwait.bin: condwait.o
	gcc -o $@ $^ -pthread

thread_ABC.bin: thread_ABC.o
	gcc -o $@ $^ -pthread

solutions: condwait_solution.bin thread_ABC_solution.bin

condwait_solution.bin: condwait_solution.o
	gcc -o $@ $^ -pthread

thread_ABC_solution.bin: thread_ABC_solution.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
