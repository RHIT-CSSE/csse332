/* myCopy.c -
 *	 
 * Program written by Matt Gold
 * modified by Archana Chidanandan
 * modified by JP Mellor
 * modififed by Delvin Defoe, March 20, 2008
 *
 */

#include <stdio.h>
#include <stdlib.h>


int ReadFromFile(char filename[], char chars[]);
int WriteToFile(char filename[], char chars[], int n);


int main(int argc, char *argv[]) {
  int charsRead;
  char fileChars[500];

  /*When debugging the child process, uncomment this line.
    When the parent process forks the child process, it will sleep for 30s.
    During that time, in a separate terminal window use "ps" to find the pid of
    the child process. Then invoke gdb and use "attach pid" to debug the child 
    process.
    sleep(30);
  */  
  if (argc != 3) {
    fprintf(stderr, "./myCopy <source-file> <destination-file>\n");
    exit(2);
  }

  int j = 0;
  while (j < argc) {
    printf(" argv[%d]  is %s\n", j, argv[j]);
    j++;
  }

  charsRead = ReadFromFile(argv[1], fileChars);

  fprintf(stderr, "In program myCopy. Have just finished reading "
          "from file %s....\n", argv[1]);

  WriteToFile(argv[2], fileChars, charsRead);

  fprintf(stderr, "In program myCopy. Have just finished writing "
          "to file %s .....\n", argv[2]);
  exit(0);
}

int ReadFromFile(char filename[], char *chars) {
  int charsRead = 0;
  FILE * readFile = NULL;

  readFile = fopen(filename, "r");
  if (readFile == NULL) {
    perror(filename);
    exit(3);
  }
  charsRead = fread(chars, 1, 500, readFile);
  fclose(readFile);
  return charsRead;
}

int WriteToFile(char filename[], char *chars, int n) {
  FILE * writeFile = NULL;

  writeFile = fopen(filename, "w");
  if (writeFile == NULL) {
    perror(filename);
    exit(3);
  }

  fwrite(chars, 1, n, writeFile);
  fclose(writeFile);
  return 1;
}
