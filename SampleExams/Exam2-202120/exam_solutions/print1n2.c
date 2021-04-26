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

The print_1n2 function as shown below will write a single number (1 or 2) to
a file specified by the input argument. You don't have to implement the part
that writes number into a file. You only need to call the function with
proper input argument.

A quick explanation about the input argument is that,

char* filename is a filename string (or say a pointer to a char array) of the
file it will write number into.

int num_to_write is the particular number it will print into the file. In
this case, it will be 1 or 2. For example, if you call the function with
filename "file0.txt" and num_to_write as 1, it will append 20 lines of "1" to
the file0.txt, i.e., each line is just a number 1.

int file_index is an index related to filename. For example, file0.txt has
the index of 0; file1.txt has the index of 1. Technically, you can extract
this info from filename. But it is just handy to have it readily available.
It is useful mostly for step 2.

STEP 1:(25 points)

We want you to create threads to write numbers into 3 files in parallel.
Specifically, for each file, we want to 1 thread to write number 1 and
another thread to write number 2. Therefore, in total, we need 6 threads for
the 3 files. The filenames must be "file0.txt", "file1.txt", and "file2.txt".

No worries about currency issue (i.e., semaphore stuff) for now. If you
implement this correctly, after running the program, you will generate three
files that each includes 40 lines of "1" or "2" in random order.

STEP 2: (15 points)

Use semaphore to enforce that the "1" and "2" in each file will always
alternate. Namely, there will be no consecutive lines of "1"s and "2"s.

HINT:
* You need arrays of semaphores.
* The file_index comes handy to identify which file a thread is handling.
* This is one type of semaphore problems we have worked on in a lecture and
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

sem_t done_1[NUM_FILES], done_2[NUM_FILES];

struct param {
    char* filename;
    int num_to_write;
    int file_index;
};


void* print_1n2(void* arg) {
  struct param* myparam = arg;
  int count = 20;
  while (count > 0) {
    printf("START Writing number %d to %s\n", myparam->num_to_write,
           myparam->filename);
    if (myparam->num_to_write == 1) {
      sem_wait(&done_2[myparam->file_index]);
    } else {
      sem_wait(&done_1[myparam->file_index]);
    }
    char buff[128];
    printf("Writing number %d to %s\n", myparam->num_to_write, myparam->filename);
    sprintf(buff, "echo \"%d\" >> %s", myparam->num_to_write, myparam->filename);
    system(buff);
    if (myparam->num_to_write == 1) {
      sem_post(&done_1[myparam->file_index]);
    } else {
      sem_post(&done_2[myparam->file_index]);
    }
    printf("FINISH Writing number %d to %s\n",myparam->num_to_write, myparam->filename);
    count--;
  }
  return NULL;
}



int main(int argc, char *argv[]) {
  int count = 0;
  pthread_t threads[NUM_FILES * 2];
  struct param thread_params[NUM_FILES * 2];
  char* filenames[] = {"file0.txt", "file1.txt", "file2.txt"};
  for(count = 0; count < NUM_FILES; count++){
      sem_init(&done_2[count], 0, 1);
      sem_init(&done_1[count], 0, 0);
  }

  for(count = 0; count < NUM_FILES * 2; count++){
    thread_params[count].filename = filenames[count/2];
    thread_params[count].file_index = count/2;
    thread_params[count].num_to_write = count % 2 + 1;
    pthread_create(&threads[count], NULL, print_1n2, &thread_params[count]);
  }

  for(count = 0; count < NUM_FILES * 2; count++){
    pthread_join(threads[count], NULL);
  }

  printf("All done\n");
  return 0;
}
