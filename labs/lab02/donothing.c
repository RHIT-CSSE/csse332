#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {

    printf("Do nothing program started!\n");
    printf("EXECUTABLE '%s'\n", argv[0]);
    for(int i = 1; i < argc; i++) {
        printf("ARGUMENT %d: %s\n", i, argv[i]); 
    }
    printf("Pausing for 5 seconds...\n");
    sleep(5);
    printf("Do nothing program finished!\n");
    return 0;
}
