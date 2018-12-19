/* server-admin.c
 
When something like a webserver is running, there's always a risk
that some weird bug will cause the webserver to exit unexpectedly
(say a segfault).

An admin process generally initially starts a server, then monitors
it, then restarts it if it fails.

HOW TO BUILD

To build the admin process:

gcc server-admin.c -o server-admin

To build the webserver (which you should not need to edit):

gcc web-server.c -o server

STEP 1 (10 points):

Using fork and exec, make it so that the server admin starts the
webserver when instructed to do so.  YOU MUST USE FORK AND EXEC for
full credit - not system.  You don't have to worry about restart
behavior yet.

The webserver you start with exec is called "./server" and needs no
parameters.

STEP 2 (10 points)

Use wait to monitor the webserver process.  If the process dies, the
admin should restart it.  It should restart it up to MAX_STARTS - 1 
times, and then it should assume there is a persistant problem and 
exit the admin with an appropiate error message.

Use wait to solve this part, DO NOT attempt to handle the SIGCHLD
signal.

To test this feature, use the unix kill command:

kill PID

where PID is the pid of the child webserver process (not the admin).
That will let you make the server die whenever you like.

STEP 3 (10 points)

Restarting is great but sometimes it's necessary to terminate the
webserver to do some work.

To trigger a safe shutdown, we'll use the SIGUSR1 signal.  When the
admin (not the child) gets a SIGUSR1 signal it should:

a) disable restarting (depending on how you implemented it this might
   or might not take special code, BUT we want to be clear that when
   terminated in this way the webserver should not be restarted)

b) send a signal to kill the webserver itself

   Do this with the unix kill function (you can see documentation with
   "man 2 kill").  In short though, it acts like the command line kill
   command, so "kill(somePid, SIGKILL);" acts like running "kill
   somePid" on the command line.  Of course you'll need to know the
   pid of the child process to do this (hint: take a look at the
   manpage for fork() if you're not sure how to get this)

c) wait for the child to terminate (BTW, it is safe to call wait if
   you have no children, it will immediately return)

d) exit gracefully with some appropiate message printed to the screen

To test this code use the kill command like this (where adminpid is
the pid of your admin process):

kill -s SIGUSR1 adminpid

*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_STARTS 3

/* Forks a child process to run the web server application. 
 * The command name (e.g., ./server) and its arguments 
 * are expected.
 */
void runWebServer() {
    // You might want to solve steps 1 and 2 in this function.
    for(int i = 0; i < MAX_STARTS; i++) {
        printf("Starting webserver try %d\n", i+1);
    }
}

int main(int argc, char *argv[]) {
    int answer;
    printf("\n\n++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("This is the server admin and its process id is %d\n", getpid());
    printf("You will need to remember this process id.\n");
    printf("Are you ready to start the web server? 1 for yes, 2 for no.\n");
    scanf("%d", &answer);
    printf("You answered '%d'\n", answer);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++\n");
    if (answer != 1) { return 0; }
    runWebServer();

    printf("Max restarts exceeded.\n");
    return EXIT_SUCCESS;
}
