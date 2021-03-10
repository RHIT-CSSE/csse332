#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"


#define NUM_MAX 5
#define NUM_TO_DELETE 2

// A little pratice using arraylist defined in arraylist.h
//
// STEP 1
//
// The given code loads values into the arraylist, but does not
// actually display the values.
//
// Add a third loop below the first two that prints out the values of
// the arraylist.  Note that array_list_get_idx (the function you'll
// want to use to retrieve values) returns a voiad pointer.
//
// Example output:
//
// mylist (size 3) values:
// 1
// 2
// 5
// 
// STEP 2
//
// Having to store our ints in the nums array as well as the array
// list is kinda silly.  Instead, let's malloc each of the integers we
// put in our arraylist on the heap.  Be sure to use free to ensure
// the ints we delete in the middle loop aren't leaked.
//
// To make things obvious, print the malloced values and the freed
// values as below.  Note that %p is the preferred printf syntax for a
// pointer value
// 
// Example output:
// 
// malloced 0x7ffff84753a0 value 1
// malloced 0x7ffff84757d0 value 2
// malloced 0x7ffff84757f0 value 3
// malloced 0x7ffff8475810 value 4
// malloced 0x7ffff8475830 value 5
// freeing 0x7ffff8475810 value 4
// freeing 0x7ffff84757f0 value 3
// mylist (size 3) values:
// 1
// 2
// 5
// 
// STEP 3
//
// Actually, the arraylist has the capability to do our frees for us.
// The function pointer passed to array_list_new is a destructor
// that's called every time something is removed from the array list.
// Create a function of the appropiate kind and replace do_nothing
// with that function.  Then you should be able to remove your free
// code from the middle loop
//
// Again print what you're freeing to make it obvious.  Now you should
// see that the values remaining in the list at the end are freed
// after the print.
//
// malloced 0x7fffc89a13a0 value 1
// malloced 0x7fffc89a17d0 value 2
// malloced 0x7fffc89a17f0 value 3
// malloced 0x7fffc89a1810 value 4
// malloced 0x7fffc89a1830 value 5
// freeing 0x7fffc89a1810 value 4
// freeing 0x7fffc89a17f0 value 3
// mylist (size 3) values:
// 1
// 2
// 5
// freeing 0x7fffc89a13a0 value 1
// freeing 0x7fffc89a17d0 value 2
// freeing 0x7fffc89a1830 value 5


void do_nothing() {

}

void my_free_func(void* free_me) {
	printf("freeing %p value %d\n", free_me, * (int*) free_me);
	free(free_me);
}

int main() {
	int nums[NUM_MAX];
	struct array_list *mylist;

	// mylist = array_list_new(do_nothing);
	// we will fix do_nothing in the last step
	mylist = array_list_new(my_free_func);

	// adds a bunch of numbers to the list
	for(int i = 1; i <= NUM_MAX; i++) {
		// nums[i - 1] = i;
		// // remember, void pointers can only safely store pointer
		// // values.  So we can't insert our intvalues directly.
		// // Instead, we must insert pointers to the ints
		// array_list_add(mylist, &nums[i - 1]);

		int* value = malloc(sizeof(int));
		*value = i;
		printf("malloced %p value %d\n", value, *value);
		array_list_add(mylist, value);
	}

	// removes a couple randomly
	for(int i = 1; i <= NUM_TO_DELETE; i++) {
		int rand_index = rand() % array_list_length(mylist);

		// STEP 2 HINT: you'll need to retrieve the value using
		// get_idx before the remove so you can safely free

		// STEP 2 only : int* value = array_list_get_idx(mylist, rand_index);
		array_list_del_idx(mylist, rand_index, 1);
		// STEP 2 only: printf("freeing %p value %d\n", value, *value);
		// STEP 2 only: free(value);
	}

	printf("mylist (size %lu) values:\n", array_list_length(mylist));
	for(int i = 0; i < array_list_length(mylist); i++) {
		void *result_as_vp = array_list_get_idx(mylist, i);
		int *result = (int*) result_as_vp;
		printf("%d\n", *result);
	}

	// free the array list which may leak some values until step 3
	array_list_free(mylist);

	return 0;
}
