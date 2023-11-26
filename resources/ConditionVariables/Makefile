all: thread_count.bin thread_count_solution.bin thread_ABC.bin thread_ABC_solution.bin

thread_count.bin: thread_count.c
	gcc -ggdb -pthread thread_count.c -o thread_count.bin

thread_count_solution.bin: thread_count_solution.c
	gcc -ggdb -pthread thread_count_solution.c -o thread_count_solution.bin

thread_ABC.bin: thread_ABC.c
	gcc -ggdb -pthread thread_ABC.c -o thread_ABC.bin

thread_ABC_solution.bin: thread_ABC_solution.c
	gcc -ggdb -pthread thread_ABC_solution.c -o thread_ABC_solution.bin
clean:
	rm -f *.bin
