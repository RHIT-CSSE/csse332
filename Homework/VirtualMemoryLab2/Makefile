
FLAGS   := -no-pie -ggdb -Wall

all: fork_tests.bin mmap_twice_example.bin

forth_embed.o: forth/forth_embed.c forth/forth_embed.h 
	gcc $(FLAGS) -c forth/forth_embed.c -o forth_embed.o

jonesforth.o: forth/myjf.S
	gcc $(FLAGS) -c forth/myjf.S -o jonesforth.o

forking_forth.o: forking_forth.c forth/forth_embed.h
	gcc $(FLAGS) -c forking_forth.c -o forking_forth.o

CuTest.o: CuTest.h CuTest.c
	gcc $(FLAGS) -c CuTest.c -o CuTest.o

fork_tests.o: forth/forking_forth.h CuTest.h fork_tests.c
	gcc $(FLAGS) -c fork_tests.c -o fork_tests.o

fork_tests.bin: forking_forth.o forth_embed.o jonesforth.o fork_tests.o CuTest.o
	gcc $(FLAGS) -o $@ forking_forth.o forth_embed.o jonesforth.o fork_tests.o CuTest.o

mmap_twice_example.bin: mmap_twice_example.c
	gcc $(FLAGS) mmap_twice_example.c -o mmap_twice_example.bin

clean:
	rm -f *.bin *~ core .test_* *.o *.dat
