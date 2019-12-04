#include <stdio.h>
#include <string.h>
#include "CuTest.h"
#include "./author.h"

/*
Copyright 2016 Rose-Hulman Institute of Technology
*/

void test_longest_author(CuTest *tc) {
  Author one;

  strncpy(one.name, "Testy McTesterson", 30);
  one.book_count = 2;
  strncpy(one.book_name[0], "Short", 70);
  strncpy(one.book_name[1], "Longer", 70);

  CuAssertStrEquals(tc, "Longer", get_longest_book_title(one));

  strncpy(one.book_name[0], "Really Long", 70);
  CuAssertStrEquals(tc, "Really Long", get_longest_book_title(one));
}

void RunAllTests() {
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_longest_author);
  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);
}

int main() {
  RunAllTests();
  return 0;
}
