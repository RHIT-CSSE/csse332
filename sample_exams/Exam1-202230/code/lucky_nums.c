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
    
    while(1) {
        pid = fork();
        if(pid == 0) {
            // child
            if(getpid() % 10 == 7) {
                printf("I am lucky child %d\n", getpid());
                while(1);
                
            } else {
                printf("I am unlucky child %d\n", getpid());
            }
            exit(0);
        }
        // parent
        if(pid % 10 == 7) {
            break;
        }
        wait(NULL);
    }


    
    
    printf("parent %d thinks it found a good process %d \n", getpid(), pid);
    sleep(1);
    char num[100];
    snprintf(num, 100, "%d", getpid()); //now num is a string I can pass to exec

    
    if(fork() == 0) {
        execlp("pstree", "pstree", "-p", "-c", num, NULL);
        perror("exec failed");
        exit(99);
    }
    
    printf("parent running forever\n");    
    while(1);
}
