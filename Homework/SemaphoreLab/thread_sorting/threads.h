/* Implement the core solution in the .c file.
   by <Name>, <Date>
*/

#define MAX_BUFFER_SIZE 100000
#define MAX_N_SIZE 100

extern int buffer[MAX_N_SIZE][MAX_BUFFER_SIZE];
/* other global variable declarations can go here */

void BruteForceSort(int inputList[], int inputLength);
void BubbleSort(int inputList[], int inputLength);
void MergeSort(int *array, int left, int right);
void Merge(int *array, int left, int mid, int right);
void MergeAndOutputBuffer(char* outputFile);
