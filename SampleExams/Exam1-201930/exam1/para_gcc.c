#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <signal.h>
#include "handy.h" // <== not needed now but you'll likely want it

// the name of the gcc version we want to use
char* gcc_name = "./slow_gcc";

//just a little utility that replaces
//a .c with a .o in a string
//
// e.g. "myfile.c" becomes "myfile.o"
void replace_dotc_with_doto(char* str) {
    char *location = strstr(str, ".c");
    if(location == NULL) return;
    location[1] = 'o';
}

// you won't use this till the last part
int max_gccs = 2;

int main(int argc, char** argv) {

    for(int i = 1; i < argc; i++) {
    
        printf("I would like to run: %s -c %s\n", gcc_name, argv[i]);
        // a sleep so you can see if it's running in paralell
        sleep(3);
        printf("done running: %s -c %s\n", gcc_name, argv[i]);

    }
    
}
