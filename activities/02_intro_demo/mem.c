#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int *a = malloc(10*sizeof(int));

	printf("Address of main is %p\n", main);
	printf("Array starts at %p\n", a);
	printf("Array address is at %p\n", &a);

	free(a);

	return 0;
}
