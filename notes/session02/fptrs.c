#include <stdio.h>
#include <stdlib.h>

int foo(int x, int y) {
  printf("in function %s\n", __func__);
  return x + y;
}

int bar(int x, int y) {
  printf("in function %s\n", __func__);
  return x - y;
}

int main(int argc, char **argv)
{
  int x = 5, y = 3, z;
  void *p;
  // define a function pointer
  int (*fptr)(int, int) = 0;

  // change the function pointer to foo
  fptr = foo;

  // call fptr
  printf("calling foo using fptr results in %d\n", fptr(x, y));

  // change it to bar now
  fptr = bar;

  // call fptr
  printf("calling bar using fptr results in %d\n", fptr(x, y));

  // can also using a void ptr to use a function pointer
  p = foo; // can also use p = &foo;

  // but must cast when calling
  z = ((int (*)(int,int))p)(x, y);

  printf("z = %d\n", z);

  exit(EXIT_SUCCESS);
}
