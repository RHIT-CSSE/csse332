#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

  
#define MAX_CHAIN 5

int main(int argc, char** argv) {

    int pid = 0;

    // not much sample code needed for this assignment, modify this as
    // you see fit
    
    printf("parent %d thinks it found a good process %d \n", getpid(), pid);

    printf("parent running forever\n");    
    while(1);
}
