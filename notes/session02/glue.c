/*
 * CSSE 332: Operating Systems.
 *
 * This file writes a simple line to a file in /tmp/csss332
 *
 */
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char *argv[])
{
	FILE *fp;

	fp = fopen("/tmp/csse332", "w");
	if (!fp) {
		perror("Could not open the file...");
		exit(EXIT_FAILURE);
	}

	/* let's do some writing and close the file...*/
	fprintf(fp, "Hello CSSE 332...\n");
	fclose(fp);
}
