#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_WORKERS 3


void main(int argc, char** argv) {

    printf("boss process (pid %d) started\n", getpid());
    
    
}
