#include <unistd.h>
#include <stdio.h>
#include "preempt_threads.h"

void yield_a_bunch()
{
    write(STDOUT_FILENO, "yielding\n",9);
    yield();
    write(STDOUT_FILENO, "yielding\n",9);
    yield();
    write(STDOUT_FILENO, "finishing\n",10);
    finish_thread();
    // should never happen
    write(STDOUT_FILENO, "ERROR!!!\n",9);
}

void create_threads()
{
    write(STDOUT_FILENO, "creating\n",9); 
    create_new_thread(yield_a_bunch);
    write(STDOUT_FILENO, "creating\n",9);
    create_new_thread(yield_a_bunch);
    write(STDOUT_FILENO, "creating\n",9); 
    create_new_thread(yield_a_bunch);

    //yield a bunch to ensure our created threads are finished
    int i;
    for(i = 0; i < 100; i++) {
        yield();
    }

    write(STDOUT_FILENO, "creating\n",9); 
    create_new_thread(yield_a_bunch);
    write(STDOUT_FILENO, "creating\n",9);
    create_new_thread(yield_a_bunch);
    write(STDOUT_FILENO, "creating\n",9); 
    create_new_thread(yield_a_bunch);
}

void main() {
    write(STDOUT_FILENO, "starting\n",9); 
    initialize_basic_threads();
    create_new_thread(create_threads);
    schedule_threads_with_preempt(5);
    write(STDOUT_FILENO, "finished\n",9); 
}

