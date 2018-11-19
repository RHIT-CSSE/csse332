/* 
 * A file copy utility program that mimics the LINUX cp command.
 * Author:  Delvin Defoe, Mar 21, 2012.
 */

#include <stdio.h>
#include <stdlib.h>

/* Maximum size of buffer for file input.*/
#define MAX_BUFFER_SIZE 500

/* Function prototypes */
int ReadFromFile(char fileName[], char *buffer);

int WriteToFile(char fileName[], char *buffer, int numCharsToWrite);

/* Reads file content and stores it in the buffer.
 * Returns the number of characters read.
 */
int ReadFromFile(char fileName[], char *buffer) {
  FILE *inputFile = fopen(fileName, "r");
  int count = 0;

  if (inputFile == NULL) {
    fprintf(stderr, "Unable to open the file %s\n", fileName);
    exit(2);
  }

  while (!feof(inputFile)) {
    buffer[count] = getc(inputFile);
    count++;
  }
  buffer[count - 1] = '\0';

  fclose(inputFile);

  return count - 1;
}

/* Writes numCharsToWrite characters from the buffer to the file. 
 * Returns the number of characters written. 
 */
int WriteToFile(char fileName[], char *buffer, int numCharsToWrite) {
  FILE *outFile = fopen(fileName, "w");
  int i = 0;

  if (outFile == NULL) {
    fprintf(stderr, "Unable to open the file %s\n", fileName);
    exit(2);
  }

  for (i=0; i < numCharsToWrite; i++) {
    putc(buffer[i], outFile);
  }

  fclose(outFile);
  return i;
}

/* Runs the file copy utility program. */
int main(int argc, char *argv[]) {
  char buffer[MAX_BUFFER_SIZE];
  int charsCount;

  if (argc != 3) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, ">>>  ./filecopy <source_file> <destination_file> \n");
    exit(1);
  }

  charsCount = ReadFromFile(argv[1], buffer);
  printf("*************** About to copy file ***************\n");
  printf("Read %d chars from input file %s.\n", charsCount, argv[1]);
  charsCount = WriteToFile(argv[2], buffer, charsCount);
  printf("Copied %d chars to output file %s.\n", charsCount, argv[2]);
  printf("*************** Finish copying file ***************\n");
  return 0;
}
