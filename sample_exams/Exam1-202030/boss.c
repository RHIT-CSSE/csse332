#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_WORKERS 3

static int *glob_var;


void make_child() {
    int fork_result = fork();
    if(fork_result == -1) {
        perror("fork failure");
        exit(1);
    }
    if(fork_result > 0) {
        return;
    }
    printf("worker process (pid %d) started\n", getpid());

    signal(SIGCHLD, SIG_IGN);
    while(1) {
        fork_result = fork();
        if(fork_result == -1) {
            perror("fork failure");
            exit(1);
        }
                
        if(fork_result > 0) {
            int status;
            wait(&status);
            if(WEXITSTATUS(status) == 99) {
                printf("task.bin crashed with exit 99. worker (pid %d) aborting\n", getpid());
                exit(0);
            }
            *glob_var = *glob_var + 1;
            continue;
        }

        execl("./task.bin", "./task.bin", NULL);
        perror("exec failure");
        exit(1);
    }

    printf("bad place\n");
}

void child_died() {
    printf("signal handler\n");
    make_child();
}

int main(int argc, char** argv) {

    printf("boss process (pid %d) started\n", getpid());

    signal(SIGCHLD, child_died);
    glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    *glob_var = 0;
    int old_glob = 0;
    for(int i = 0; i < NUM_WORKERS; i++) {
        make_child();
    }

    while(1) {
        if(*glob_var != old_glob) {
            old_glob++;
            printf("%d tasks finished\n", old_glob);
        }
    }
}
