# add your rules here

tests.bin: tests.o CuTest.o basic_threads.o
	gcc -o tests.bin tests.o CuTest.o basic_threads.o

tests.o: tests.c CuTest.h basic_threads.h
	gcc -c tests.c

CuTest.o: CuTest.c CuTest.h
	gcc -c CuTest.c

basic_threads.o: basic_threads.c basic_threads.h
	gcc -c basic_threads.c

in_use_example.o : in_use_example.c basic_threads.h
	gcc -c in_use_example.c

in_use_example.bin: in_use_example.o basic_threads.o
	gcc -o in_use_example.bin basic_threads.o in_use_example.o

# not required for assignment, but handy for your testing
clean:
	rm -rf *.bin *.o
