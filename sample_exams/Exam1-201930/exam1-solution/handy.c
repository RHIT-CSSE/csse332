#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "handy.h"

/* 

Takes an array of char*s of a given size.

Mallocs a copy of the array that is one larger, with the final element
of the array set to null.

*/
char** malloc_a_copy_that_ends_in_null(char** array, int size) {
    char** space = malloc(1 + size * sizeof(char*));
    for(int i = 0; i < size; i++) {
        space[i] = array[i];
    }
    space[size] = NULL;
    return space;
}
