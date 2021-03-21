/* 
 * This header file declares the string manipulation functions.
 */

#ifndef STRINGFUNCTIONS_H_
#define STRINGFUNCTIONS_H_

#endif /*STRINGFUNCTIONS_H_*/


/* 
 * Capitalizes the first character of the given string "in place".  
 * That is, the string is mutated.  (This is another difference between 
 * C and Python strings. Strings in Python are immutable.)
 */
void capitalize(char *str);

/*
 * Replaces every character of the given string with its lowercase version.
 * Using a function from ctype.h would be helpful here. 
 */
void lower(char *str);

/* 
 * Replaces every character of the given string with its uppercase version.
 * Using a function from ctype.h would be helpful here.
 */
void upper(char *str);

/*
 * Replaces every lowercase character of the given string with its uppercase 
 * version, replaces every uppercase character with its lowercase version, 
 * and leaves the rest unchanged.
 */
void swapcase(char *str);

/*
 * Capitalizes every word of the given string.  That is, every character 
 * following a space is converted to its uppercase version, as is the first 
 * character.
 */
void capwords(char *str);

/*
 * Pads the end of the given string with spaces so that it is justWidth 
 * characters long.  If str is already longer than justWidth, then it is 
 * left unchanged.
 */
void ljust(char *str, int justWidth);

/*
 * Pads the beginning of the given string with spaces so that it is 
 * justWidth characters long.  If str is already longer than justWidth, 
 * then it is left unchanged.
 */
void rjust(char *str, int justWidth);

/*
 * Equally pads the beginning and end of the given string with spaces so 
 * that it is justWidth characters long, with the original string contents 
 * centered in the length.  If str is already longer than justWidth, then 
 * it is left unchanged.
 */
void center(char *str, int justWidth);

/*
 * Removes any leading and trailing spaces from the given string.
 */
void strip(char *str);

/*
 * Reverses the characters of the given string in place.
 */
void reverse(char *str);

/*
 * Finds the first occurence of the search string within the str string.  
 * Returns a pointer to the occurence.  If the search string is not found, 
 * then returns the special NULL pointer.  
 * A string.h function would be helpful here.
 */
char *find(char *str, char *search);

/*
 * Replaces _every_ occurence of the old string in appearing in the str 
 * string with the new string.   
 */
void replace(char *str, char *old, char *new);
