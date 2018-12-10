/*
 * Sample application of chars, strings, and files.
 * by Curt Clifton, Rose-Hulman Institute of Technology.
 * Nov. 1, 2007.
 * Updated: Delvin Defoe, Rose-Hulman Institute of Technology.
 * Apr 8, 2014.
 *
 * Limericks from:
 *     http://www.grahamlester.com/
 *     http://www.funnypoets.com/poetry/maths.htm
 *     http://www.alphadictionary.com/fun/limericks.html
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "./file-functions.h"
#include "./string-functions.h"

#define JUSTIFICATION_WIDTH 72

/*
 * This helper function strips the trailing newline, if any, off the
 * given string. You do not need to call this function, but our test
 * code for ljust, rjust, and center uses it.
*/
void stripNewline(char *str) {
  int len = strlen(str);
  /* Handles unix newlines, and half of Windows newlines */
  if (str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
  /* Handles the other half of Windows newlines. */
  if (str[len - 2] == '\r') {
    str[len - 2] = '\0';
  }
}

/*
 * Tests the capitalize function by repeatedly calling it using successive
 * lines from the file with the given name.
 */
void test_capitalize(char *filename) {
  char **lines;
  printf("\n\nCapitalizing: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    capitalize(lines[i]);  /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the lower function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_lower(char *filename) {
  char **lines;
  printf("\n\nDowncasing: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    lower(lines[i]);  /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the upper function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_upper(char *filename) {
  char **lines;
  printf("\n\nUpcasing: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    upper(lines[i]);  /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the swapcase function by repeatedly calling it using successive 
 * lines from the file with the given name.
 */
void test_swapcase(char *filename) {
  char **lines;
  printf("\n\nSwapping case: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    swapcase(lines[i]);  /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the capwords function by repeatedly calling it using successive 
 * lines from the file with the given name.
 */
void test_capwords(char *filename) {
  char **lines;
  printf("\n\nCapitalizing words: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    capwords(lines[i]);  /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the ljust function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_ljust(char *filename) {
  char **lines;
  printf("\n\nLeft justifying: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    stripNewline(lines[i]);
    ljust(lines[i], JUSTIFICATION_WIDTH);  /* <-- test call to your code */
    printf("|%s|\n", lines[i]);
      /* <-- Prints "pipes" on either side to 
       * show justification. */
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the rjust function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_rjust(char *filename) {
  char **lines;
  printf("\n\nRight justifying: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    stripNewline(lines[i]);
    rjust(lines[i], JUSTIFICATION_WIDTH);  /* <-- test call to your code */
    printf("|%s|\n", lines[i]);
      /* <-- Prints "pipes" on either side to 
       * show justification. */
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the center function by repeatedly calling it using successive 
 * lines from the file with the given name.
 */
void test_center(char *filename) {
  char **lines;
  printf("\n\nCenter justifying: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    stripNewline(lines[i]);  /* <-- strips white space before justifying */
    center(lines[i], JUSTIFICATION_WIDTH);  /* <-- test call to your code */
    printf("|%s|\n", lines[i]);
      /* <-- Prints "pipes" on either side to
       * show justification. */
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the reverse function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_reverse(char *filename) {
  char **lines;
  printf("\n\nReversing: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    reverse(lines[i]);  /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the strip function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_strip(char *filename) {
  char **lines;
  printf("\n\nStripping: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  for (i=0; i<lineCount; i++) {
    strip(lines[i]);  /* <-- test call to your code */
    printf("|%s|\n", lines[i]);
        /* <-- The newline should have been
        * stripped from lines[i], so we add
        * one for printing. */
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the find function by repeatedly calling it using successive lines
 * from the file with the given name.
 */
void test_find(char *filename) {
  char **lines;
  printf("\n\nFinding in: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  char *location, *ptr;
  for (i=0; i<lineCount; i++) {
    location = find(lines[i], "on");  /* <-- test call to your code */
    if (location == NULL) {
      printf("%s", lines[i]);
    } else {
      for (ptr=lines[i]; ptr < location; ptr++) {
        putchar(*ptr);
      }
      printf("-->%s", location);
    }
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

/*
 * Tests the replace function by repeatedly calling it using successive 
 * lines from the file with the given name.
 */
void test_replace(char *filename) {
  char **lines;
  printf("\n\nReplacing in: %s\n", filename);
  int lineCount = fgetlines(filename, &lines);
  int i;
  /* print the original for comparison */
  for (i=0; i<lineCount; i++) {
    printf("%s", lines[i]);
  }
  printf("----\n");
  for (i=0; i<lineCount; i++) {
    replace(lines[i], "t", "f");   /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  printf("----\n");
  for (i=0; i<lineCount; i++) {
    replace(lines[i], "oo", "u");   /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  printf("----\n");
  for (i=0; i<lineCount; i++) {
    replace(lines[i], "fo", "2");   /* <-- test call to your code */
    printf("%s", lines[i]);
  }
  if (lineCount) {
    freelines(lines, lineCount);
  }
}

int main(int argc, char *argv[]) {
  int fileNum = 0;
  char *fileNames[] = {
    "Data/Bright.txt",
    "Data/Finney.txt",
    "Data/IBMvsSUN.txt",
    "Data/McGarr.txt",
    "Data/Beauty.txt",
    "Data/Ilene.txt",
    "Data/Liquor.txt",
    "Data/OldEdward.txt",
    "Data/MissKeyes.txt",
    "Data/Pythagoras.txt",
    "Data/IntroToC.txt",
    "Data/FluteTutor.txt",
  };

  /*
   * These printFile functions are declared in file-functions.h 
   * and defined in file-functions.c.  If you are interested in
   * how file I/O is done in C, you can study those files.  We
   * won't have time to cover file I/O in detail this term.
   */
  printf("By Character:\n");
  printFileToConsole1(fileNames[0]);

  printf("\n\nBy Line:\n");
  printFileToConsole2(fileNames[1]);

  printf("\n\nBy whole file:\n");
  printFileToConsole3(fileNames[2]);

  /*
   * These test functions test each of your required operations
   * in turn.
   */
  test_capitalize(fileNames[fileNum++]);
  test_lower(fileNames[fileNum++]);
  test_upper(fileNames[fileNum++]);
  test_swapcase(fileNames[fileNum++]);
  test_capwords(fileNames[fileNum++]);
  test_ljust(fileNames[fileNum++]);
  test_rjust(fileNames[fileNum++]);
  test_center(fileNames[fileNum++]);
  test_reverse(fileNames[fileNum++]);
  test_strip(fileNames[fileNum++]);
  test_find(fileNames[fileNum++]);
  /*
   * The replace function is optional.
   */
  test_replace(fileNames[fileNum++]);

  return 0;
}
