#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

int
main(int argc, char **argv)
{
	int read_fd = 0, write_fd = 0;
	int i = 0;
	char buf[] = {'a', '\0'};

	if(argc != 3) {
		fprintf(stderr, "Incorrect number of arguments passed to ping\n");
		exit(EXIT_FAILURE);
	}
	read_fd = atoi(argv[1]);
	write_fd = atoi(argv[2]);

	while(i < 5) {
		write(write_fd, buf, 2);
		read(read_fd, buf, 2);
		printf("%d: Read %s from parent\n", getpid(), buf);
		i++;
		sleep(1);
	}
	close(write_fd);
	close(read_fd);

	exit(EXIT_SUCCESS);
}
