#include "shell.h"


void
hibernate(void)
{
}

void
csse332_sleep(void)
{
}

void
cleanup(void)
{
}

void
pong(void)
{
}


void
fpoint(void)
{
}

void
primes(void)
{
}

void
status(void)
{
	system("ps -a");
}

void
do_fp_work(void)
{
	double x, y;
	int a, b;

	srand(getpid());

	if(rand() >= RAND_MAX/2) {
		printf("PANIC\n");
		b = 0;
	} else {
		x = 10.0;
		y = 5.0;
		b = 1;
	}

	while(1) {
		x = x / ((a / b) * y);
	}
}

