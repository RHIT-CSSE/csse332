#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

#include "handy.h"

void handle_signal(int num) {
    printf("got sigint signal...quitting!\n");
    exit(4);
}

int main(int argc, char** argv) {

    char** argv_copy = malloc_a_copy_that_ends_in_null(argv, argc);

    signal(SIGINT, handle_signal);
    
    printf("SLOW GCC ");
    for(int i = 1; i < argc; i++) {        
        printf("%s ", argv[i]);
    }
    printf("\n");
    argv_copy[0] = "gcc";

    srand(time(NULL) + getpid());
    int r = rand();
    
    usleep(500000 + (r % 100000));
    execvp(argv_copy[0], argv_copy);
    perror("failure running gcc");
    
}
