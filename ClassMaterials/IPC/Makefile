all: factoring_pipes.bin factoring_shared.bin factoring_pipes_solution.bin factoring_shared_solution.bin

factoring_pipes.bin: factoring_pipes.c
	gcc -ggdb factoring_pipes.c -o factoring_pipes.bin

factoring_shared.bin: factoring_shared.c
	gcc -ggdb factoring_shared.c -o factoring_shared.bin

factoring_pipes_solution.bin: factoring_pipes_solution.c
	gcc -ggdb factoring_pipes_solution.c -o factoring_pipes_solution.bin

factoring_shared_solution.bin: factoring_shared_solution.c
	gcc -ggdb factoring_shared_solution.c -o factoring_shared_solution.bin

pipes_simple.bin: pipes_simple.c
	gcc -ggdb pipes_simple.c -o pipes_simple.bin

shared_simple.bin: shared_simple.c
	gcc -ggdb shared_simple.c -o shared_simple.bin

clean:
	rm -f *.bin
