.PHONY: all clean

all: mutex.bin condwait.bin thread_ABC.bin

mutex.bin: mutex.o
	gcc -o $@ $^ -pthread

condwait.bin: condwait.o
	gcc -o $@ $^ -pthread

thread_ABC.bin: thread_ABC.o
	gcc -o $@ $^ -pthread

solutions: mutex_solution.bin condwait_solution.bin thread_ABC_solution.bin

mutex_solution.bin: mutex_solution.o
	gcc -o $@ $^ -pthread

condwait_solution.bin: condwait_solution.o
	gcc -o $@ $^ -pthread

thread_ABC_solution.bin: thread_ABC_solution.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
