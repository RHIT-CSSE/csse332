#include "stdio.h"
#include <stdlib.h>


void void_pointer_example() {
  printf("\n\nvoid pointer stuff\n\n");
  void *my_void_pointer;
  int my_int = 77;
  double my_double = 17;
  
  //a void pointer can hold any pointer type
  my_void_pointer = &my_int;
  my_void_pointer = &my_double;
  
  //you must convert or cast back to another pointer type before use
  double *my_double_pointer;
  my_double_pointer = (double*) my_void_pointer;
  my_double_pointer = my_void_pointer;
  double val_as_double = *my_double_pointer;
  printf("double value: %f\n", val_as_double);
  

  //but of course if you convert incorrectly you can get weird results
  int *my_int_pointer;
  my_int_pointer = my_void_pointer;
  int val_as_int = *my_int_pointer;
  printf("int value: %d\n", val_as_int);
  
}

void malloc_example() {
  
  printf("\n\nmalloc stuff\n\n");
  
  // malloc allocates memory on the heap
  // you pass it the size to allocate in bytes
  // it will allocate that much memory for you
  // but don't compute the size directly - use sizeof
  
  void *malloc_result = malloc(4*sizeof(int));
  if(malloc_result == NULL) {
      //malloc can of course fail if you're out of memory
      exit(77);
  }
  int *data = malloc_result;
  for(int i = 0; i < 4; i++) {
    data[i] = i+1;
  }
  printf("data %d %d %d %d\n", data[0], data[1], data[2], data[3]);
  
  //note that there are generally no protections for accessing outside of alloced space
  printf("unsafely accessing data out of bounds %d\n", data[4]);
  
  //you must always free by passing the same pointer you got orignally from malloc
  //otherwise you will leak memory
  free(malloc_result);
  // be aware that freeing the same memory twice is unsafe and bad
  
  //again, you won't usually get an error if you do something stupid, just strange data corruption
  printf("unsafely accessing data after free %d %d %d %d\n", data[0], data[1], data[2], data[3]);
}

int example_1(double val1, double val2) {
  printf("calling example 1 %f %f\n", val1, val2);
  return 1;
}

int example_2(double val1, double val2) {
  printf("calling example 2 %f %f\n", val1, val2);
  return 2;
}

void example_3() {
  printf("example 3\n");
}

void function_pointer_example() {
  
    printf("\n\function pointer stuff\n\n");
  
  //declared type looks really weird, but go with it
  int (*my_fun_pointer)(double,double);
  void (*other_fun_pointer)();
  
  //you can assign a pointer to a function of the right signature
  my_fun_pointer = example_1;
  other_fun_pointer = example_3;
  
  //and then at some future time call that function through the pointer
  int result = my_fun_pointer(66,7);
  other_fun_pointer();
  
  //then reassign and call again if you wish
  my_fun_pointer = example_2;
  result = my_fun_pointer(66,7);
  
}

int main(void) {
  void_pointer_example();
  malloc_example();
  function_pointer_example();
  return 0;
}