#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <signal.h>
#include "handy.h"


char* gcc_name = "./slow_gcc";
char** argv_copy;


/*
Edits a given string to replace ".c" with ".o"

E.g. hello.c becomes hello.o
 */
void replace_dotc_with_doto(char* str) {
    char *location = strstr(str, ".c");
    if(location == NULL) return;
    location[1] = 'o';
}

void handle_signal(int signal) {
    int cur = 1;
    
    //let all children finish
    while(wait(NULL) > 0);
    printf("cleaning up files...\n");

    while(argv_copy[cur] != NULL) {
        replace_dotc_with_doto(argv_copy[cur]);
        int result = fork();
        if(result == 0) {
            execlp("rm", "rm", "-v", argv_copy[cur], NULL);
        }
        cur++;
    }
    exit(4);
}

int max_gccs = 3; //only used for this last part

int main(int argc, char** argv) {

    int result;
    int start_counter = 0;
    int end_counter = 0;
    int files = argc - 1;
    
    signal(SIGINT, handle_signal);
    
    argv_copy = malloc_a_copy_that_ends_in_null(argv, argc);
    argv_copy[0] = gcc_name;
    
    while(end_counter < files) {
    
        if(start_counter < files && start_counter - end_counter < max_gccs) {

            result = fork();
            if(result < 0) exit(99);
            if(result != 0) {
                start_counter++;
                continue;
            }

            execlp(gcc_name, gcc_name, "-c", argv[start_counter + 1], NULL);

        } else {
            int status;
            wait(&status);
            end_counter++;
            if(WEXITSTATUS(status) != 0) {
                printf("child failed...aborting\n");
                exit(1);
            }
        }
        
    }

    for(int i = 1; i < argc; i++) {
        replace_dotc_with_doto(argv_copy[i]);
    }
    
    execvp(gcc_name, argv_copy);
    
}
