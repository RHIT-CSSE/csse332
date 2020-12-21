all: boss_solution.bin task.bin task_bin_mem_report.txt

boss_solution.o: boss_solution.c coollibrary.h
	gcc -c -ggdb boss_solution.c

coollibrary.o: coollibrary.c coollibrary.h
	gcc -c -ggdb coollibrary.c

boss_solution.bin: boss_solution.o coollibrary.o
	gcc -ggdb boss_solution.o coollibrary.o -o boss_solution.bin

task.bin: task.o coollibrary.o
	gcc -ggdb task.o coollibrary.o -o task.bin

task_bin_mem_report.txt: task.bin
	valgrind ./task.bin 2> task_bin_mem_report.txt

task.o: task.c
	gcc -c -ggdb task.c

clean:
	rm -f *.o *.bin task_bin_mem_report.txt
