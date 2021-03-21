test: gdbexample.o author.o
	gcc gdbexample.o author.o -o test -ggdb

gdbexample.o: gdbexample.c author.h
	gcc gdbexample.c -c -ggdb

author.o: author.c author.h
	gcc author.c -c -ggdb

clean: 
	rm -f *.o test
