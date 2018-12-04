#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

/*

Thanks to Evan Jones for this example
http://www.evanjones.ca/software/threading.html

...very slight modified from it's initial version
*/

// 64kB stack
#define THREAD_STACK_SIZE 1024*64

ucontext_t child, parent;
bool child_done;

// The child thread will execute this function
void threadFunction()
{
    int i;
    for(i = 1; i <= 10; i++) {
        printf("5) child thread running %d of 10\n", i);
        sleep(1);
        //swapcontext( &child, &parent );
    }
    printf( " 6) Child thread exiting\n" );
    child_done = true;
    
}

void man_yield(){
    printf("7) before swapcontext\n");
    swapcontext( &child, &parent );
    printf("8) after swapcontext\n");

}
int main()
{
    signal(SIGINT,man_yield);
    child_done = false;
    // Get the current execution context
    getcontext( &child );

    // Modify the context to a new stack
    child.uc_link = 0;
    child.uc_stack.ss_sp = malloc( THREAD_STACK_SIZE );
    child.uc_stack.ss_size = THREAD_STACK_SIZE;
    child.uc_stack.ss_flags = 0;        
    if ( child.uc_stack.ss_sp == 0 ) {
        perror( "malloc: Could not allocate stack" );
        exit( 1 );
    }

    // Create the new context
    printf( "1) Creating child thread\n" );
    makecontext( &child, &threadFunction, 0 );
        
    while(!child_done) {
        printf("2) doing parent stuff and then switching back to child\n");
        sleep(1);
        swapcontext( &parent, &child );
    }

    printf( "3) Child done\n" );

    // Note that although we (correctly) free here, freeing correctly
    // with swapcontext has its wrinkles.
    //
    // Go ahead and leak the stack space (i.e. don't bother calling
    // free) until we can discuss the issue fully in the final test
    // case
    free( child.uc_stack.ss_sp );

    printf( "4) Child thread returned and stack freed\n" );
        
    return 0;
}


