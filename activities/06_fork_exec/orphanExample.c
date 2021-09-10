#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid = fork();

	if (pid < 0) {
		perror("Fork failed.\n");
		return 1;  // nonzero means a failure result in unix
	}

	if (pid == 0) {
		printf("Child going for infinite loop\n");
		while(1);
	}

	printf("Parent sleeping for a bit\n");
	sleep(10);

	return 0;
}

