#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// TODO:
//  After running this code and understanding it, add the following line
//    sleep(3);
//  before the write call in the child, and answer the following question:
//
//    What is the expected behavior in the parent?

int main(int argc, char **argv)
{
  int fd[2];
  pid_t pid;
  int nbytes;
  char readbuff[128];
  const char *msg = "My name is Aloy";

  // STEP 1:
  //  MUST call pipe(fd) BEFORE you call fork.
  if(pipe(fd) < 0) {
    perror("PANIC: pipe failed");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if(pid < 0) {
    perror("PANIC: fork failed");
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {
    // child: I am the writer.

    // STEP 2: I will write so MUST close the reading end.
    close(fd[0]);

    // STEP 4: Write the message into the pipe
    write(fd[1],                      // the writing end of the pipe
          msg,                        // the message to write
          strlen(msg)                 // how many bytes to write
         );

    // STEP 6: Done writing, MUST close the writing end
    close(fd[1]);

    // leave
    exit(EXIT_SUCCESS);
  } else {
    // parent: I am the reader.

    // STEP 3: I will read so MUST close the writing end.
    close(fd[1]);

    // STEP 5: Read from the pipe
    nbytes = read(fd[0],    // where to read from
                  readbuff, // where to save the data read
                  128       // how many bytes to read at most
                 );
    if(nbytes > 0) {
      printf("Parent read %d bytes from the child\n", nbytes);
      // To be safe, null terminate your strings
      if(nbytes < 128)
        readbuff[nbytes] = 0;
      printf("Parent read the following message from the child: %s\n",
             readbuff);
    }

    // STEP 7: Done reading, MUST close the reading end
    close(fd[0]);

    // leave
    exit(EXIT_SUCCESS);
  }
}
