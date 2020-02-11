all: fork_tests mmap_twice_example

forth_embed.o: forth_embed.c forth_embed.h 
	gcc -m32 -ggdb -c forth_embed.c

jonesforth.o: jonesforth.S
	gcc -m32 -ggdb -c jonesforth.S

forking_forth.o: forking_forth.c forth_embed.h
	gcc -m32 -ggdb -c forking_forth.c -o forking_forth.o

CuTest.o: CuTest.h CuTest.c
	gcc -m32 -ggdb -c CuTest.c -o CuTest.o

fork_tests.o: forking_forth.h CuTest.h fork_tests.c
	gcc -m32 -ggdb -c fork_tests.c -o fork_tests.o

fork_tests: forking_forth.o forth_embed.o jonesforth.o fork_tests.o CuTest.o
	gcc -m32 -ggdb -o $@ forking_forth.o forth_embed.o jonesforth.o fork_tests.o CuTest.o

mmap_twice_example: mmap_twice_example.c
	gcc -m32 -ggdb mmap_twice_example.c -o mmap_twice_example

clean:
	rm -f fork_tests *~ core .test_* *.o bigmem.dat delete_me.dat mmap_twice_example
