#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// max threads allowed, initially ignored
#define MAX_THREADS 2

// max number of characters in the file
// you can use this if you wish
#define MAX_FILE_SIZE 1000

/*

So this is code that reads in numerical data from a file and sums the
digits.  So for example if the file contains '12345' the program will
output 'the total file sum is 15'.  It handles the summing in "chunks"
where each chunk is a block of digits.  So if you call the program
with a chunk size of 2:

    parallel_add 2 data.dat

it will do the sum in 2 digit groups.  So the output looks like

    summing data of size 2
    finished summing data of size 2 result 3
    summing data of size 2
    finished summing data of size 2 result 7
    summing data of size 1
    finished summing data of size 1 result 5
    the total file sum is 15

Of course regardless of the chunk size the overall file sum should be
the same.

STEP 1 (20 POINTS):

Modify the code so that the individual chunks sum in paralell threads.
Note that right now the paramater array passed to slow_sum is reused
on subsequent calls to slow_sum, so you'll have to find a different
way to ensure each thread gets its own data.  You can do this in
whatever way you see fit, but if you use malloc you must ensure that
malloc data is correctly freed.  Feel free to use the MAX_FILE_SIZE
variable to help you if you need to bound the max data that can be
read.

For this step, you don't have to worry about computing the final sum
but your threads should run in parallel and your main should wait for
all the threads to complete before exiting.

You can change slow sum if you wish, but keep the sleep and the
printing.

Note that at this stage more than MAX_THREADS threads can be created.

Example output:

    ./parallel_add 2 testfile1.dat         
    summing data of size 2
    summing data of size 1
    summing data of size 2
    finished summing data of size 2 result 3
    finished summing data of size 1 result 5
    finished summing data of size 2 result 7
    the total file sum is 0

STEP 2 (10 POINTS):

Modify your code so that the total file sum is computed correctly.
This can be with any mechanism you choose (e.g. semaphore protected
global, thread return results, etc.) but you have to use the
intermediate calculations from the threads (i.e. can't just re-sum the
whole array).

Example output:

    ./parallel_add 2 testfile1.dat    
    summing data of size 2
    summing data of size 2
    summing data of size 1
    finished summing data of size 2 result 3
    finished summing data of size 2 result 7
    finished summing data of size 1 result 5
    the total file sum is 15

STEP 3 (10 POINTS):

For this final stage, we want to limit the number of threads that run
at the same time.  For example, if we have a file with 1000 chunks, we
don't want to create 1000 threads.  Instead, we want to start
NUM_THREADS threads, and then when one of those threads finishes,
starts another etc.

We don't want to assume that the first thread to start is the first
thread to finish, so we can't just join on the first pthread created.
Actually, we can wait on joining till the end if we want - we're
concerned about the CPU load of running too many threads, not the
memory cost of unjoined threads.

Use semaphores to enforce this constraint.

Example output:

    ./parallel_add 2 testfile1.dat    
    summing data of size 2
    summing data of size 2
    finished summing data of size 2 result 3
    finished summing data of size 2 result 7
    summing data of size 1
    finished summing data of size 1 result 5
    the total file sum is 15

For all steps, note we will run your program with valigrind and with
testfile2.dat which should sum to 200 (has 200 1s).

 */


// expects a array of digits and sums them slowly.
//
// for your calling convenience, this code assumes the first array
// entry is the size of the array

u_int32_t slow_sum(u_int8_t data[]) {
    u_int8_t size = data[0];
    printf("summing data of size %d\n", size);
    sleep(1);
    u_int32_t sum = 0;
    for(u_int8_t i = 1; i < size + 1; i++) {
        sum = sum + data[i];
    }
    printf("finished summing data of size %u result %u\n", size, sum);
    return sum;
        
}


int main(int argc, char** argv) {

    if(argc < 3) {
        printf("not enough arguments!\n");
        exit(1);
    }

    // I'm reading the chunk size (i.e. the size that an individual
    // thread will sort) on the command line
    int chunk_size = atoi(argv[1]);
    if(chunk_size <= 0 || chunk_size > 255) {
        printf("bad chunk size%d\n", chunk_size);
        exit(1);
    }

    // opening the file which is the 2nd argument
    int read_fd = open(argv[2], O_RDONLY);
    if(read_fd == -1) {
        perror("couldn't open file for reading");
        exit(1);
    }

    u_int8_t chunk_and_size[chunk_size + 1];

    int read_result;
    u_int32_t total_sum = 0;
    while((read_result = read(read_fd, chunk_and_size + 1, chunk_size))) {
        chunk_and_size[0] = read_result;
        u_int8_t *chunk = chunk_and_size + 1;
        for(int i = 0; i < read_result; i++) {
            if(chunk[i] == '\n') continue; //we ignore newlines
            if(chunk[i] < '0' || chunk[i] > '9') {
                printf("invalid digit (ascii code %u) read\n", chunk[i]);
                break;
            }
            chunk[i] = chunk[i] - '0'; // this converts the digit from
                                       // ascii to the corresponding
                                       // int value.  i.e. the
                                       // character '7' becomes
                                       // intvalue 7
        }
        total_sum = total_sum + slow_sum(chunk_and_size);
    }
    
    if(read_result < 0) {
        perror("file read error");
        exit(1);
    } else {
        close(read_fd);
    }

    printf("the total file sum is %u\n", total_sum);
    
}
