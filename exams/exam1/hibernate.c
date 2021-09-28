#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	unsigned int sleep_time = 5;

	printf("Hello from process hibernate...\n");
	if (argc > 1) {
		sleep_time = (unsigned) atoi(argv[1]);
	}

	sleep(sleep_time);

	exit(EXIT_SUCCESS);
}
