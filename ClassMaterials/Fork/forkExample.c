#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid < 0) {
        perror("Fork failed.\n");
        return 1;  // nonzero means a failure result in unix
    }

    if (pid == 0) {
        printf("I am the child process\n");
        sleep(1);  // wait 1 second
        exit(3); // THIS EXIT IS IMPORTANT
                 // If it was not here, the child would go
                 // and run the parent code below
    } 

    printf("I am the parent process\n");
    int status;
    wait(&status);
    printf("Child complete with status %d\n",WEXITSTATUS(status));
    printf("exiting.\n");
    
    return 0;
}

