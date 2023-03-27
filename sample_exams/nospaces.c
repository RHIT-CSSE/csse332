#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// This is a little exercise I wrote to make sure folks understand
// read and write before the 202330 Exam 1 makeup

int main(int argc, char** argv) {

    if(argc != 3) {
        printf("A utility that removes spaces from a file.  Usage: %s <inputfile> <outputfile>\n", argv[0]);
        return -1;
    }

    int in_fd = open(argv[1], O_RDONLY);
    if(in_fd == -1) { perror("unable to open read file"); exit(1); }
    int out_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0740);
    if(in_fd == -1) { perror("unable to open write file"); exit(1); }

    // the above code creates to file descriptors in_fd (input fie) and out_fd
    // (output file).  Use read and write to solve the problem.  I reccommend
    // you read/write only 1 character at a time for simplicity.  Check for spaces
    // like this: if(mychar == ' ')
    
    // YOUR CODE HERE

    close(in_fd);
    close(out_fd);
    return 0;
    
}
