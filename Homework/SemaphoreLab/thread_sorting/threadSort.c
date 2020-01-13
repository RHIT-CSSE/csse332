#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "threads.h"

// max number of its in input file
#define MAX_BUFFER_SIZE 100000
// max threads allowed
#define MAX_N_SIZE 100


/* other global variable instantiations can go here */

/* Uses a brute force method of sorting the input list. */
void BruteForceSort(int inputList[], int inputLength) {
  int i, j, temp;
  for (i = 0; i < inputLength; i++) {
    for (j = i+1; j < inputLength; j++) {
      if (inputList[j] < inputList[i]) {
        temp = inputList[j];
        inputList[j] = inputList[i];
        inputList[i] = temp;
      }
    }
  }
}

/* Uses the bubble sort method of sorting the input list. */
void BubbleSort(int inputList[], int inputLength) {
  char sorted = 0;
  int i, temp;
  while (!sorted) {
    sorted = 1;
    for (i = 1; i < inputLength; i++) {
      if (inputList[i] < inputList[i-1]) {
        sorted = 0;
        temp = inputList[i-1];
        inputList[i-1] = inputList[i];
        inputList[i] = temp;
      }
    }
  }
}

/* Merges two arrays.  Instead of having two arrays as input, it
 * merges positions in the overall array by re-ording data.  This 
 * saves space. */
void Merge(int *array, int left, int mid, int right) {
  int tempArray[MAX_BUFFER_SIZE];
  int pos = 0, lpos = left, rpos = mid + 1;
  while (lpos <= mid && rpos <= right) {
    if (array[lpos] < array[rpos]) {
      tempArray[pos++] = array[lpos++];
    } else {
      tempArray[pos++] = array[rpos++];
    }
  }
  while (lpos <= mid)  tempArray[pos++] = array[lpos++];
  while (rpos <= right)tempArray[pos++] = array[rpos++];
  int iter;
  for (iter = 0; iter < pos; iter++) {
    array[iter+left] = tempArray[iter];
  }
  return;
}

/* Divides an array into halfs to merge together in order. */
void MergeSort(int *array, int left, int right) {
  int mid = (left+right)/2;
  if (left < right) {
    MergeSort(array, left, mid);
    MergeSort(array, mid+1, right);
    Merge(array, left, mid, right);
  }
}

int main(int argc, char** argv) {

    if(argc < 4) {
        printf("not enough arguments!\n");
        exit(1);
    }

    // I'm reading the value n (number of threads) for you off the
    // command line
    int n = atoi(argv[1]);
    if(n <= 0 || n > MAX_N_SIZE) {
        printf("bad n value (number of threads) %d\n", n);
        exit(1);
    }
    
    int read_fd = open(argv[2], O_RDONLY);
    if(read_fd == -1) {
        perror("couldn't open file for reading");
        exit(1);
    }

    char buffer[100]; //we're reading ints, so this is plenty for a line
    int read_result;
    while((read_result = read(read_fd, buffer, 100))) {
        int data = atoi(buffer);
        // remove this line, its just to prove read works
        printf("I just read int %d from file\n", data);
        // this is where you'll divvy up the integers into buffers for
        // the various threads
    }
    if(read_result < 0) {
        perror("file read error");
        exit(1);
    } else {
        close(read_fd);
    }
    // this is where you'll dispatch threads for sorting
    // then wait for things to finish
    // then print the statistics

    // make one giant buffer capable of storing all your
    // output.

    // make a loop to copy data into it, then use the Merge function
    // to merge that data, then copy more data into it etc.

    // once everything is sorted, write it out to the file

    int output_fd;
    if ((output_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
        perror("Cannot open output file\n");
        exit(1);
    }

    dprintf(output_fd, "Just an example for writing an int %d\n", 7);
    close(output_fd);

}
