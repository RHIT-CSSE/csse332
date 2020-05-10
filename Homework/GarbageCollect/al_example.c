#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"


#define NUM_MAX 5
#define NUM_TO_DELETE 2

// An example of using the provided array list class


void do_nothing() {

}

void my_free_func(void* free_me) {
    printf("freeing %p value %d\n", free_me, * (int*) free_me);
    free(free_me);
}

int main() {
    int nums[NUM_MAX];
    struct array_list *mylist;

    // you initialize the array list with a function you'd like to be
    // called when an item is removed from the array list.  If your
    // desire is that removing an item should free the memory, do as below.

    // if on the other hand your pointers are used in other places,
    // use something like do_nothing above

    mylist = array_list_new(my_free_func);

    
    // remember only access the array list using built in array list
    // functions (for size, getting particular elements etc).  See my
    // examples below or look in arraylist.h for what functions you
    // have.
    
    // adds a bunch of numbers to the list
    for(int i = 1; i <= NUM_MAX; i++) {
        // the arraylist can only store void pointers, not structs or
        // strings or anything else!
        //
        // that means that if you want to use the arraylist to store
        // something different, you store void pointers of that thing
        // rather than the thing directly

        // frequently, you'll malloc the things you want to store (as
        // I do here) and then when they are removed from the list you
        // make the list free them with the your free function passed
        // in initialization
        
        int* value = malloc(sizeof(int));
        *value = i;
        printf("malloced %p value %d\n", value, *value);
        array_list_add(mylist, value);
    }

    // removes a couple randomly
    for(int i = 1; i <= NUM_TO_DELETE; i++) {
        int rand_index = rand() % array_list_length(mylist);

        array_list_del_idx(mylist, rand_index, 1);
    }

    printf("mylist (size %lu) values:\n", array_list_length(mylist));
    for(int i = 0; i < array_list_length(mylist); i++) {
        void *result_as_vp = array_list_get_idx(mylist, i);
        int *result = (int*) result_as_vp;
        printf("%d\n", *result);
    }

    // you must free your array list when done or you'll leak its
    // internal data memory.  This will also call your free function
    // on any values that remain in the list
    array_list_free(mylist);
    
    return 0;
}
