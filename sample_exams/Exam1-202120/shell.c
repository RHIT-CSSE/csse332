#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char** argv) {
    char command[10];
    while (1) {
        fgets(command, 10, stdin);
        if (command[0] == 'O') {
            //plug your code here
        } else if(command[0] == 'D'){
            //add code here for part 5
            int input_num = atoi(command+2);
        }
        else {
            printf("Invalid command. Shell terminated.\n");
            break;
        }
    }
    return 0;
}