all:	para_benchmark para_benchmark_0 para_benchmark_2 para_benchmark_3 para_benchmark_4 para_benchmark_5 para_benchmark_6 para_benchmark_7 fast_receiver fast_sender slow_receiver slow_sender unreliable_receiver unreliable_sender 

para_benchmark:	para_benchmark.c
	gcc -std=gnu99 para_benchmark.c -o para_benchmark

para_benchmark_0:	para_benchmark_0.c
	gcc -std=gnu99 para_benchmark_0.c -o para_benchmark_0

para_benchmark_2:	para_benchmark_2.c
	gcc -std=gnu99 para_benchmark_2.c -o para_benchmark_2

para_benchmark_3:	para_benchmark_3.c
	gcc -std=gnu99 para_benchmark_3.c -o para_benchmark_3

para_benchmark_4:	para_benchmark_4.c
	gcc -std=gnu99 para_benchmark_4.c -o para_benchmark_4

para_benchmark_5:	para_benchmark_5.c
	gcc -std=gnu99 para_benchmark_5.c -o para_benchmark_5

para_benchmark_6:	para_benchmark_6.c
	gcc -std=gnu99 para_benchmark_6.c -o para_benchmark_6

para_benchmark_7:	para_benchmark_7.c
	gcc -std=gnu99 para_benchmark_7.c -o para_benchmark_7

common.o:	common.c common.h
	gcc -c -std=gnu99 common.c

fast_receiver.o:	fast_receiver.c common.h
	gcc -c -std=gnu99 fast_receiver.c

fast_sender.o:	fast_sender.c common.h
	gcc -c -std=gnu99 fast_sender.c

slow_receiver.o:	slow_receiver.c common.h
	gcc -c -std=gnu99 slow_receiver.c

slow_sender.o:	slow_sender.c common.h
	gcc -c -std=gnu99 slow_sender.c

unreliable_receiver.o:	unreliable_receiver.c common.h
	gcc -c -std=gnu99 unreliable_receiver.c

unreliable_sender.o:	unreliable_sender.c common.h
	gcc -c -std=gnu99 unreliable_sender.c

fast_receiver:	fast_receiver.o common.o
	gcc fast_receiver.o common.o -o fast_receiver

fast_sender:	fast_sender.o common.o
	gcc fast_sender.o common.o -o fast_sender

slow_receiver:	slow_receiver.o common.o
	gcc slow_receiver.o common.o -o slow_receiver

slow_sender:	slow_sender.o common.o
	gcc slow_sender.o common.o -o slow_sender

unreliable_receiver:	unreliable_receiver.o common.o
	gcc unreliable_receiver.o common.o -o unreliable_receiver

unreliable_sender:	unreliable_sender.o common.o
	gcc unreliable_sender.o common.o -o unreliable_sender

clean:
	rm *.o para_benchmark para_benchmark_0 para_benchmark_2 para_benchmark_3 para_benchmark_4 para_benchmark_5 para_benchmark_6 para_benchmark_7 fast_receiver fast_sender slow_receiver slow_sender unreliable_receiver unreliable_sender
