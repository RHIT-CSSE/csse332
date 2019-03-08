#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  ==================
  PART 1
  ==================

  Write a process that forks 10 child processes, then waits for them all
  to complete, prints a message with their exit code, then exits.

  The child processes should print a message with an ID, sleep for 1 second, then exit with that ID code.

  If you do it correctly the output should look like this:

  I'm child 0
  I'm child 1
  I'm child 2
  I'm child 3
  I'm child 4
  I'm child 5
  I'm child 6
  I'm child 8
  I'm child 9
  I'm child 7 // ~ one second delay
  One child returns with exit code 0
  One child returns with exit code 1
  One child returns with exit code 2
  One child returns with exit code 3
  One child returns with exit code 4
  One child returns with exit code 6
  One child returns with exit code 5
  One child returns with exit code 8
  One child returns with exit code 9
  One child returns with exit code 7
  exiting

  ** The order of print out could change slightly every time.
  
  ==================
  PART 2
  ==================

  To do this part you need to compile buffalosays.c.  You can do it like this:

  $ gcc -o buffalosay buffalosay.c

  This is a simple program that prints out a buffalo saying a phrase
  that's passed on the command line. Try this and see:

  $ ./buffalosay hello

  What we want is rather than printing I'm child 3 we want to buffalosay I'm child 3.

  Use execlp or execv to invoke buffalosay from the child.  You'll
  need to pass the parameter as a string.  Here's how to build a
  string with an integer in c:

  char output[50];
  snprintf(output, 50, "I'm child %d",i);

  Note that we still want all our children to exit with the various
  exit codes.  This means we will have to fork a second time before
  we exec.

  Example output:

               _.-````'-,_
   _,.,_ ,-'`           `'-.,_
 /)     (\                   '``-.
((      ) )                      `\
 \)    (_/                        )\
  |       /)           '    ,'    / \
  `\    ^'            '     (    /  ))
    |      _/\ ,     /    ,,`\   (  "`
     \Y,   |  \  \  | ````| / \_ \
    /  `)_/    \  \  )    ( >  ( >
   |            \( \(     |/   |/
   |           /_(/_(    /_(  /_(
   | 
   < I'm child 0 > 

               _.-````'-,_
   _,.,_ ,-'`           `'-.,_
 /)     (\                   '``-.
((      ) )                      `\
 \)    (_/                        )\
  |       /)           '    ,'    / \
  `\    ^'            '     (    /  ))
    |      _/\ ,     /    ,,`\   (  "`
     \Y,   |  \  \  | ````| / \_ \
    /  `)_/    \  \  )    ( >  ( >
   |            \( \(     |/   |/
   |           /_(/_(    /_(  /_(
   | 
   < I'm child 1 > 

A BUNCH OF OTHER BUFFALOS, maybe overwriting each other, which have
been omitted for brevity


One child returns with exit code 0
One child returns with exit code 1
One child returns with exit code 2
One child returns with exit code 3
One child returns with exit code 5
One child returns with exit code 6
One child returns with exit code 4
One child returns with exit code 7
One child returns with exit code 9
One child returns with exit code 8
exiting

 */

int main() {
  // look at forkExample for help!
  printf("exiting\n");

  return 0;
}
