/* Name: gdbexample.c 
 * Author: Archana Chidanandan, December 3, 2007
 * Modified:  Delvin Defoe on March 11, 2008
 * Description: This program is meant to be used with a tutorial on 
 * GDB. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./author.h"

int
main(int argc, char *argv[]) {
  int i = 0;

  /* Used to show how pointers and their pointees
     can be examined using GDB */
  Author *ptr;

  /* Used to show how a struct can be examined 
     using GDB */
  Author one;

  /* The following is included to show how command line arguments must
     be specified when running the program in gdb */
  if (argc < 2) {
    printf("Error: incorrect number of arguments\n");
    printf("Enter <exec_name> <num_of_times_to_display_details>\n");
    exit(1);
  }

  int count = atoi(argv[1]);

  strncpy(one.name, "J. K. Rowling", 30);
  one.book_count = 3;
  strncpy(one.book_name[0], "Harry Potter and the Philosopher's Stone", 70);
  strncpy(one.book_name[1], "Harry Potter and the Chamber of Secrets", 70);
  strncpy(one.book_name[2], "Harry Potter and the Prisoner of Azkaban", 70);

  ptr = &one;

  for (i=0; i < count; i++) {
    /* Function defined in author.c.
       Use to demonstrate:
       a. "step"
       b. Setting a breakpoint in another file
    */
    display_author_details(one);
  }

  return 0;
}
