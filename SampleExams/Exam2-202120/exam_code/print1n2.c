#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/**

In this program, threads will write certain numbers into files. 

The print_1n2 function as shown below will write several lines of number
(either 1 or 2) to a file specified by the input argument. You don't have to
implement the part that writes numbers into a file. You only need to call the
function with proper input argument.

A quick explanation about the input argument is that,

char* filename is a filename string (or say a pointer to a char array) of the
file to write number into.

int num_to_write is the particular number it will print into the file. In
this case, it will be 1 or 2. For example, if you call the function with
filename "file0.txt" and num_to_write as 1, it will append 20 lines of "1" to
the file0.txt, i.e., each line is just a number "1".

int file_index is an index related to the filename. For example, file0.txt
has the index of 0; file1.txt has the index of 1. Technically, you can
extract this info from the filename. But it is just handy to have it readily
available. It will be useful in step 2.

STEP 1:(25 points)

We want you to create threads to write numbers into 3 files in a parallel
manner. Specifically, for each file, we want to use 1 thread to write number
1 and another thread to write number 2. Therefore, in total, we need 6
threads for the 3 files. The filenames must be "file0.txt", "file1.txt", and
"file2.txt".

No worries about currency issue (i.e., semaphore stuff) for now. If you
implement this correctly, after running the program, you will generate three
files that each includes 40 lines of "1" or "2" in random order.

STEP 2: (15 points)

Use semaphore to enforce that the "1" and "2" in each file will always
alternate. It will start with "1" at the first line and then alternate
between "2" and "1" throughout the file. Namely, there will be no consecutive
lines of "1"s and "2"s.

HINT:
* You need arrays of semaphores.
* The file_index comes handy to identify which file a thread is handling.
* We have worked on this type of semaphore problem in a lecture and
several assignments.

After done, all files should include 40 lines that shows this pattern:
1
2
1
2
1
2

**/

#define NUM_FILES 3

// Declare semaphores here for step 2

struct param {
    char* filename;
    int num_to_write;
    int file_index;
};


void* print_1n2(void* arg) {
  // Add code in this function for step 2
  struct param* myparam = arg;
  int count = 20;
  while (count > 0) {
    printf("START Writing number %d to %s\n", myparam->num_to_write, myparam->filename);

    char buff[128];
    printf("Writing number %d to %s\n", myparam->num_to_write, myparam->filename);
    sprintf(buff, "echo \"%d\" >> %s", myparam->num_to_write, myparam->filename);
    system(buff);
  
    printf("FINISH Writing number %d to %s\n",myparam->num_to_write, myparam->filename);
    count--;
  }
  return NULL;
}



int main(int argc, char *argv[]) {
  char* filenames[] = {"file0.txt", "file1.txt", "file2.txt"};
  //feel free to use this array to loop thru the filenames

  //plug in your code here




  printf("All done\n");
  return 0;
}
