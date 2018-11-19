/* 
 * This file is where the declared string functions are to be implemented. 
 * by YOUR_NAME_HERE, on THE_DATE_HERE  
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "./file-functions.h"
#include "./string-functions.h"

#define FALSE 0
#define TRUE 1

typedef int bool;

/* 
 * Capitalizes the first character of the given string "in place".  
 * That is, the string is mutated.  (This is another difference between 
 * C and Python strings. Strings in Python are immutable.)
 */
void capitalize(char *str) {
  *str = toupper(*str);
}

/*
 * Replaces every character of the given string with its lowercase version.
 * Using a function from ctype.h would be helpful here. 
 */
void lower(char *str) {
  char *p;
  for (p = str; *p != '\0'; p++) {
    *p = tolower(*p);
  }
}

/* 
 * Replaces every character of the given string with its uppercase version.
 * Using a function from ctype.h would be helpful here.
 */
void upper(char *str) {
  char *p;
  for (p = str; *p != '\0'; p++) {
    *p = toupper(*p);
  }
}

/*
 * Replaces every lowercase character of the given string with its uppercase 
 * version, replaces every uppercase character with its lowercase version, 
 * and leaves the rest unchanged.
 */
void swapcase(char *str) {
  char *p;
  for (p = str; *p != '\0'; p++) {
    if (isupper(*p)) {
      *p = tolower(*p);
    } else if (islower(*p)) {
      *p = toupper(*p);
    }
  }
}

/*
 * Capitalizes every word of the given string.  That is, every character 
 * following a space is converted to its uppercase version, as is the first 
 * character.
 */
void capwords(char *str) {
  char *p;
  bool afterSpace = TRUE;
  for (p = str; *p != '\0'; p++) {
    if (afterSpace) {
      *p = toupper(*p);
    }
    afterSpace = isspace(*p);
  }
}

/*
 * Pads the end of the given string with spaces so that it is justWidth 
 * characters long.  If str is already longer than justWidth, then it is 
 * left unchanged.
 */
void ljust(char *str, int justWidth) {
  /* This error check makes sure that we don't try to add more spaces than
   * the string can hold. */
  if (justWidth > MAX_LINE_LENGTH) {
    return;
  }
}

/*
 * Pads the beginning of the given string with spaces so that it is 
 * justWidth characters long.  If str is already longer than justWidth, 
 * then it is left unchanged.
 */
void rjust(char *str, int justWidth) {
  /* This error check makes sure that we don't try to add more spaces than
   * the string can hold. */
  if (justWidth > MAX_LINE_LENGTH) {
    return;
  }
}

/*
 * Equally pads the beginning and end of the given string with spaces so 
 * that it is justWidth characters long, with the original string contents 
 * centered in the length.  If str is already longer than justWidth, then 
 * it is left unchanged.
 */
void center(char *str, int justWidth) {
  /* This error check makes sure that we don't try to add more spaces than
   * the string can hold. */
  if (justWidth > MAX_LINE_LENGTH) {
    return;
  }
  int len = strlen(str), leftSpaces;
  int spacesToAdd = justWidth - len;
  if (spacesToAdd < 0)
    return;
  leftSpaces = spacesToAdd / 2;
  rjust(str, len + leftSpaces);
  ljust(str, justWidth);
}

/*
 * Removes any leading and trailing spaces from the given string.
 */
void strip(char *str) {
  int startIndex;
  int endIndex;
  int i, len = strlen(str);

  /* finds first non-space character */
  for (startIndex=0; isspace(str[startIndex]); startIndex++) {}

  /* finds last non-space character */
  for (endIndex=len-1; isspace(str[endIndex]); endIndex--) {}

  /* copies characters back */
  for (i=startIndex; i<=endIndex; i++) {
    str[i-startIndex] = str[i];
  }
  str[i - startIndex] = '\0';
}

/*
 * Reverses the characters of the given string in place.
 */
void reverse(char *str) {
  char result[MAX_LINE_LENGTH];

  /* Copy characters from str to result in the right order.*/
  int i;
  int len = strlen(str);
  for (i=0; i < len; i++) {
    result[len-1-i] = str[i];
  }
  result[i] = '\0';
  strcpy(str, result);
}

/*
 * Finds the first occurence of the search string within the str string.  
 * Returns a pointer to the occurence.  If the search string is not found, 
 * then returns the special NULL pointer.  
 * A string.h function would be helpful here.
 */
char *find(char *str, char *search) {
  return strstr(str, search);
}

/*
 * Replaces _every_ occurence of the old string in appearing in the str 
 * string with the new string.   
 */
void replace(char *str, char *old, char *new) {
  char result[MAX_LINE_LENGTH];
  char *searchStart = str;
  char *endOfResult = result;
  char *match;
  int newLength = strlen(new);
  int oldLength = strlen(old);
  int remaining, addLength;

  match = find(searchStart, old);
  while (match != NULL) {
    addLength =  match - searchStart;
    strncpy(endOfResult, searchStart, addLength);
    endOfResult += addLength;
    strcpy(endOfResult, new);
    endOfResult += newLength;
    searchStart = match + oldLength;
    match = find(searchStart, old);
  }
  remaining = strlen(str) - (searchStart - str);
  if (remaining > 0) {
    strcpy(endOfResult, searchStart);
  }
  strcpy(str, result);
}

