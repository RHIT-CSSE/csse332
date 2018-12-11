#include <unistd.h> // execlp()
#include <stdio.h>  // perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(void) {
    char *const argv[] = {"/bin/ls","-l",NULL};
    /* execlp("ls", "ls", "-l", NULL); */
    execv(argv[0],argv);
    perror("Return from execlp() not expected");
    exit(EXIT_FAILURE);
}

