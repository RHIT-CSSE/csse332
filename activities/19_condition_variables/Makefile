.PHONY: all clean

all: mutex.bin condwait.bin prodcons.bin semaphores.bin

mutex.bin: mutex.o
	gcc -o $@ $^ -pthread

condwait.bin: condwait.o
	gcc -o $@ $^ -pthread

prodcons.bin: prodcons.o
	gcc -o $@ $^ -pthread

semaphores.bin: semaphores.o
	gcc -o $@ $^ -pthread

solutions: mutex_solution.bin condwait_solution.bin prodcons_solution.bin semaphores_solution.bin

mutex_solution.bin: mutex_solution.o
	gcc -o $@ $^ -pthread

condwait_solution.bin: condwait_solution.o
	gcc -o $@ $^ -pthread

prodcons_solution.bin: prodcons_solution.o
	gcc -o $@ $^ -pthread

semaphores_solution.bin: semaphores_solution.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
