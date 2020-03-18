#include <unistd.h> // execlp()
#include <stdio.h>  // perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(void) {
    /* Lets run "ls -l -h" in all the different ways we can

       BTW "ls -l -h" means print the local directory, in list form,
       with human-readable sizes if you're curious.

       Note you should only uncomment one of these, because once we
       exec the rest of this program is destroyed. */


    /* Option 1: execl

       This exec takes any number of arguments like printf */

    // execl("/bin/ls", "/bin/ls", "-l", "-h", NULL);

    /* Option 2: execlp 

       Same as above, but we don't need to say /bin/ because its on
       our path.  Note that in this class, we frequently execute stuff
       in the local directory (e.g. "./someProg") in that case execlp
       and excecl are equivalent. */

    // execlp("ls", "ls", "-l", "-h", NULL);

    /* Option 3: execv 
       
       This form always takes 2 arguments, but the 2nd argument is a
       NULL terminated array of arguments.  Because building the array
       is an extra step, this form is usually annoying unless you need
       to be able to vary how many arguments you pass programmatically
       (i.e. you don't know how many args there will be when you write
       the exec command). */
    
    char *const command[] = {"/bin/ls","-l","-h", NULL};
    //execv("/bin/ls", command);

    // for reasons of laziness I'll usually write that as
    // execv(command[0], command) but the above is more explicit

    /* Option 4: execvp 
       
       again, exactly the same as option 3, but we use the path. */

    char *const command2[] = {"ls","-l","-h", NULL};
    execvp("ls",command2);

    
    /* 
       NO MATTER WHICH FORM YOU USE

       1. You must always pass the executable name twice.  The first
       time is what to execute, the second time is what you're going
       to tell the executing program its first parameter is (first
       parameter is always the name of the executable).

       2. NULL must always be the last parameter.  Otherwise your exec
       variant can't know where the arguments stop.
    
    */
    
    perror("Return from exec() not expected");
    // ^^ this should never print because exec *replaces* the process that runs it
    exit(EXIT_FAILURE);
}

