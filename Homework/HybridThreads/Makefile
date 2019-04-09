all: standalone1 us1tests basic_para_tests create_para_tests

hybrid_threads.o: hybrid_threads.h hybrid_threads.c
	gcc -Wall -c -o hybrid_threads.o hybrid_threads.c

standalone1.o: standalone1.c hybrid_threads.h
	gcc -Wall -c standalone1.c

standalone1: standalone1.o hybrid_threads.o
	gcc -Wall -pthread -o standalone1 standalone1.o hybrid_threads.o

CuTest.o: CuTest.c CuTest.h
	gcc -c CuTest.c

us1tests.o: us1tests.c CuTest.h hybrid_threads.h
	gcc -Wall -c us1tests.c

us1tests: us1tests.o CuTest.o hybrid_threads.o
	gcc -Wall -pthread -o us1tests us1tests.o CuTest.o hybrid_threads.o

basic_para_tests.o: basic_para_tests.c CuTest.h hybrid_threads.h
	gcc -Wall -c basic_para_tests.c

basic_para_tests: basic_para_tests.o CuTest.o hybrid_threads.o
	gcc -Wall -pthread -o basic_para_tests basic_para_tests.o CuTest.o hybrid_threads.o

create_para_tests.o: create_para_tests.c CuTest.h hybrid_threads.h
	gcc -Wall -c create_para_tests.c

create_para_tests: create_para_tests.o CuTest.o hybrid_threads.o
	gcc -Wall -pthread -o create_para_tests create_para_tests.o CuTest.o hybrid_threads.o

clean:
	rm -f *.o standalone1 us1tests basic_para_tests create_para_tests
