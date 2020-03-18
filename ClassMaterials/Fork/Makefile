all: execExample.bin forkExample.bin buffalosay.bin forkActivity.bin forkActivitySolution.bin

execExample.bin: execExample.c
	gcc execExample.c -o execExample.bin

forkExample.bin: forkExample.c
	gcc forkExample.c -o forkExample.bin

buffalosay.bin: buffalosay.c
	gcc buffalosay.c -o buffalosay.bin

forkActivity.bin: buffalosay.bin forkActivity.c
	gcc forkActivity.c -o forkActivity.bin

forkActivitySolution.bin: buffalosay.bin forkActivitySolution.c
	gcc forkActivitySolution.c -o forkActivitySolution.bin

clean:
	rm -f *.bin
