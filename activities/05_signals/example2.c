#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	printf("Hello from process %u\n", getpid());

	printf("Sending the stop signal to myself!\n");
	kill(getpid(), SIGSTOP);

	printf("Recovered from the stop signal, continuing...\n");

	printf("Waiting for you to terminate\n");
	while(1);
}
