---
layout: post
title: Review of C Programming
readtime: true
date: Sun Nov 27 18:13:13 2022 
---

# Learning Objectives

At the end of this lecture, you should be able to:
- Define pointers in C and implement basic pointer manipulation operations. 
- Define and manipulate void pointers.
- Apply the concepts of pointers to functions through the use of function
  pointers. 
- Apply basic memory management operations through the use of the `malloc` and
  `free` routines. 

---

# Topics:

In this lecture, we will cover the following topics:
- C pointers.
- C memory management.

---

# Notes


<!-- vim-markdown-toc GFM -->

  * [Basics of C Pointers](#basics-of-c-pointers)
    * [Pointers and Types](#pointers-and-types)
    * [Casting C Pointers](#casting-c-pointers)
* [Pointer Arithmetic](#pointer-arithmetic)
* [C Memory Management](#c-memory-management)
  * [Allocating Memory](#allocating-memory)
  * [Deallocating Memory](#deallocating-memory)
  * [The C Memory Commandments](#the-c-memory-commandments)
* [Function Pointers](#function-pointers)

<!-- vim-markdown-toc -->

## Basics of C Pointers

- ❓ Let's say you want to order some pizza, what would you need to give the
  delivery person other than your order?
- __Definition__: A pointer in C is a variable that contains the address of
  another variable.
  - A pointer is your way for ordering pizza in C programming. 
- To define a pointer in your code, use the `*` operator in the variable's
  declaration. 
  - Example, `int *p` declares a pointer `p` that points to another variable of
    an integer type.
    - Note that `p` can also be a pointer to an __array__ of integers. In
      actuality, it points to the __first element__ of the array, the rest of
      the elements can be accessed by offsetting from that pointer. 
  - Similarly, `double *p2` declares a pointer `p2` that points to another
    variable  of type `double`. 
  - Another example would be `struct person *p3` declares a pointer `p3` that
    points to a structure of type `struct person`. 

- Given a variable `x`, we can use the `&` operator to obtain the address of
  `x`. 
  - For example, `&x` is the address of `x` in memory, so we could do something
    like the following to declare a pointer to x:
    ```c
    int *p = &x;
    ```
- To follow a pointer to go to its address -- we refer to this operation as
  __dereferencing__ a pointer --, we use the `*` operator.
  - For example, `*p` when used in an expression returns the variable to which
    `p` points to in memory. So we can write something like:
    ```c
    int *p = &x;
    int y = *p; // now y becomes equal to x
    ```

- To print a pointer (i.e., the actual address), you can use the `%x` modifier
  in the `printf` format specifier as follows:
  ```c
  printf("%x points to %lf in memory.", p, *p);
  ```


### Pointers and Types

- Generally, pointers contain the address of a variable of a certain type, that
  type is typically the one used when declaring the pointer. 
- However, we can define `void` pointers, which are pointers that are not
  associated with a certain data type. 
  - In other words, a `void` pointer can hold the address of a variable of any
    type. 
- When dereferencing a `void` pointer, you __must__ typecast it into a specific
  pointer type. 
  - The benefit of using a `void` pointer is that it can point to different
    things in memory at different times in the code. 
- To typecast a pointer, it must be cast a __pointer__ of another type.
  - For example,
    ```c
    void *p = &x;
    int *q = (int *)p;
    ```
- Here is an example of using and casting a `void` pointer:
```c
int x = 3;
double y = 3.14;
void *p;
p = &x;
printf("p = %x, *p = %d\n", p, *(int *)p);
p = &y;
printf("p = %x, *p = %lf\n", p, *(double *)p);
```

### Casting C Pointers

- The `gcc` compiler will not complain if you cast a pointer into another
  pointer associated with a different type. 
- For example, given a pointer to an integer `int *p`, it is completely legal to
  cast `p` into a pointer to a character (or anything else actually). 
  ```c
  int *p = &x;
  char *cptr = (char *)p;
  printf("p = %x, cptr = %x\n", p, cptr);
  printf("*p = %d, *cptr = %c\n", *p, *cptr);
  ```
  - ❓ Can you guess what the outcome of the above program is?

# Pointer Arithmetic

- Pointers support operations to move around in memory, this is achieved through
  the use of pointer arithmetic. 
  - You can add or subtract pointer addresses to point to different areas in
    memory. 
- Consider the following array of integers, `int array[] = {1, 2, 3, 4};`. 
  - As a matter of fact, `array` is nothing but an integer pointer that points
    to the __first integer__ in the memory area that contains the numbers `1, 2,
    3, 4`. 
  - Therefore, it is legal to define another variable `p` as `int *p = array;`
    and then use `p` in the same way you would use `array`. 
- Adding 1 to `array` (or equivalently, to `p`) corresponds to moving `array` to
  point to the __next (or second) integer__ in the memory area that contains the
  array. 
  - In other words, `int *p2 = array + 1` is an integer pointer that points to
    the second element in the array, which is the constant `2`. 
- 🏃 What would the expression `(array + 2) == &(array[2])` evaluate to?
  true of false?

- Similarly, consider the character array `char carray[] = {'a', 'b', 'c'};`
  - We can define the character pointer `c` as `char *c = carray;`
- Adding 1 to `carray` would evaluate to a pointer to the second element (i.e.,
  `b`) in the original array. 

- 🏃 Assume that we have a pointer `p` to an array of integers that
  contains `{1, 2, 3, 4}`, and let's assume that `p` contains the address
  `0x00000004`. 
  - Let's define another pointer `p2` as `int *p2 = p + 1`, what would be the
    address contained in `p2`?

- 🏃 Repeat the previous exercise assuming `p` is a pointer to an array of
  characters, what would be the address contained in `p + 1`?

- __General Rule__: Adding `1` to an integer pointer `p` is equivalent to adding
  `sizeof(int)` bytes to the address contained in `p`. 
- Similarly, adding `1` to a character pointer `c` is equivalent to adding
  `sizeof(char)` bytes to the address contained in `c`. 

# C Memory Management

- 🏃 Consider the following C code:
  ```c
  int *p;

  printf("p points to the value %d in memory\n", *p);
  ```
  - What would happen when we run the above program?

## Allocating Memory

- So we need a way to initialize a pointer. 
- One way to do it is to assign it to the address of an already existing
  variable as follows:
  ```c
  int x = 3;
  int *p = &x;
  ```
- But that is very limiting, we need a way to allocate memory (i.e., ask the
  operating system for some memory) and save the address returned in a pointer. 
  - The way to do that is using the `malloc` function. 

- The signature of the `malloc` function is as follows:
  ```c
  void *malloc(size_t size);
  ```
  - where `size` is the __number of bytes__ you would like the operating system
    to allocate for you. 

- Using your terminal, type `man malloc` to check the documentation for the
  `malloc` function. 

## Deallocating Memory

- When we are done using a piece of memory, we must return this piece of memory
  to the OS so that it can allocate it to someone else. 
- Therefore, given an allocated pointer `p`, use `free(p)` to free the memory
  area that `p` points to and return it to the OS. 
- The signature of the `free` function is as follows:
  ```c
  void free(void *ptr);
  ```
- Using your terminal, type `man free` to check the documentation for the `free`
  function. 

- 🏃 Why is it bad to do the following:
  ```c
  int x = 3;
  int *p = &x;
  free(p);
  ```


- __NOTE__: We are abusing notation a bit here by saying that `malloc` and
  `free` are handled by the operating system. In fact, memory management happens
  in user-space and is supported by OS-provided system calls.
  - You will implement a memory manager in the second xv6 assignment. 

## The C Memory Commandments 

- Whoever malloc'eth shall free'th what they have malloced. 
  - In other words, don't free someone else's memory.
- Check `malloc`'s return value, you might be out of memory.
- `malloc` returns a chunk of memory that contains __garbage__. 
  - Do not make any assumptions about its content!
- Freed memory should __never__ be accessed again. 
- Double freeing memory is not good.
  - This is not the place for charity. 

# Function Pointers

- Similar to variables, functions in C have addresses in memory.
- So we can define pointers to functions the same way we can define pointers to
  variables. 
- A __function pointer__ is a variable that contains the address of a function
  in your code. 
- To define a function pointer, we can use the following syntax:
  ```c
  return_type (*function_ptr_name)(arguments);
  ```
- For example, the following piece of code defines a function pointer `foo` that
  points to a function that returns an `int` and accepts three `int` arguments.
  ```c
  int (*foo)(int, int, int);
  ```
- To assign a function pointer to an already existing function, you can
  equivalently use the `&` operator or simply assign it to the function's name.
  ```c
  int bar(int a, int b, int c)
  {
    return a + b + c;
  }

  int (*foo)(int, int, int);

  /* The following two statements are equivalent */
  foo = &bar;
  foo = bar;
  ```
- To call the function that `foo` points to, we simply dereference the pointer
  and pass the arguments to it.
  ```c
  int r = (*foo)(1, 2, 3);
  ```
