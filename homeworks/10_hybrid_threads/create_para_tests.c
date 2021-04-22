#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "hybrid_threads.h"
#include "CuTest.h"

int count;
int count2;


sem_t sem; //this is inited in main

void add_10_to_count_sem()
{
    for(int i = 0; i < 10; i++) {
        sem_wait(&sem);
        count = count + 1;
        sem_post(&sem);
    }
}

void make_25_threads()
{
    for(int i = 0; i < 25; i++) {
        create_new_thread(add_10_to_count_sem);
    }
}


void test_create_a_lot(CuTest *tc) {

    count = 0;

    initialize_basic_threads();
    for(int i = 0; i < 3; i++) {
        create_new_thread(make_25_threads);
    }
    schedule_hybrid_threads(3);
    CuAssertIntEquals(tc, 750, count);
}

void test_create_a_lot_with_extra_threads(CuTest *tc) {

    count = 0;

    initialize_basic_threads();
    for(int i = 0; i < 3; i++) {
        create_new_thread(make_25_threads);
    }
    schedule_hybrid_threads(10);
    CuAssertIntEquals(tc, 750, count);
}

bool child_ran, parent_ran;

void child_t() {
    child_ran = true;
    usleep(200);
    if(!parent_ran) count = -75;
    count++;
}

void parent_t() {
    create_new_thread(child_t);
    usleep(100);
    if(!child_ran) count2 = -75;
    parent_ran = true;
    count2++;
}


void test_parent_and_child_run_in_parallel(CuTest *tc) {

    count = 0;
    count2 = 0;
    child_ran = false;
    parent_ran = false;
    
    initialize_basic_threads();

    create_new_thread(parent_t);

    schedule_hybrid_threads(2);
    CuAssertIntEquals(tc, 1, count);
    CuAssertIntEquals(tc, 1, count2);
}

void slow_parent_t() {
    usleep(1000); // time for unused threads to die if you're doing it wrong
    parent_t();
}

void test_pthreads_live_even_if_initially_uneeded(CuTest *tc) {

    count = 0;
    count2 = 0;
    child_ran = false;
    parent_ran = false;
    
    initialize_basic_threads();

    create_new_thread(slow_parent_t);

    schedule_hybrid_threads(2);
    CuAssertIntEquals(tc, 1, count);
    CuAssertIntEquals(tc, 1, count2);
}



int main(int argc, char *argv[]) {

    sem_init(&sem, 0, 1);
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_create_a_lot);
    SUITE_ADD_TEST(suite, test_create_a_lot_with_extra_threads);
    SUITE_ADD_TEST(suite, test_parent_and_child_run_in_parallel);
    SUITE_ADD_TEST(suite, test_pthreads_live_even_if_initially_uneeded);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}
