all: canoe chopsticks priority_tunnel reverse_verify

canoe: canoe.c
	gcc -pthread -ggdb canoe.c -o canoe

chopsticks: chopsticks.c
	gcc -pthread -ggdb chopsticks.c -o chopsticks

priority_tunnel: priority_tunnel.c
	gcc -pthread -ggdb priority_tunnel.c -o priority_tunnel

reverse_verify: reverse_verify.c
	gcc -pthread -ggdb reverse_verify.c -o reverse_verify

clean:
	rm -rf canoe chopsticks priority_tunnel reverse_verify 
	rm -rf *.dSYM
