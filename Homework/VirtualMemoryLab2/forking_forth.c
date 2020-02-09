#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "forth_embed.h"
#include "forking_forth.h"

// if the region requested is already mapped, things fail
// so we want address that won't get used as the program
// starts up
#define UNIVERSAL_PAGE_START 0xf9f8c000


// the number of memory pages will will allocate to an instance of forth
#define NUM_PAGES 12 // last two pages are for the return stack
#define MAX_FORTHS 10

#define PAGE_UNCREATED -1

struct forth_extra_data {
    bool valid;
    struct forth_data data;
};

struct forth_extra_data forth_extra_data[MAX_FORTHS];  

int used_pages_count;

int get_used_pages_count() {
    return used_pages_count;
}

bool first_time = true;
void initialize_forths() {
    if(first_time) {
        // here's the place for code you only want to run once, like registering
        // our SEGV signal handler

        first_time = false;
               
    }
    // here's the place for code you want to run every time we run a test case

    // mark all the forths as invalid
    for(int i = 0; i < MAX_FORTHS; i++) {
        forth_extra_data[i].valid = false;
    }
    
    used_pages_count = 0;

}

int find_available_slot() {
    int forth_num;
    for(forth_num = 0; forth_num < MAX_FORTHS; forth_num++) {
        if(forth_extra_data[forth_num].valid == false) {
            break; // we've found a num to use
        }
    }
    if(forth_num == MAX_FORTHS) {
        printf("We've created too many forths!");
        exit(1);
    }
    return forth_num;
}

// This function creates a brand new forth instance (not a fork) with the given code
// The function returns the id num of the newly created forth
int create_forth(char* code) {
    int forth_num = find_available_slot();
    forth_extra_data[forth_num].valid = true;


    // STEP 0
    // this is where you should allocate NUM_PAGES*getpagesize() bytes
    // starting at position UNIVERSAL_PAGE_START to get started
    //
    // use mmap
    
    

    // the return stack is a forth-specific data structure.  I
    // allocate a seperate space for it as the last 2 pages of
    // NUM_PAGES.
    int returnstack_size = getpagesize() * 2;

    int stackheap_size = getpagesize() * (NUM_PAGES - 2);

    // note that in this system, to make forking possible, all forths
    // are created with pointers only in the universal memory region
    initialize_forth_data(&forth_extra_data[forth_num].data,
                          (void*) UNIVERSAL_PAGE_START + stackheap_size + returnstack_size, //beginning of returnstack
                          (void*) UNIVERSAL_PAGE_START, //begining of heap
                          (void*) UNIVERSAL_PAGE_START + stackheap_size); //beginning of the stack


    load_starter_forth(&forth_extra_data[forth_num].data);

    forth_extra_data[forth_num].data.input_current = code;
    return forth_num;
}

struct run_output run_forth_until_event(int forth_to_run) {
    struct run_output output;
    output.result_code = f_run(&forth_extra_data[forth_to_run].data,
                               NULL,
                               output.output,
                               sizeof(output.output));
    output.forked_child_id = -1; // this should only be set to a value if we are forking
    if(output.result_code == FCONTINUE_FORK) {
        printf("fork not yet implemented\n");
        // output.forked_child_id = fork_forth(forth_to_run);
    } 
    return output;

}
