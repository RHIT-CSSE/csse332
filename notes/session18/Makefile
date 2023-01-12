all: threadSpeedTest.bin threadSpeedTest_solution.bin thread_example.bin

threadSpeedTest.bin: threadSpeedTest.c
	gcc -ggdb -pthread threadSpeedTest.c -o threadSpeedTest.bin

threadSpeedTest_solution.bin: threadSpeedTest_solution.c
	gcc -ggdb -pthread threadSpeedTest_solution.c -o threadSpeedTest_solution.bin

thread_example.bin: thread_example.c
	gcc -ggdb -pthread thread_example.c -o thread_example.bin
clean:
	rm -f *.bin
