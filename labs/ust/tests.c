#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "basic_threads.h"
#include "CuTest.h"

int count;

void add_10_to_count()
{
  for(int i = 0; i < 10; i++) {
    yield();
    count = count + 1;
  }
  finish_thread();
}


void test_1(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  create_new_thread(add_10_to_count);
  schedule_threads();
  CuAssertIntEquals(tc, 10, count);
}

void test_2a(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  create_new_thread(add_10_to_count);
  create_new_thread(add_10_to_count);
  schedule_threads();
  CuAssertIntEquals(tc, 20, count);
}

void add_3_to_count()
{
  for(int i = 0; i < 3; i++) {
    yield();
    count = count + 1;
  }
  finish_thread();
}


void test_2b(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  create_new_thread(add_3_to_count);
  create_new_thread(add_10_to_count);
  create_new_thread(add_3_to_count);
  schedule_threads();
  CuAssertIntEquals(tc, 16, count);
}

bool one_ran = false, two_ran = false;

void interleave1()
{
  one_ran = true;
  yield();
  if(!two_ran) {
    printf("ERROR not interleaved\n");
    count = -80;
  }
  count = count + 1;
  finish_thread();
}

void interleave2()
{
  two_ran = true;
  yield();
  if(!one_ran) {
    printf("ERROR not interleaved\n");
    count = -75;
  }
  count = count + 1;
  finish_thread();
}

void test_2interleave(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  create_new_thread(interleave1);
  create_new_thread(interleave2);
  schedule_threads();
  CuAssertIntEquals(tc, 2, count);
}

void quick_ending_thread()
{
  count = count + 1;
  finish_thread();
}

void start_many_quick_threads()
{
  for(int i = 0; i < 20; i++) {
    create_new_thread(quick_ending_thread);
    yield();
  }
  finish_thread();
}

void test_3(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  create_new_thread(start_many_quick_threads);
  schedule_threads();
  CuAssertIntEquals(tc, 20, count);
}


void takes_an_int(void* val) {
  int* int_ptr = (int*) val;
  count += *int_ptr;
  finish_thread();
}

void test_4int(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  int val = 7;
  int val2 = 3;
  create_new_parameterized_thread(takes_an_int, &val);
  create_new_parameterized_thread(takes_an_int, &val2);
  schedule_threads();
  CuAssertIntEquals(tc, 10, count);
}

struct two_ints {
  int a;
  int b;
};

void takes_a_struct(void* val) {
  struct two_ints* s_ptr = (struct two_ints*) val;
  count += s_ptr->a;
  count += s_ptr->b;
  finish_thread();
}

void test_4struct(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  struct two_ints data;
  data.a = 5;
  data.b = 6;
  create_new_parameterized_thread(takes_a_struct, &data);
  schedule_threads();
  CuAssertIntEquals(tc, 11, count);
}

void add_10_to_count_no_finish()
{
  for(int i = 0; i < 10; i++) {
    yield();
    count = count + 1;
  }
  // there's no finish thread here
}

void test_5(CuTest *tc) {
  count = 0;
  initialize_basic_threads();
  create_new_thread(add_10_to_count_no_finish);
  create_new_thread(add_10_to_count_no_finish);
  schedule_threads();
  CuAssertIntEquals(tc, 20, count);
}



int main(int argc, char *argv[]) {

  if(argc < 2) {
    printf("Please include a test level to run (1-6)\n");
    exit(1);
  }

  int level = atoi(argv[1]);

  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();

  switch(level) {
  case 6:
  case 5:
    SUITE_ADD_TEST(suite, test_5);
  case 4:
    SUITE_ADD_TEST(suite, test_4int);
    SUITE_ADD_TEST(suite, test_4struct);
  case 3:
    SUITE_ADD_TEST(suite, test_3);
  case 2:
    SUITE_ADD_TEST(suite, test_2a);
    SUITE_ADD_TEST(suite, test_2b);
    SUITE_ADD_TEST(suite, test_2interleave);
  case 1:
    SUITE_ADD_TEST(suite, test_1);
    break;
  default:
    printf("unknown test level: %d\n", level);
    exit(1);
  }


  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);
  CuStringDelete(output);
  CuSuiteDelete(suite);
}
