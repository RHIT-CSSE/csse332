#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int
main(int argc, char **argv)
{
	int a;
	double b;
	int *iptr;
	double *dptr;

	void *vptr;

	a = 3;
	b = 3.0;

	iptr = &a;
	dptr = &b;

	printf("iptr contains %p, and points to %d\n",
	       iptr, *iptr);
	printf("dptr contains %p, and points to %lf\n",
	       dptr, *dptr);

	// what happens if we cast b as an integer
	printf("If we do a direct cast of b as an integer, we get %d\n",
	       (int)b);
	printf("If we cast dptr as an integer pointer, we get %d\n",
	       *(int*)dptr);

	// let's play with void pointers
	vptr = &b;
	printf("\nDereferencing vptr as a double give %lf\n",
	       *(double*)vptr);

	*(double *)vptr = 5.0;
	printf("\nWhat happens to b? %lf\n", b);

	// let's play with void pointers
	vptr = &a;
	*(int *)vptr = 10;
	printf("\na is now: %d\n", a);
}
