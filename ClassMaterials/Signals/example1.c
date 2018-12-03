/*

This example taken from
http://space.wccnet.edu/~chasselb/linux275/ClassNotes/process/sigbasics.htm

 */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int ouch_count;

void disable_sigint_signal() {
    sigset_t mask;
    sigemptyset (&mask);
    sigaddset (&mask, SIGINT);
    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
        perror ("sigprocmask");
    }
}


void ouch(int sig)
{
    ouch_count++;

    if(ouch_count == 1) {
        printf("OUCH! - I got signal %d.  That hurt.\n", sig);
    }
    if(ouch_count == 2) {
        // after 2 Ctrl-Cs, install the default handler
        // this will terminate the program if another Ctrl-C
        printf("OUCH! - I got signal %d.  I'm nearly dead.\n", sig);
    }
    if(ouch_count == 3) {
        exit(0);
    }
}

/*  The main function has to intercept the SIGINT signal generated when we type Ctrl-C .
    For the rest of the time, it just executes the loop,
    printing a message once a second.  */

int main() 
{
    ouch_count = 0;
    // installs the new signal handler
    signal(SIGINT, ouch);

    // disable sigint signal.  Uncomment this and see what happens.
    // disable_sigint_signal();
    
    int i;
    for(i = 0; i < 6; i++) {
        printf("Hello World!\n");
        sleep(1);
    }
    printf("Exiting naturally\n");
}
