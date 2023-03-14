#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
Here is some code that uses fork to speed up factoring.  Yes, I agree
it factors in a super dumb way.

First:
  Understand the code

Second:
  Modify this code so that rather than have the 2 child processes
  individually print out the factors discovered, instead the child
  processes IPC communicate back to the parent process which prints them
  out. (In a more realistic example, the parent would use the results in
  its own calculations)

  Its fine if doing this changes the ordering of the solutions printed
  out.

  Take a look at exercise1 for pipes sample uses.
*/

int main(void)
{
    unsigned long long int target, i, start = 0;
    printf("Give a number to factor.\n");
    // 18446744073709551615 is max, I think
    // I think 666666662 gives good results
    scanf("%llu",&target);

    int fpip[2];

    if(pipe(fpip) == -1)
        perror("Failed to create pipe.");

    int pid = fork();
    if(pid == 0) {
        start = 2;
        close(fpip[0]);
    } else {
        pid = fork();
        if(pid == 0) {
            start = 3;
            close(fpip[0]);
        } else {
            // I'm the parent
            close(fpip[1]);
            wait(NULL);
            wait(NULL);

            int recv_len = 1;
            char buff[128];
            while(recv_len > 0){
                recv_len = read(fpip[0],buff,128);
                write(0,buff,recv_len);
            }
            close(fpip[0]);
            printf("Finished.\n");
            return 0;
        }
    }

    for(i = start; i <= target/2; i = i + 2) {
        if(target % i == 0) {
            char buff[128];
            sprintf(buff,"%llu is a factor\n", i);
            write(fpip[1],buff,strlen(buff));
        }
    }
    close(fpip[1]);
    return 0;
}
