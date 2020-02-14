#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include "forth_embed.h"

/*

An example of catching and using mmap to recover from a segfault.

Compile this code like this:

gcc segfault_catch_example.c -o segfault_catch_example

 */

// if the region requested is already mapped, things fail
// so we want address that won't get used as the program
// starts up
#define STACKHEAP_MEM_START 0xf9f8c000

static void handler(int sig, siginfo_t *si, void *unused)
{
    void* fault_address = si->si_addr;

    printf("in handler with invalid address %p\n", fault_address);
    int distance = (void*) fault_address - (void*) STACKHEAP_MEM_START;
    if(distance < 0 || distance > getpagesize()) {
        printf("address not within expected page!\n");
        exit(2);
    }


    // in your code you'll have to compute a particular page start and
    // map that, but in this example we can just map the same page
    // start all the time
    printf("mapping page starting at %p\n", STACKHEAP_MEM_START);
    void* result = mmap((void*) STACKHEAP_MEM_START,
                        getpagesize(),
                        PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS,
                        -1,
                        0);
    if(result == MAP_FAILED) {
        perror("map failed");
        exit(1);
    }
}



void main() {
    
    // installing SEGV signal handler
    // incidently we must configure signal handling to occur in its own stack
    // otherwise our segv handler will use the regular stack for its data
    // and it might try and unmap the very memory it is using as its stack

    // note it is important that this variable be static, not so much
    // in this example (it would be safe to remove it from main in
    // particular)...but if you moved this setup to another function
    // making this variable non-static is asking for some really funky
    // bugs
    static char stack[SIGSTKSZ];
    
    stack_t ss = {
                  .ss_size = SIGSTKSZ,
                  .ss_sp = stack,
    };
    
    sigaltstack(&ss, NULL);

    struct sigaction sa;

    // SIGINFO tells sigaction that the handler is expecting extra parameters
    // ONSTACK tells sigaction our signal handler should use the alternate stack
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;

    //this is the more modern equalivant of signal, but with a few
    //more options
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("error installing handler");
        exit(3);
    }

    // OK now let's do some illegal memory accesses!
    int *array = (int*) STACKHEAP_MEM_START;
    printf("about to do an illegal memory access\n");
    array[5] = 99;
    printf("stored and retrieved %d from memory!\n", array[5]);
    
    printf("done\n");
}
