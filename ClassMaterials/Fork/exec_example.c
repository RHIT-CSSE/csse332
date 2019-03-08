#include <unistd.h> // execlp()
#include <stdio.h>  // perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(void) {
    char *const command[] = {"/bin/ls","-l",NULL};
    execv(command[0],command);

    /* 
       you could also the same thing like this:

       execlp("ls", "ls", "-l", NULL);  <= This is printf style where execlp accepts an arbitrary number of args

       Either way though, there are two things to keep in mind:

       1. You must always pass the excuatble name twice.  The first
       time is what to execute, the second time is what you're going
       to tell the excuting program its first parameter is (first
       parameter is always the name of the executable).

       2. NULL must always be the last parameter.  Otherwise your exec
       variant can't know where the arguments stop.
    
    */
    
    perror("Return from execlp() not expected");
    // ^^ this should never print because exec *replaces* the process that runs it
    exit(EXIT_FAILURE);
}

