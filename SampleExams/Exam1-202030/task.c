#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {

    srand(getpid());
    
    printf("task started\n");

    // upcomment this line once you get to the right part of the
    // assignment
    if(rand() % 3 == 0) exit(99);
    
    sleep(3);
    printf("task finished\n");
}
