/*
 * Header file for the file functions used to read lines from a file.
 */

#ifndef FILEFUNCTIONS_H_
#define FILEFUNCTIONS_H_

#endif /*FILEFUNCTIONS_H_*/

/* The code for manipulating lines assumes files have a maximum line
 * length. */
#define MAX_LINE_LENGTH 255

/* Reads the file with the given name, one character at a time and
 * writes the characters to the console. */
void printFileToConsole1(char *name);

/* Reads a line from the given file, up to the maximum length given by
 * max.  Returns the line length, or 0 for end of file. 
 * Based on getline from K&R, p. 165. */
int fgetline(FILE *file, char *line, int max);

/* Reads the file with the given name, one line at a time and
 * writes the characters to the console. */
void printFileToConsole2(char *name);

/* Reads the file with the given name in its entirety using dynamic
 * memory allocation.  Writes the content to the console. */
void printFileToConsole3(char *name);

/* Changes fileLines to point to an array of strings, one per each
 * line in the named file.  Returns the number of strings, or 0 if
 * there was a problem reading the file.
 */
int fgetlines(char* fileName, char ***fileLines);

/* Deallocates the memory for the given array of strings, where count
 * is the number of strings. */ 
void freelines(char *fileLines[], int count);
