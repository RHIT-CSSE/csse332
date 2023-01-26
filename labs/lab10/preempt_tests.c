#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "preempt_threads.h"
#include "CuTest.h"

int count;
int count2;

void add_10000_to_count()
{
    for(int i = 0; i < 10000; i++) {
        count = count + 1;
    }
    finish_thread();
}

void add_10000_to_count2()
{
    // need a different one because otherwise we can get race
    // conditions
    for(int i = 0; i < 10000; i++) {
        count2 = count2 + 1;
    }
    finish_thread();
}


void test_1(CuTest *tc) {
    count = 0;
    initialize_basic_threads();
    create_new_thread(add_10000_to_count);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 10000, count);
}

void test_2(CuTest *tc) {
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(add_10000_to_count);
    create_new_thread(add_10000_to_count2);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 10000, count);
    CuAssertIntEquals(tc, 10000, count2);
}

void add_10000_to_count_with_yields()
{
    for(int i = 0; i < 10000; i++) {
        count = count + 1;
        yield();
    }
    finish_thread();
}

void add_10000_to_count2_with_yields()
{
    // need a different one because otherwise we can get race
    // conditions
    for(int i = 0; i < 10000; i++) {
        count2 = count2 + 1;
        yield();
    }
    finish_thread();
}


void test_3(CuTest *tc) {
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(add_10000_to_count_with_yields);
    create_new_thread(add_10000_to_count2_with_yields);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 10000, count);
    CuAssertIntEquals(tc, 10000, count2);
}

void test_4(CuTest *tc) {
    // a mix of yielding and non-yielding functions
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(add_10000_to_count);
    create_new_thread(add_10000_to_count2_with_yields);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 10000, count);
    CuAssertIntEquals(tc, 10000, count2);
}

bool one_ran = false, two_ran = false;

void interleave1()
{
    one_ran = true;
    while(!two_ran) {}
    count = count + 1;
    finish_thread();
}

void interleave2()
{
    two_ran = true;
    while(!one_ran) {}
    count2 = count2 + 1;
    finish_thread();
}

void test_5(CuTest *tc) {
    //this test will run forever if threads are not interleaved
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(interleave1);
    create_new_thread(interleave2);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 1, count);
    CuAssertIntEquals(tc, 1, count2);
}


void quick_ending_thread()
{
    count = count + 1;
}

void quick_ending_thread2()
{
    count2 = count2 + 1;
}


void start_many_quick_threads()
{
    for(int i = 0; i < 20; i++) {
        int old_count = count;
        int old_count2 = count2;
        create_new_thread(quick_ending_thread);
        create_new_thread(quick_ending_thread2);
        while(old_count == count) { } //get stuck in a loop
        while(old_count2 == count2) { } //get stuck in a loop
    }
}

void test_6(CuTest *tc) {
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(start_many_quick_threads);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 20, count);
    CuAssertIntEquals(tc, 20, count2);
}

void takes_an_int(void* val) {
    int* int_ptr = (int*) val;
    for(int i = 0; i < 10000; i++) {
        count += *int_ptr;
    }
    while(count2 < 10) { } //ensures interleaving
}

void takes_an_int2(void* val) {
    int* int_ptr = (int*) val;
    for(int i = 0; i < 10000; i++) {
        count2 += *int_ptr;
    }
    while(count < 10) { } //ensures interleaving
}


void test_7(CuTest *tc) {
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    int val = 7;
    int val2 = 3;
    create_new_parameterized_thread(takes_an_int, &val);
    create_new_parameterized_thread(takes_an_int2, &val2);
    schedule_threads_with_preempt(5);
    CuAssertIntEquals(tc, 70000, count);
    CuAssertIntEquals(tc, 30000, count2);
}


int main(int argc, char *argv[]) {
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_1);
    SUITE_ADD_TEST(suite, test_2);
    SUITE_ADD_TEST(suite, test_3);
    SUITE_ADD_TEST(suite, test_4);
    SUITE_ADD_TEST(suite, test_5);
    SUITE_ADD_TEST(suite, test_6);
    SUITE_ADD_TEST(suite, test_7);
    

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}
