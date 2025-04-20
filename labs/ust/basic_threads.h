/*
Basic Threads - a rudimentary userspace threads library

Author: Buffalo (hewner@rose-hulman.edu)

Contrary to C convention (but for your convenience) we've documented
these functions in the .c file rather than the header.

You should not need to modify this header.

*/

void initialize_basic_threads();

void create_new_thread(void (*fun_ptr)());

void create_new_parameterized_thread(void (*fun_ptr)(void*), void* parameter);

void schedule_threads();

void yield();

void finish_thread();
