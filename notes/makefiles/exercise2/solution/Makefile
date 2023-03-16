b.bin: a.o b.o c.o
	gcc -o b.bin a.o b.o c.o

a.o: a.c a.h
	gcc -c a.c a.h

b.o: b.c a.h
	gcc -c b.c a.h

c.o: c.c
	gcc -c c.c
