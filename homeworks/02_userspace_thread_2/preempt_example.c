#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// 64kB stack
#define THREAD_STACK_SIZE 1024*64


ucontext_t child, parent;

// define an alarm signal handler
void catch_alarm(int sig_num)
{
    printf("alarm\n"); //BTW it's not really safe to print in a signal
                       //handler, but it won't usually cause a problem
    swapcontext( &child, &parent );
}

// The child thread will execute this function
void threadFunction()
{
    while(1) {
        printf("I run forever\n ");
    }
}

int main()
{
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
    printf( "Creating child thread\n" );
    makecontext( &child, &threadFunction, 0 );

    // set a signal handler for ALRM signals
    signal(SIGALRM, catch_alarm);

    // set an alarm signal to go off after 20 microseconds
    // the second 0 means the alarm won't repeat
    ualarm(5000, 0);

    // Execute the child context
    printf( "Switching to child thread\n" );
    swapcontext( &parent, &child );


    ualarm(5000, 0); // if we did not install the alarm again, we'd get
                   // trapped forever

    printf( "Switching to child thread a second time\n" );
    swapcontext( &parent, &child );


    // Free the stack
    free( child.uc_stack.ss_sp );

    printf( "Child thread returned and stack freed\n" );
        
    return 0;
}

