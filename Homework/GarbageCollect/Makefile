
FLAGS   := -no-pie -ggdb -Wall

all: gc_forth.bin

forth_embed.o: forth/forth_embed.c forth/forth_embed.h 
	gcc $(FLAGS) -c forth/forth_embed.c -o forth_embed.o

jonesforth.o: forth/myjf.S
	gcc $(FLAGS) -c forth/myjf.S -o jonesforth.o

test: gc_forth.bin
	./gc_forth.bin

gc_forth.bin: gc_forth.c forth/forth_embed.h CuTest.h forth_embed.o jonesforth.o arraylist.o CuTest.o
	gcc $(FLAGS) -o gc_forth.bin gc_forth.c forth_embed.o jonesforth.o arraylist.o CuTest.o

gc_forth_solution.bin: gc_forth_solution.c forth/forth_embed.h CuTest.h forth_embed.o jonesforth.o arraylist.o CuTest.o
	gcc $(FLAGS) -o gc_forth_solution.bin gc_forth_solution.c forth_embed.o jonesforth.o arraylist.o CuTest.o

CuTest.o: CuTest.h CuTest.c
	gcc $(FLAGS) -c CuTest.c -o CuTest.o

clean:
	rm -f *.bin *~ core .test_* *.o *.dat
