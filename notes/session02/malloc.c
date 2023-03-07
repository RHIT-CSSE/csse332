#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void basic_malloc(void)
{
  int *a = 0;
  int i;

  a = (int *)malloc(10 * sizeof(int));
  if(!a) {
    fprintf(stderr, "Out of space: cannot allocate a\n");
    return;
  }

  for(i = 0;i < 10; i++) {
    a[i] = rand();
  }

  // free the space allocated
  free(a);
}

void out_of_bounds(void) {
  int *a = 0;
  a = (int *)malloc(10 * sizeof(int));
  if(!a) {
    fprintf(stderr, "Out of space: cannot allocate a\n");
    return;
  }

  a[11] = 300;
  printf("a[12] is %d\n", a[12]);
}

void memory_leak(void) {
  int *a = 0;
  int b[10];

  a = (int *)malloc(10 * sizeof(int));
  if(!a) {
    fprintf(stderr, "Out of space: cannot allocate a\n");
    return;
  }

  // set a to b
  a = b;

  a[0] = rand();
  printf("b[0] is now %d\n", b[0]);

  // when we exit, b will be destroyed since it is allocated on the stack, but
  // not the original a, and we have lost access to a
}

void bad_free(void) {
  int *a = 0, *b = 0;
  a = (int *)malloc(10 * sizeof(int));
  if(!a) {
    fprintf(stderr, "Out of space: cannot allocate a\n");
    return;
  }

  b = a;

  // done with b, now free it
  free(b);

  // oh but a is still there, so let's use it. VERY BAD!
  a[3] = rand();
  printf("a[3] is now %d\n", a[3]);

  // hmm, should we free a now? VERY BAD!
  free(a);

  // better approach
  // free(b);
  // a = b = 0;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  basic_malloc();
  out_of_bounds();
  memory_leak();
  bad_free();
}
