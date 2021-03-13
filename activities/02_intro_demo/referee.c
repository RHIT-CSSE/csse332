/*
 * CSSE 332: Operating Systems.
 *
 * This file writes several times to a signle file
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int
main(int argc, char *argv[])
{
	FILE *fp;
	int i = 0;

	fp = fopen("/tmp/csse332", "a");
	if (!fp) {
		perror("Could not open the file...");
		exit(EXIT_FAILURE);
	}

	/* let's do some writing and close the file...*/
	while(i < 5) {
		fprintf(fp, "Hello from process (%u) with i=%d\n",
			getpid(), i);
		sleep(1);
		printf("(%u) sleeping on %d\n", getpid(), i++);
	}
	fclose(fp);
}
