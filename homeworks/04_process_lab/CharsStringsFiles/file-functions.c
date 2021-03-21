#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./file-functions.h"

#define MAX_LINES 255

/* Reads the file with the given name, one character at a time and
 * writes the characters to the console. */
void printFileToConsole1(char *name) {
  int c;
  FILE *fp = fopen(name, "r");
  if (fp == NULL) {
    printf("error opening file: %s", name);
    return;
  }
  c = getc(fp);
  while (c != EOF) {
    putchar(c);
    c = getc(fp);
  }
  fclose(fp);
}

/* Reads a line from the given file, up to the maximum length given by
 * max.  Puts the result into the given line.  Returns the line length, 
 * or 0 for end of file. 
 * Based on getline from K&R, p. 165. */
int fgetline(FILE *file, char *line, int max) {
  if (fgets(line, max, file) == NULL) {
    return 0;
  } else {
    return strlen(line);
  }
}

/* Changes fileLines to point to an array of strings, one per each
 * line in the named file.  Returns the number of strings, or 0 if
 * there was a problem reading the file.
 */
int fgetlines(char* fileName, char ***fileLines) {
  /* This implementation uses dynamic memory allocation to ask the
   * operating system for just enough memory to hold the data.  The
   * code has extensive error checking of return values.  Don't worry
   * if you don't understand this code yet; we'll cover dynamic
   * memory allocation more in future sessions.  But it is good 
   * practice to see if you can work out what we're doing here.
   */

  int  len, e, lineCount = 0;
  char *nextLine = NULL;
  FILE *filePtr = NULL;
  char **reallocResult = NULL;
  char **result = (char **) calloc(MAX_LINES, sizeof(char *));
  if (result == NULL) {
    return 0;
  }

  filePtr = fopen(fileName, "r");
  if (filePtr == NULL) {
    free(result);
    return 0;
  }

  len = fgetline(filePtr, nextLine, MAX_LINE_LENGTH);
  while (len && lineCount < MAX_LINES) {
    result[lineCount] = nextLine;
    lineCount++;
    nextLine = (char *) calloc(MAX_LINE_LENGTH, sizeof(char));
    if (nextLine == NULL) {
      freelines(result, lineCount);
      return 0;
    }
    len = fgetline(filePtr, nextLine, MAX_LINE_LENGTH);
  }

  /* Frees the last line, which wasn't used. */
  free(nextLine);

  /* Closes the file. */
  e = fclose(filePtr);
  if (e == EOF) {
    /* If there's an error closing the file, then release all the memory
     * we asked for and return 0. */
    freelines(result, lineCount);
    return 0;
  }

  /* Shrinks the line array to just the size needed. */
  reallocResult = (char **) realloc(result, lineCount * sizeof(char *));
  if (reallocResult == NULL) {
    *fileLines = result;
  } else {
    *fileLines = reallocResult;
  }

  return lineCount;
}

/* Deallocates the memory for the given array of strings, where count
 * is the number of strings. */ 
void freelines(char *fileLines[], int count) {
  int i;
  for (i=0; i<count; i++) {
    free(fileLines[i]);
  }
  free(fileLines);
}


/* Reads the file with the given name, one line at a time and
 * writes the characters to the console. */
void printFileToConsole2(char *name) {
  char line[MAX_LINE_LENGTH];
  FILE *fp;
  int len;
  fp = fopen(name, "r");
  len = fgetline(fp, line, MAX_LINE_LENGTH);
  while (len) {
    printf("%s", line);
    len = fgetline(fp, line, MAX_LINE_LENGTH);
  }
  fclose(fp);
}

/* Reads the file with the given name in its entirety using dynamic
 * memory allocation.  Writes the content to the console. */
void printFileToConsole3(char *name) {
  char **fileLines;
  int i, lineCount = fgetlines(name, &fileLines);
  for (i=0; i<lineCount; i++) {
    printf("%s", fileLines[i]);
  }
  if (lineCount) {
    freelines(fileLines, lineCount);
  }
}
