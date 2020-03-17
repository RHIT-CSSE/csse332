all: buffalosay.bin forkActivity.bin forkActivitySolution.bin

buffalosay.bin: buffalosay.c
	gcc buffalosay.c -o buffalosay.bin

forkActivity.bin: buffalosay.bin forkActivity.c
	gcc forkActivity.c -o forkActivity.bin

forkActivitySolution.bin: buffalosay.bin forkActivitySolution.c
	gcc forkActivitySolution.c -o forkActivitySolution.bin

clean:
	rm -f *.bin
