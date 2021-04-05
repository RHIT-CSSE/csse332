#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int order_num = 0;
void alarm_handler(){
    printf("Takes too long to cook. Order %d aborted.\n", order_num);
    exit(1);
}

int main(int argc, char** argv) {
    char command[10];
    int pipes[3];
    signal(SIGALRM, alarm_handler);
    while (1) {
        fgets(command, 10, stdin);
        if (command[0] == 'O') {
            //plug your code here
            order_num++;
            if(order_num > 3){
                printf("Accept No More Orders. Closing system now.\n");
                return 0;
            }
            int pipeResult[2];
            pipe(pipeResult);
            int pid = fork();
            if(pid == 0){
                int pid1 = fork();
                if(pid1 == 0){
                    char buff[10];
                    sprintf(buff, "%d", order_num);
                    execlp("./order.bin", "order.bin", buff, NULL);
                }
                wait(NULL);
                pid1 = fork();
                if(pid1 == 0){
                    char buff[10];
                    sprintf(buff, "%d", order_num);
                    execlp("./cook.bin", "cook.bin", buff, NULL);
                }
                alarm(4);
                int status;
                wait(&status);
                signal(SIGALRM, SIG_IGN);
                if(WEXITSTATUS(status) > 0){
                    printf("Order %d is aborted.\n", order_num);
                    return 0;
                }
                close(pipeResult[1]);
                char rbuff[10];
                int rlen = read(pipeResult[0], rbuff, 10);
                if(rlen > 0){
                    pid1 = fork();
                    if (pid1 == 0) {
                        char buff[10];
                        sprintf(buff, "%d", order_num);
                        execlp("./deliver.bin", "deliver.bin", buff, NULL);
                    }
                }
                return 0;
            }
            close(pipeResult[0]);
            pipes[order_num - 1] = pipeResult[1];

        } else if(command[0] == 'D'){
            int input_num = atoi(command+2);
            write(pipes[input_num - 1], "Go", 3);
        }
        else {
            printf("Invalid command. Shell terminated.\n");
            break;
        }
    }
    return 0;
}