#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
  // look at forkExample for help!
  int i = 0;
  for(i = 0;i < 10; i++){
          int fresult = fork();
          if(fresult == 0){
                  
              char output[50];
              snprintf(output, 50, "I'm child %d",i);
              execlp("./buffalosay.bin", "./buffalosay.bin", output, NULL);
              perror("error execing!");
              exit(99);                      
          }
  }
  int status;
  for(i = 0;i < 10; i++){
          wait(&status);
          printf("One child returns with exit code %d\n",WEXITSTATUS(status));
  }
  printf("exiting\n");

  return 0;
}
