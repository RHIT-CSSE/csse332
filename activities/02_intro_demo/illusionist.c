/*
 * CSSE 332: Operating Systems.
 *
 * This file allocates space for a signle integer and
 * then changes its value.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
	int *iptr;
	int i;

	/* let's allocate and print an integer */
	iptr = malloc(sizeof(int));
	if (!iptr) {
		perror("Ooops, out of memory...\n");
		exit(EXIT_FAILURE);
	}
	*iptr = 0;

	printf("(%u): Allocated int at location %p\n", getpid(), iptr);

	i = 0;
	while(i < 10) {
		*iptr = i++;
		printf("(%u): Updated the pointer's value to %d\n", getpid(),
		       *iptr);
		sleep(1);
	}

	/* done */
	free(iptr);
}
