FLAGS   := -no-pie -ggdb -Wall

all: jonesforth.bin pagedforth.bin

forth_embed.o: forth/forth_embed.c forth/forth_embed.h 
	gcc $(FLAGS) -c forth/forth_embed.c -o forth_embed.o

jonesforth.o: forth/myjf.S
	gcc $(FLAGS) -c forth/myjf.S -o jonesforth.o

jonesforth.bin: forth/jf_intepret.c forth_embed.o jonesforth.o
	gcc $(FLAGS) -o $@ forth_embed.o jonesforth.o forth/jf_intepret.c

paged_forth.o: paged_forth.c forth/forth_embed.h
	gcc $(FLAGS) -c paged_forth.c -o paged_forth.o

paged_forth_solution.o: paged_forth_solution.c forth/forth_embed.h
	gcc $(FLAGS) -c paged_forth_solution.c -o paged_forth_solution.o

pagedforth.bin: paged_forth.o forth_embed.o jonesforth.o
	gcc $(FLAGS) -o $@ forth_embed.o jonesforth.o paged_forth.o

paged_forth_solution.bin: paged_forth_solution.o forth_embed.o jonesforth.o
	gcc $(FLAGS) -o $@ forth_embed.o jonesforth.o paged_forth_solution.o

interactive: jonesforth.bin
	./jonesforth.bin forth/jonesforth.f $(PROG)

clean:
	rm -f *.bin *.dat *~ core .test_* *.o
