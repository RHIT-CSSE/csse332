#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*

  To build everything you need for this activity, type "make". The file
  you run will be called forkActivity.bin.
  
  ==================
  PART 1
  ==================

  Write a process that forks 10 child processes, then waits for them all
  to complete, prints a message with their exit code, then exits.

  The child processes should print a message with an ID, sleep for 1 second, then exit with that ID code.

  How to do this?  Make a simple for loop that counts from 0 to 9.

  Within the for loop, do a fork.  Child forks should print and exit.
  The part fork should continue looping.

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
  One child has quit with exit code 0
  One child has quit with exit code 1
  One child has quit with exit code 2
  One child has quit with exit code 3
  One child has quit with exit code 4
  One child has quit with exit code 6
  One child has quit with exit code 5
  One child has quit with exit code 8
  One child has quit with exit code 9
  One child has quit with exit code 7
  exiting

  ** The order of print out could change slightly every time.
  
  ==================
  PART 2
  ==================

  To do this part you need to use buffalosay.bin.  It should have been
  automatically built by the makefile.

  This is a simple program that prints out a buffalo saying a phrase
  that's passed on the command line. Try this and see:

  $ ./buffalosay.bin hello

  What we want is rather than printing I'm child 3 we want to buffalosay I'm child 3.

  Use execlp or execv to invoke buffalosay.bin from the child.  You'll
  need to pass the parameter as a string.  Here's how to build a
  string with an integer in c:

  char output[50];
  snprintf(output, 50, "I'm child %d",i);
  // at this point output contains the desired string (plus the
  // required \0 terminator character).  Its similar to how it would be
  // if we used printf to print it, but snprintf outputs to a string
  // rather than to the console.

  Note if you invoke exec in the obvious way, you'll notice all your
  children will start returning 0.  The reason they are returning 0 is
  because you are execing buffalosay, and buffalosay always returns a
  0 exit value.

  If you really want to get fancy, fix that problem (hint: it requires
  an additional fork).  But that's not really required.

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
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
One child returns with exit code 0
exiting

 */

int main() {
  // look at forkExample for help!
  printf("exiting\n");

  return 0;
}
