#include <unistd.h>
#include <stdio.h>
#include "preempt_threads.h"

void thread_function()
{
    
    for(int i = 0; i < 200; i++) {
        write(STDOUT_FILENO, "111 working\n",12); 
    }
    write(STDOUT_FILENO, "111 done\n",9); 

}

void thread_function2()
{
    for(int i = 0; i < 200; i++) {
        write(STDOUT_FILENO, "222 working\n",12); 
    }
    write(STDOUT_FILENO, "222 done\n",9);
}

void main() {

    initialize_basic_threads();
    create_new_thread(thread_function);
    create_new_thread(thread_function2);
    schedule_threads_with_preempt(5);
    printf( "Everything finished\n" );
}

