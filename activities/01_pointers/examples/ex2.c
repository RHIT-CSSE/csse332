#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int
main(int argc, char **argv)
{
	int a[10];
	int *iptr;
	char *cptr;

	a[0] = 1;
	a[1] = 2;

	// assign iptr to the start of the array
	iptr = a;
	printf("iptr is  %p\n", iptr);
	printf("&a[0] is %p\n", &a[0]);

	// let's do some pointer arithmetic
	printf("\n\n*********************************\n");
	printf("(iptr + 1) is	%p\n", iptr + 1);
	printf("*(iptr + 1) is	%d\n", *(iptr + 1));
	printf("&a[1] is	%p\n", &a[1]);
	printf("sizeof(int) is	%lu\n", sizeof(int));

	// let's do some weird stuff
	printf("\n\n*********************************\n");
	cptr = (char *)iptr;
	printf("cptr is			%p\n", cptr);
	printf("cptr + 1 is		%p\n", cptr+1);
	printf("sizeof(char) is		%lu\n", sizeof(char));

	printf("\n\n*********************************\n");
	printf("*cptr is		%x\n", *cptr);
	printf("*(cptr+4) is		%x\n", *(cptr+4));

#if 0
	// what if we read back as an integer?
	cptr++;
	iptr = (int *)cptr;
	printf("\n\n*********************************\n");
	printf("a[0] is		%d (%08x)\n", a[0], a[0]);
	printf("a[1] is		%d (%08x)\n", a[1], a[1]);
	printf("*iptr is	%d (%08x)\n", *iptr, *iptr);
#endif
}
