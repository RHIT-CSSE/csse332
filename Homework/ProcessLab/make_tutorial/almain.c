#include <stdio.h>
#include "arraylist.h"

/*

  This code can be built with

  gcc almain.c arraylist.c -o almain

  but it sure would be cool if somebody would build a makefile for it

 */

void do_nothing(void* data) {

}

int main() {

    char * foo = "hello";
    char * bar = "world";
    struct array_list *example;
    example = array_list_new(do_nothing);
    array_list_add(example, foo);
    array_list_add(example, bar);
    for(int i = 0; i < array_list_length(example); i++) {
        printf("%s\n", (char*) array_list_get_idx(example, i));
    }
}
