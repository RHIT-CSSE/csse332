/* Copyright 2016 Rose-Hulman
   But based on idea from http://cnds.eecs.jacobs-university.de/courses/caoslab-2007/
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
  char command[82];

  // infinite loop but ^C quits
  while (1) {
    printf("%% ");
    fgets(command, 82, stdin);
    // note that for the zombie step you'll want to use strcmp, but
    // remember that fgets will return the \n as part of the result;
    // so compare with "kill zombies\n" or drop the \n from the command.
    system(command);
  }
}
