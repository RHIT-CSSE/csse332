#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "common.h"

int read_pipe(char *name, MODE mode, int fd) {
  int nbytes, r;
  char buffer[BUFSIZE];

  srand(time(NULL) + getpid());

  while ((nbytes=read(fd, buffer, BUFSIZE)) > 0) {
    if (VERB) {
      printf("%s receiving: ", name);
    }
    for (int i=0; i<nbytes; i++) {
      if (buffer[i] == 0) {
	if (i < nbytes-1) {
	  if (VERB) {
	    printf("\n");
	  }

	  switch (mode) {

	  case SLOW:
	    r = rand();
	    usleep(500000 + (r % 500000));
	    break;
      
	  case UNRELIABLE:
	    r = rand();
	    if (r%3 == 0) {
	      printf("%s random failure\n", name);
	      return 1;
	    }
	    break;
          case FAST:
            break;
	  }

	  if (VERB) {
	    printf("%s receiving: ", name);
	  }
	}
      } else {
	if (VERB) {
	  printf("%c", buffer[i]);
	}
      }
    }
    if (VERB) {
      printf("\n");
    }
  }

  return 0;
}

int write_pipe(char *name, MODE mode, int fd) {
  int r;
  char buffer[BUFSIZE];

  srand(time(NULL) + getpid());

  for (int i=0; i<ITER; i++) {
    snprintf(buffer, sizeof(buffer), "%s: %d", name, i);
    if (VERB) {
      printf("%s sending: %s\n", name, buffer);
    }
    write(fd, buffer, (strlen(buffer)+1));
    
    switch (mode) {
	  
    case SLOW:
      r = rand();
      usleep(500000 + (r % 500000));
      break;
      
    case UNRELIABLE:
      r = rand();
      if (r%3 == 0) {
	printf("%s random failure\n", name);
	return 1;
      }
      break;
    case FAST:
      break;
     
    }
  }

  return 0;
}
