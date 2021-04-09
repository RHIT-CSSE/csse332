.PHONY: all clean

all: a.bin b.bin c.bin d.bin

a.bin: ActivityA.o
	gcc -o $@ $^ -pthread

b.bin: ActivityB.o
	gcc -o $@ $^ -pthread

c.bin: ActivityC.o
	gcc -o $@ $^ -pthread

d.bin: ActivityD.o
	gcc -o $@ $^ -pthread

solutions: a_solution.bin

a_solution.bin: ActivityA_solution.o
	gcc -o $@ $^ -pthread

clean:
	rm -f *.o *.bin
