all: simpleshell donothing

simpleshell: simpleshell.o
	gcc simpleshell.o -o simpleshell

donothing: donothing.o
	gcc donothing.o -o donothing

simpleshell.o: simpleshell.c
	gcc -c simpleshell.c

donothing.o: donothing.c
	gcc -c donothing.c

clean:
	rm -f simpleshell donothing *.o
