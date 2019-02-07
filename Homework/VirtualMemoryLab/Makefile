all: jonesforth pagedforth

forth_embed.o: forth_embed.c forth_embed.h 
	gcc -m32 -ggdb -c forth_embed.c

jonesforth.o: jonesforth.S
	gcc -m32 -ggdb -c jonesforth.S

jonesforth: jf_intepret.c forth_embed.o jonesforth.o
	gcc -m32 -ggdb -o $@ forth_embed.o jonesforth.o jf_intepret.c

paged_forth.o: paged_forth.c forth_embed.h
	gcc -m32 -ggdb -c paged_forth.c -o paged_forth.o

pagedforth: paged_forth.o forth_embed.o jonesforth.o
	gcc -m32 -ggdb -o $@ forth_embed.o jonesforth.o paged_forth.o


interactive: jonesforth
	./jonesforth jonesforth.f $(PROG)

clean:
	rm -f jonesforth perf_dupdrop *~ core .test_* *.o
