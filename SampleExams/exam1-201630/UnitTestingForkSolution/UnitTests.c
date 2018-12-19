#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*

You should not need to modify this file!
  
 */

int buggyAdd1(int a, int b) {
  /* works perfectly, so long as you don't need to carry */
  return a | b;
}

int buggyAdd2(int a, int b) {
  /* works perfectly, except it segfaults */
  int *pointer = NULL;
  *pointer = b;
  return a + *pointer;
}


void assertEquals(int expected, int actual)
{
  if(expected == actual){
    printf("assert passed.\n");
  } else {
    printf("assert failed.  Expected %d got %d\n", expected, actual);
  }
}

void runUnitTest1()
{
  printf("Starting unit test 1 of 4\n");
  sleep(1);
  assertEquals(10, buggyAdd1(9,1));
  assertEquals(10, buggyAdd1(8,2));
}

void runUnitTest2()
{
  printf("Starting unit test 2 of 4\n");
  sleep(1);
  assertEquals(10, buggyAdd2(9,1));
  assertEquals(10, buggyAdd2(8,2));
}

void runUnitTest3()
{
  printf("Starting unit test 3 of 4\n");
  sleep(1);
  assertEquals(2, buggyAdd2(0,2));
}

void runUnitTest4()
{
  printf("Starting unit test 4 of 4\n");
  sleep(1);
  assertEquals(2, buggyAdd1(0,2));
}
