all: example1.bin activity.bin activity_solution.bin

example1.bin: example1.c
	gcc -ggdb example1.c -o example1.bin

activity.bin: activity.c
	gcc -ggdb activity.c -o activity.bin

activity_solution.bin: activity_solution.c
	gcc -ggdb activity_solution.c -o activity_solution.bin

clean:
	rm -f *.bin
