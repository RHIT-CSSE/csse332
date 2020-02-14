# $Id: Makefile,v 1.9 2007-10-22 18:53:12 rich Exp $

#BUILD_ID_NONE := -Wl,--build-id=none 
BUILD_ID_NONE := 

SHELL	:= /bin/bash

all:	jonesforth gc_forth

forth_embed.o: forth_embed.c forth_embed.h 
	gcc -m32 -ggdb -c forth_embed.c

jonesforth.o: jonesforth.S
	gcc -m32 -ggdb -c jonesforth.S

jonesforth: jf_intepret.c forth_embed.o jonesforth.o
	gcc -m32 -ggdb -o $@ forth_embed.o jonesforth.o jf_intepret.c

arraylist.o: arraylist.h arraylist.c
	gcc -m32 -c -ggdb arraylist.c

CuTest.o: CuTest.c CuTest.h
	gcc -m32 -ggdb -c -o $@ CuTest.c

test: gc_forth
	./gc_forth

gc_forth: gc_forth.c forth_embed.h forth_embed.o jonesforth.o arraylist.o CuTest.o
	gcc -m32 -ggdb -o gc_forth gc_forth.c forth_embed.o jonesforth.o arraylist.o CuTest.o

run: jonesforth
	./jonesforth jonesforth.f $(PROG)

clean:
	rm -f jonesforth gc_forth *.o


.PHONY: test run
