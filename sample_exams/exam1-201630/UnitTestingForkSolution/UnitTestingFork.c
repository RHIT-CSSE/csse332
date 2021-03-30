#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./UnitTests.h"

void runTest(void (*test)()) {
  int pid = fork();
  if (pid != 0) {
    wait(NULL);
  } else {
    test();
    exit(0);
  }
}

int main() {

  void (*tests)();
  runTest(runUnitTest1);
  runTest(runUnitTest2);
  runTest(runUnitTest3);
  runTest(runUnitTest4);
  return 0;
}


