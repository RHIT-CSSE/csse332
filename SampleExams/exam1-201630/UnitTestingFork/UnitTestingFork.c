#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./UnitTests.h"

int main() {
  runUnitTest1();
  runUnitTest2();
  runUnitTest3();
  runUnitTest4();
  printf("All tests complete\n");
  return 0;
}


