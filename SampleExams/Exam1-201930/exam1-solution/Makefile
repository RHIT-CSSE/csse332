
all: slow_gcc unreliable_gcc

slow_gcc: slow_gcc.o handy.o
	gcc -o slow_gcc slow_gcc.o handy.o

unreliable_gcc: unreliable_gcc.o handy.o
	gcc -o unreliable_gcc unreliable_gcc.o handy.o

slow_gcc.o: slow_gcc.c handy.h
	gcc -c slow_gcc.c

unreliable_gcc.o: unreliable_gcc.c handy.h
	gcc -c unreliable_gcc.c

handy.o: handy.c handy.h
	gcc -c handy.c
