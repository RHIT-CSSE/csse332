FLAGS   := -no-pie -ggdb -Wall

all: jonesforth pagedforth

forth_embed.o: forth/forth_embed.c forth/forth_embed.h 
	gcc $(FLAGS) -c forth/forth_embed.c -o forth_embed.o

jonesforth.o: forth/myjf.S
	gcc $(FLAGS) -c forth/myjf.S -o jonesforth.o

jonesforth: forth/jf_intepret.c forth_embed.o jonesforth.o
	gcc $(FLAGS) -o $@ forth_embed.o jonesforth.o forth/jf_intepret.c

paged_forth.o: paged_forth.c forth/forth_embed.h
	gcc $(FLAGS) -c paged_forth.c -o paged_forth.o

pagedforth: paged_forth.o forth_embed.o jonesforth.o
	gcc $(FLAGS) -o $@ forth_embed.o jonesforth.o paged_forth.o


interactive: jonesforth
	./jonesforth forth/jonesforth.f $(PROG)

clean:
	rm -f pagedforth jonesforth *~ core .test_* *.o
