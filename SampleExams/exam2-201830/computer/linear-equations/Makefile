

all: main solver

solver: shm.o solver.o
	gcc shm.o solver.o -o solver

main: main.o shm.o
	gcc main.o shm.o -o main

solver.o: solver.c shm.h
	gcc -c solver.c -o solver.o

main.o: main.c shm.h
	gcc -c main.c -o main.o

shm.o: shm.h shm.c
	gcc -c shm.c -o shm.o

clean:
	rm *.o main solver
