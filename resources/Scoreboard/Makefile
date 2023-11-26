all: 2criticalsection.bin 2criticalsection_solution.bin alternating_ifpossible.bin alternating_ifpossible_solution.bin

2criticalsection.bin: 2criticalsection.c
	gcc -pthread -o $@ 2criticalsection.c

2criticalsection_solution.bin: 2criticalsection_solution.c
	gcc -pthread -o $@ 2criticalsection_solution.c

alternating_ifpossible.bin: alternating_ifpossible.c
	gcc -pthread -o $@ alternating_ifpossible.c

alternating_ifpossible_solution.bin: alternating_ifpossible_solution.c
	gcc -pthread -o $@ alternating_ifpossible_solution.c

clean:
	rm -rf *.bin
