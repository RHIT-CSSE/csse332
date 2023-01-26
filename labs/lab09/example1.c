#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*

   Thanks to Evan Jones for this example
http://www.evanjones.ca/software/threading.html

...very slight modified from it's initial version
*/

// 64kB stack
#define THREAD_STACK_SIZE 1024 * 64

ucontext_t child, parent;
bool child_done;

// The child thread will execute this function
void threadFunction()
{
  int i;
  for (i = 1; i <= 10; i++)
  {
    printf("child thread running %d of 10\n", i);
    swapcontext(&child, &parent);
  }
  printf("Child thread exiting\n");
  child_done = true;
  swapcontext(&child, &parent);
}

int main()
{
  child_done = false;
  // Get the current execution context
  getcontext(&child);

  // Modify the context to a new stack
  child.uc_link = 0;
  child.uc_stack.ss_sp = malloc(THREAD_STACK_SIZE);
  child.uc_stack.ss_size = THREAD_STACK_SIZE;
  child.uc_stack.ss_flags = 0;
  if (child.uc_stack.ss_sp == 0)
  {
    perror("malloc: Could not allocate stack");
    exit(1);
  }

  // Create the new context
  printf("Creating child thread\n");
  makecontext(&child, &threadFunction, 0);

  // Execute the child context
  printf("Switching to child thread\n");
  swapcontext(&parent, &child);

  while (!child_done)
  {
    printf("doing parent stuff and then switching back to child\n");
    swapcontext(&parent, &child);
  }

  printf("Child done\n");

  // Note that although we (correctly) free here, freeing correctly
  // with swapcontext has its wrinkles.
  //
  // Go ahead and leak the stack space (i.e. don't bother calling
  // free) until we can discuss the issue fully in the final test
  // case
  free(child.uc_stack.ss_sp);

  printf("Child thread returned and stack freed\n");

  return 0;
}
