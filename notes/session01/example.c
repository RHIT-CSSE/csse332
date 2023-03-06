#include <stdio.h>
#include <stdlib.h>

int global_variable = 3;

void basic_ptrs(void)
{
  int local_variable = 5;
  int *p;
  int y;

  // assign p to the address of local_variable
  p = &local_variable;
  printf("p now contains the address %p\n", p);

  // let's change local_variable's value
  *p = 10;
  printf("after changing *p, local_variable is %d\n", local_variable);

  // let's create a copy of local_variable but one that contains the same value
  y = *p;
  printf("y is now a copy of local_variable and contains %d\n", y);

  // change y and see if that changes local_variable
  y = 15;
  printf("After changing y, y = %d, local_variable = %d\n", y, local_variable);
}

void void_ptrs(void)
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

void ptr_arithmetic(void)
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
}

int main(int argc, char **argv)
{
  printf("The address of our global variable is %p\n", &global_variable);

  basic_ptrs();
  puts("################################################################");
  void_ptrs();
  puts("################################################################");
  ptr_arithmetic();
}
