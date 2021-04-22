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

void add_10_to_count()
{
    for(int i = 0; i < 10; i++) {
        count = count + 1;
    }
}

void add_10_to_count2()
{
    for(int i = 0; i < 10; i++) {
        count2 = count2 + 1;
    }
}


void test_1(CuTest *tc) {
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(add_10_to_count);
    create_new_thread(add_10_to_count2);
    schedule_hybrid_threads(2);
    CuAssertIntEquals(tc, 10, count);
    CuAssertIntEquals(tc, 10, count2);
}

bool one_ran = false, two_ran = false, three_ran = false;

void interleave1()
{
    one_ran = true;
    usleep(1000); // should be plenty of time
    if(!two_ran  || !three_ran ) {
        printf("ERROR not interleaved\n");
        count = -80;
    }
    count = count + 1;
    finish_thread();
}

void interleave2()
{
    two_ran = true;
    usleep(1000); // should be plenty of time
    if(!one_ran) {
        printf("ERROR not interleaved\n");
        count = -75;
    }
    count2 = count2 + 1;
    finish_thread();
}

void test_2interleave(CuTest *tc) {
    one_ran = false;
    two_ran = false;
    three_ran = true;
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(interleave1);
    create_new_thread(interleave2);
    schedule_hybrid_threads(2);
    CuAssertIntEquals(tc, 1, count);
    CuAssertIntEquals(tc, 1, count2);
}

void interleave2_with_yield()
{
    two_ran = true;
    yield();
    if(!three_ran) {
        printf("ERROR not interleaved\n");
        count = -75;
    }
    count2 = count2 + 1;
}

void interleave3_with_yield()
{
    three_ran = true;
    yield();
    if(!two_ran) {
        printf("ERROR not interleaved\n");
        count = -75;
    }
    count2 = count2 + 1;
}

void test_3interleave(CuTest *tc) {
    one_ran = false;
    two_ran = false;
    three_ran = false;
    count = 0;
    count2 = 0;
    initialize_basic_threads();
    create_new_thread(interleave1);
    create_new_thread(interleave2_with_yield);
    create_new_thread(interleave3_with_yield);
    schedule_hybrid_threads(2);
    CuAssertIntEquals(tc, 1, count);
    CuAssertIntEquals(tc, 2, count2);
}

sem_t sem; //this is inited in main

void add_100_to_count_sem()
{
    for(int i = 0; i < 100; i++) {
        sem_wait(&sem);
        count = count + 1;
        sem_post(&sem);
    }
}

void test_lots_o_threads(CuTest *tc) {

    count = 0;

    initialize_basic_threads();
    for(int i = 0; i < 75; i++) {
        create_new_thread(add_100_to_count_sem);
    }
    schedule_hybrid_threads(3);
    CuAssertIntEquals(tc, 7500, count);
}


void add_100_to_count_sem_and_yield()
{
    for(int i = 0; i < 100; i++) {
        usleep(1);
        yield();
        sem_wait(&sem);
        // yielding is cool, but don't yield when you have a system
        // level semaphore - the os will put waiting pthreads thread
        // in the waiting state.  If all your threads enter that state
        // the hyrbid thread system will be hopelessly deadlocked.
        //
        // If we wanted to get fancy we could build our own semaphores
        // that are treated correctly by the hybrid system
        count = count + 1;
        sem_post(&sem);
        yield();
    }
}

void test_lots_o_bored_schedulers(CuTest *tc) {

    count = 0;

    initialize_basic_threads();
    for(int i = 0; i < 2; i++) {
        create_new_thread(add_100_to_count_sem_and_yield);
    }
    schedule_hybrid_threads(25); // lots of schedulers who can't get jobs
    CuAssertIntEquals(tc, 200, count);
}

void test_lots_o_yielding(CuTest *tc) {

    count = 0;

    initialize_basic_threads();
    for(int i = 0; i < 75; i++) {
        create_new_thread(add_100_to_count_sem_and_yield);
    }
    schedule_hybrid_threads(10);
    CuAssertIntEquals(tc, 7500, count);
}





int main(int argc, char *argv[]) {

    sem_init(&sem, 0, 1);
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_1);
    SUITE_ADD_TEST(suite, test_2interleave);
    SUITE_ADD_TEST(suite, test_3interleave);
    SUITE_ADD_TEST(suite, test_lots_o_threads);
    SUITE_ADD_TEST(suite, test_lots_o_bored_schedulers);
    SUITE_ADD_TEST(suite, test_lots_o_yielding);


    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}
