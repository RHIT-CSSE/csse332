## Variables:
## The name of the C compiler.  The default is gcc
CC = gcc

## Name the executable program
PROG = runTests

## Build the program from the object files
$(PROG): UnitTestingFork.c UnitTests.c UnitTests.h
	$(CC) UnitTestingFork.c UnitTests.c -o $(PROG)

## Remove all the compilation and debugging files
clean: 
	rm -f core $(PROG) *.o *~
