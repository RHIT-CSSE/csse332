#ifndef _CRITICAL_H
#define _CRITICAL_H

// constants
#define NUM_RED_THREADS 6
#define NUM_BLUE_THREADS 8

// thread functions
void *red(void*);
void *blue(void*);

// test function signatures
void test1(void);
void test2(void);
void test3(void);

#endif /* critical.h */
