/* code adapted from CS461 at UIUC by Dr. Michael Bailey */
#include <stdio.h>
#include <stdlib.h>

#define STACK_GOAL 0xFFFF0000U
#define MAX_PAD 0xFFFF

extern int _main(int, char**, char**);

int main(int argc, char *argv[], char *envp[])
{
  volatile int canary = 0xDEADBEEF;

  // Advance the stack pointer to a position that's invariant of
  // the size of the environment and the program arguments.
  char *esp = alloca(0);
  if ((esp < (char *)STACK_GOAL) || (esp - (char *)STACK_GOAL > MAX_PAD)) {
    fprintf(stderr, "Can't normalize stack position: %p\n", esp);
    return 1;
  }
  alloca(esp - (char *)STACK_GOAL);

#ifdef COOKIE
  alloca(COOKIE);
#else
  fprintf(stderr, "Please run ./setcookie.py before starting this program!\n");
  exit(EXIT_FAILURE);
#endif

  int ret = _main(argc, argv, envp);

  if (canary != 0xDEADBEEF) {
    fprintf(stderr, "Uh oh, the canary is dead.\n" \
            "Don't overwrite the stack frame for main().\n");
  }
  return ret;
}
