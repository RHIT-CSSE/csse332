## Variables:
## The name of the C compiler.  The default is gcc
CC = gcc

## Name the executable program
PROG = runTests

## Build the program from the object files
$(PROG): UnitTests.o UnitTestingFork.o
	$(CC) UnitTests.o UnitTestingFork.o -o $(PROG)


UnitTests.o: UnitTests.c UnitTests.h
	$(CC) -c UnitTests.c -o UnitTests.o

UnitTestingFork.o: UnitTestingFork.c UnitTests.h
	$(CC) -c UnitTestingFork.c -o UnitTestingFork.o

## Remove all the compilation and debugging files
clean: 
	rm -f core $(PROG) *.o *~
