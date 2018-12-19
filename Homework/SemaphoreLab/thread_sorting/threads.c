#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "threads.h"
/* This is where you implement the core solution.
   by <Name>, <Date>
*/

int buffer[MAX_N_SIZE][MAX_BUFFER_SIZE];
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

/* Merges the sorted files into an output file using the two
 * dimensional output buffer */
void MergeAndOutputBuffer(char* outputFile) {
  FILE *outFile = fopen(outputFile, "w");
  if (outFile == NULL) {
    fprintf(stderr, "Unable to open the file %s\n", outputFile);
    exit(2);
  }
  int i;
  int64_t j;
  int indexes[MAX_N_SIZE], maxIndexes[MAX_N_SIZE];
  for (i = 0; i < n; i++) {
    for (j = 0; buffer[i][j] > 0; j++) {
    }
    maxIndexes[i] = j;
    indexes[i] = 0;
  }
  int smallIndex;
  j = 0;
  while (1) {
    smallIndex = -1;
    for (i = 0; i < n; i++) {
      if (indexes[i] < maxIndexes[i]) {
        smallIndex = i;
        break;
      }
    }
    if (smallIndex == -1) {
      break;
    }
    for (i = 1; i < n; i++) {
      if ((indexes[i] < maxIndexes[i]) &&
          (buffer[i][indexes[i]] < buffer[smallIndex][indexes[smallIndex]])) {
        smallIndex = i;
      }
    }
    fprintf(outFile, "%d\n", buffer[smallIndex][indexes[smallIndex]]);
    fflush(outFile);
    indexes[smallIndex]++;
    j++;
  }
  fclose(outFile);
}
