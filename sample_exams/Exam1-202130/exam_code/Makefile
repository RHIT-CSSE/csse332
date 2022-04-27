all: beezy repair_demon

beezy: beezy.o say_hello.o
	gcc -o beezy beezy.o say_hello.o

beezy.o: beezy.c say_hello.h
	gcc -c beezy.c say_hello.h 

say_hello.o: say_hello.c say_hello.h
	gcc -c say_hello.c say_hello.h

repair_demon: repair_demon.o
	gcc -o repair_demon repair_demon.o

repair_demon.o: repair_demon.c
	gcc -c repair_demon.c

clean:
	rm -rf *.o repair_demon beezy *.dSYM
