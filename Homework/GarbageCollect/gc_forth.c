#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "forth/forth_embed.h"
#include "arraylist.h"
#include "CuTest.h"


// these memory locations are global so I can reinit them with every test
void* stackheap;
void* stackheap_end;
void* returnstack;
void* returnstack_end;

// a global forth instance that everything uses
struct forth_data forth;

// TODO: declare whatever structs and globals you need to
// store regions

void handle_alloc_begin() {
    // TODO: your code here
}

void handle_alloc_end() {
    // TODO: your code here
}

int compute_alloced_size() {
    // TODO: your code here
    return 0;
}

int compute_num_regions() {
    // TODO: your code here
    return 0;
}

int compute_unrefed_size() {
    // TODO: your code here
    return 0;
}

void gc_collect() {
    // TODO: your code here
}

int run_forth_for_string(char *string);
uint64_t pop_forth_as_uinteger();

void initialize_forth_for_test() {

    //TODO: initialize your region struct in whatever way you see fit
    // you can also add stuff to the top if main if you'd like it to
    // be initialized once only
    
    // zero out the stacks to prevent tests from infecting each other
    memset(stackheap, 0, stackheap_end - stackheap);
    memset(returnstack, 0, returnstack_end - returnstack);

    initialize_forth_data(&forth,
                          returnstack_end, //beginning of returnstack
                          stackheap, //begining of heap
                          stackheap_end);

    // add some code that tracks all function compiles, which are stored in the heap
    char * setup_code =
        " : [COMPILE] IMMEDIATE WORD FIND >CFA , ; " // this one is defined in jonesforth.f but we need it now
        ": ALLOC_BEGIN 20 PAUSE_WITH_CODE ; "
        ": ALLOC_END 21 PAUSE_WITH_CODE ; "
        ": : ALLOC_BEGIN : ; " // call alloc begin before compilation begins
        ": ; [COMPILE] ; ALLOC_END ; IMMEDIATE " // call alloc_end after compilation finishes 
        " ALLOC_END " // manually call alloc_end this one time so all begins and ends are matched
        ; 
    run_forth_for_string(setup_code);

    // now load the basic forth functions in jonesforth.f
    FILE* file = fopen("forth/jonesforth.f","r");
    if(file == NULL) {
        printf("error loading jonesforth.f\n");
        exit(1);
    }

    char input_buffer[200];
    while(fgets(input_buffer, 200, file) != NULL) {
        int result = run_forth_for_string(input_buffer);
        if(result != FCONTINUE_INPUT_DONE) {
            printf("unexpected result returned from forth: %d\n", result);
            exit(1);
        }
    }
    // now redefine a few newly added functions to include logging
    char * post_setup_code = 
        ": ALLOT ALLOC_BEGIN ALLOT ALLOC_END ; ";
    run_forth_for_string(post_setup_code);
    post_setup_code =    ": CONSTANT ALLOC_BEGIN CONSTANT ALLOC_END ; ";
    run_forth_for_string(post_setup_code);
    post_setup_code =
        ": VARIABLE 1 CELLS ALLOT WORD ALLOC_BEGIN CREATE DOCOL , ' LIT , , ' EXIT , ALLOC_END ; ";
    run_forth_for_string(post_setup_code);
    post_setup_code =
        ": VALUE ALLOC_BEGIN VALUE ALLOC_END ; ";
    run_forth_for_string(post_setup_code);
    post_setup_code =
        ": :NONAME ALLOC_BEGIN :NONAME ; "
        ; 
    run_forth_for_string(post_setup_code);
    
}

void test_region_records(CuTest *tc) {
    initialize_forth_for_test();

    // if we change the standard forth initialization, these
    // values need to be updated
    CuAssertIntEquals(tc, 6576, compute_alloced_size());
    CuAssertIntEquals(tc, 71, compute_num_regions());

    // allocate 24 bytes in 2 regions
    run_forth_for_string("2 CELLS ALLOT 1 CELLS ALLOT ");
    CuAssertIntEquals(tc, 6600, compute_alloced_size());
    CuAssertIntEquals(tc, 73, compute_num_regions());
}

void test_basic_refs(CuTest *tc) {
    initialize_forth_for_test();
    
    run_forth_for_string("2 CELLS ALLOT 3 CELLS ALLOT DROP ");

    // remember, this is 64 bit so 3 Cells (a cell is the standard
    // pointer size) is 24 bytes
    CuAssertIntEquals(tc, 24, compute_unrefed_size());
    run_forth_for_string(" DROP ");

    CuAssertIntEquals(tc, 40, compute_unrefed_size());
    // allocate some data and store it in A
    run_forth_for_string(" 2 CELLS ALLOT VARIABLE A A ! ");

    // it is not unreferenced
    CuAssertIntEquals(tc, 40, compute_unrefed_size());

    // now set A to null
    run_forth_for_string("0 A ! ");

    CuAssertIntEquals(tc, 56, compute_unrefed_size());
}

void test_chain(CuTest *tc) {
    initialize_forth_for_test();

    // allocates first block of 16 bytes and puts it on the stack
    run_forth_for_string("2 CELLS ALLOT");

    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // allocates second block of 8 bytes and puts it on the stack
    run_forth_for_string("1 CELLS ALLOT");

    // puts the address of the first block in the second
    // but keeps the second on the stack
    run_forth_for_string("DUP ROT SWAP !");
    CuAssertIntEquals(tc, 0, compute_unrefed_size());
    
    // store the second block address variable a A, still not unreferenced
    // but no longer on the stack
    run_forth_for_string(" VARIABLE A A ! ");
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // allocate a 3rd block, store the second block's address in it
    // and replace A with the 3rd block's address
    // we're building a crude linked list here with head at A
    // and nothing is unreferenced
    run_forth_for_string(" 1 CELLS ALLOT DUP A @ SWAP ! A !");
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // now set A to null
    run_forth_for_string("0 A ! ");
    // set a to null, now all 3 blocks are unreferenced
    CuAssertIntEquals(tc, 32, compute_unrefed_size());
}

void test_1_refs_3(CuTest *tc) {

    initialize_forth_for_test();

    // allocates first block of 24 bytes and puts it on the stack
    run_forth_for_string("3 CELLS ALLOT");

    // store our 3 byte block in A
    run_forth_for_string(" VARIABLE A A ! ");
    // nothing is unrefed
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // allocate 3 different blocks of memory
    run_forth_for_string("1 CELLS ALLOT 1 CELLS ALLOT 1 CELLS ALLOT");

    // store the first one's address in the first 8 bytes of the first block
    run_forth_for_string("A @ ! ");
    // nothing is unrefed
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // store the second one's address in the second 8 bytes of the first block
    run_forth_for_string("A @ 8+ ! ");

    // store the 3rd one's address in the 3rd 8 bytes of the first block
    run_forth_for_string("A @ 16 + ! ");
    
    // nothing is unrefed
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // null the second entry in our 3 element block and we've lost exactly 1
    // allocated block
    run_forth_for_string("0 A @ 8+ ! ");
    CuAssertIntEquals(tc, 8, compute_unrefed_size());
    
    // null A itself and we've lost everything
    run_forth_for_string("0 A ! ");
    CuAssertIntEquals(tc, 48, compute_unrefed_size());
    
}

void test_circular_ref(CuTest *tc) {

    initialize_forth_for_test();

    // allocates first block of 8 bytes
    run_forth_for_string("1 CELLS ALLOT");

    // call our first block A
    run_forth_for_string(" VARIABLE A A ! ");


    // allocates second block of 8 bytes
    run_forth_for_string("1 CELLS ALLOT");

    // call our second block B
    run_forth_for_string(" VARIABLE B B ! ");

    // nothing is unrefed
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // make b ref A and a ref B
    run_forth_for_string(" B @ A @ ! ");
    run_forth_for_string(" A @ B @ ! ");

    //now if we null A nothing is unrefed
    run_forth_for_string(" 0 A ! ");
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    //but if we null B too the whole structure is lost
    run_forth_for_string(" 0 B ! ");
    CuAssertIntEquals(tc, 16, compute_unrefed_size());
    
}

void test_ref_within_region(CuTest *tc) {

    initialize_forth_for_test();

    // allocates first block of 16 bytes, address on stack
    run_forth_for_string("2 CELLS ALLOT");

    // move the pointer so that it points to the 8th
    // byte of the region rather than the first
    run_forth_for_string("8+ ");

    // the region is still considered referenced
    CuAssertIntEquals(tc, 0, compute_unrefed_size());

    // move the pointer so that it points one byte
    // beyond the referenced region
    run_forth_for_string("8+ ");

    // the region is no longer considered referenced
    CuAssertIntEquals(tc, 16, compute_unrefed_size());

}

void test_gc_no_pointer_rewrite(CuTest *tc) {
    initialize_forth_for_test();

    // allocates first block of 16 bytes of garbage
    // then allocates 16 bytes of real data
    // then allocates 16 bytes of garbage
    run_forth_for_string("2 CELLS ALLOT DROP 2 CELLS ALLOT DUP 2 CELLS ALLOT DROP");

    // set entries of real data to be 73 and 25
    run_forth_for_string("DUP 73 SWAP ! DUP 8+ 25 SWAP !");

    void* oldhere = forth.here;
    
    gc_collect();

    // we should save 16 bytes of data and here should be updated appropiately
    CuAssertIntEquals(tc, 32, oldhere - forth.here);

    // directly before here should be our relocated 2 eight bit integers
    CuAssertIntEquals(tc, 73, *((int64_t*) (forth.here - 16)));
    CuAssertIntEquals(tc, 25, *((int64_t*) (forth.here - 8)));
}

void test_gc_stack_update(CuTest *tc) {
    initialize_forth_for_test();


    // allocates first block of 8 bytes of garbage
    // then allocates 8 bytes of real data
    run_forth_for_string("2 CELLS ALLOT DROP 2 CELLS ALLOT DUP");

    
    // store the address of the data we're keeping
    uint64_t address = pop_forth_as_uinteger();
    
    // set entries of real data to be 73 and 25
    run_forth_for_string("DUP 73 SWAP ! DUP 8+ 25 SWAP !");
    
    void* oldhere = forth.here;
    
    gc_collect();

    // we should save 16 bytes of data and here should be updated appropiately
    CuAssertIntEquals(tc, 16, oldhere - forth.here);
    
    // the address on the stack should be rewritten to be a new location
    run_forth_for_string("DUP ");
    uint64_t address2 = pop_forth_as_uinteger();
    // printf("old address %lu new address %lu %lu\n", address, address2, address - address2); 
    CuAssertIntEquals(tc, address - 16, address2);

    run_forth_for_string("DUP @ SWAP 8+ @ ");

    CuAssertIntEquals(tc, 25, pop_forth_as_uinteger());
    CuAssertIntEquals(tc, 73, pop_forth_as_uinteger());
    
    // collect everything so that it isn't confusing once we add more data
    gc_collect();
    
    // 4 bytes garbage
    // 4 bytes data
    // 4 bytes garbage
    // 4 bytes data
    run_forth_for_string("1 CELLS ALLOT DROP 1 CELLS ALLOT 1 CELLS ALLOT DROP 1 CELLS ALLOT ");

    run_forth_for_string("DUP 22 SWAP ! SWAP DUP 23 SWAP ! 2DUP 2DUP");
    address = pop_forth_as_uinteger();
    address2 = pop_forth_as_uinteger();
    
    gc_collect();

    CuAssertIntEquals(tc, 8, address - pop_forth_as_uinteger());
    CuAssertIntEquals(tc, 16, address2 - pop_forth_as_uinteger());

    run_forth_for_string("@ SWAP @ ");
    CuAssertIntEquals(tc, 22, pop_forth_as_uinteger());
    CuAssertIntEquals(tc, 23, pop_forth_as_uinteger());
}

void test_gc_internal_data_update(CuTest *tc) {
    initialize_forth_for_test();

    // allocates first block of 24 bytes and puts it on the stack
    run_forth_for_string("3 CELLS ALLOT");

    // store our 3 byte block in A
    run_forth_for_string(" VARIABLE A A ! ");

    // allocate 1 cell of data on the stack
    run_forth_for_string("1 CELLS ALLOT DUP ");
    uint64_t address1orig = pop_forth_as_uinteger();
    
    //allocate 1 cell of garbage
    run_forth_for_string("1 CELLS ALLOT");
    pop_forth_as_uinteger();
    
    //allocate 1 cell of data on the stack
    run_forth_for_string("1 CELLS ALLOT DUP");

    uint64_t address2orig = pop_forth_as_uinteger();
    
    //allocate 1 cell of garbage
    run_forth_for_string("1 CELLS ALLOT");
    pop_forth_as_uinteger();
    
    //allocate 1 more cell of data (again leaving address on the stack)
    run_forth_for_string("1 CELLS ALLOT DUP ");

    uint64_t address3orig = pop_forth_as_uinteger();

    // store the 3rd one's address in the first 8 bytes of the first block
    run_forth_for_string("A @ ! ");

    // store the second one's address in the second 8 bytes of the first block
    run_forth_for_string("A @ 8+ ! ");

    // store the 1st one's address in the 3rd 8 bytes of the first block
    run_forth_for_string("A @ 16 + ! ");

    gc_collect();

    // 3rd one should have moved 16 bytes
    run_forth_for_string("A @ @ ");
    CuAssertIntEquals(tc, 16, address3orig - pop_forth_as_uinteger());

    // 2nd one should have moved 8 bytes
    run_forth_for_string("A @ 8+ @ ");
    CuAssertIntEquals(tc, 8, address2orig - pop_forth_as_uinteger());

    // 1st one should not have moved
    run_forth_for_string("A @ 16 + @ ");
    CuAssertIntEquals(tc, 0, address1orig - pop_forth_as_uinteger());
    
    
}

void test_gc_unaligned_data_update(CuTest *tc) {
    // by unaligned, I mean data where a pointer is stored
    // not at a four byte boundary
    
    initialize_forth_for_test();

    // allocates block of 9 bytes and puts it on the stack
    run_forth_for_string("9 ALLOT ");
    //allocates 1 byte of garbage
    run_forth_for_string("1 ALLOT DROP ");
    //allocates 8 bytes of data and stores the value 4 in it
    run_forth_for_string("1 CELLS ALLOT DUP 4 SWAP ! DUP ");
    uint64_t address1orig = pop_forth_as_uinteger();
    
    //stores the address of the 4 in the last 8 bytes of 9 byte block
    run_forth_for_string("SWAP DUP ROT SWAP 1+ ! ");

    //only that one byte should be considered unrefed
    CuAssertIntEquals(tc, 1, compute_unrefed_size());

    gc_collect();

    //it might be wise for the gc to force blocks to be aligned but
    //ours doesn't
    
    run_forth_for_string("1+ @ DUP ");
    CuAssertIntEquals(tc, 1, address1orig - pop_forth_as_uinteger());

    //ensure the 4 is still there
    run_forth_for_string("@ ");
    CuAssertIntEquals(tc, 4, pop_forth_as_uinteger());
    
}

void test_gc_code_relocation(CuTest *tc) {
    
    initialize_forth_for_test();

    // allocate 32 bytes of garbage
    run_forth_for_string("4 CELLS ALLOT DROP ");
    
    // declare a new function
    run_forth_for_string(": CUBE DUP DUP * * ; S\" CUBE\" FIND ");
    uint64_t find_address = pop_forth_as_uinteger();
    run_forth_for_string(": CALLSCUBE CUBE ;  ");

    gc_collect();

    // ensure cube has been relocated
    run_forth_for_string("S\" CUBE\" FIND ");
    CuAssertIntEquals(tc, 32, find_address - pop_forth_as_uinteger());
    
    // run the cube function
    run_forth_for_string(" 3 CUBE ");
    CuAssertIntEquals(tc, 27, pop_forth_as_uinteger());
    
    run_forth_for_string(" 2 CALLSCUBE ");
    CuAssertIntEquals(tc, 8, pop_forth_as_uinteger());

    // allocate 4 bytes of garbage
    run_forth_for_string("4 CELLS ALLOT DROP ");

    // a function with a loop
    run_forth_for_string(" : USESTACK BEGIN DUP 1- DUP 0= UNTIL ; ");
    
    gc_collect();

    run_forth_for_string("2 USESTACK .S ");
    CuAssertIntEquals(tc, 0, pop_forth_as_uinteger());
    CuAssertIntEquals(tc, 1, pop_forth_as_uinteger());
    CuAssertIntEquals(tc, 2, pop_forth_as_uinteger());
}

/*

UTILITY FUNCTIONS BEGIN

This function runs a forth string to completion

Calling the correct callbacks for alloc begin/ends

If it gets a error result, it outputs any remaining data and returns
it

You don't need to modify these but feel free to take a look.

 */
char output[200];

/* ouput goes into the output variable */
int run_forth_for_string(char *string) {

    int fresult = f_run(&forth, string, output, sizeof(output));
    while(1) {
        switch(fresult) {
        case 20:
            handle_alloc_begin();
            fresult = f_run(&forth, NULL, NULL, 0);
            break;
        case 21:
            handle_alloc_end();
            fresult = f_run(&forth, NULL, NULL, 0);
            break;
        case FCONTINUE_OUTPUT_FLUSH:
            // we don't expect forth to print so we don't print the output here
            //printf("%s", output);
            fresult = f_run(&forth, NULL, output, sizeof(output));
            break;
        case FCONTINUE_ERROR:
            printf("Parse Error on input %s\n", string);
            exit(0);
        default:
            // we don't expect forth to print so we don't print the output here            
            //printf("%s", output);
            return fresult;
        }
    }    
}


// you might think you could simply inspect the stack directy, but forth's
// input handling puts data on the stack so the "top" is probably not the
// top you expect
uint64_t pop_forth_as_uinteger() {
    int result = run_forth_for_string("U. ");
    if(result != FCONTINUE_INPUT_DONE) {
        printf("unexpected result returned from forth: %d\n", result);
        exit(1);
    }
    
    // more or less like atoi
    unsigned long num = strtoul(output, NULL, 10);
    // if we were cooler we'd check errno to see if parse fails
    return num;    
}


int main() {
    
    // TODO: add some one time initailization here if you want
    
    int returnstack_size = getpagesize() * 2;
    returnstack = mmap(NULL, returnstack_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                             MAP_ANON | MAP_PRIVATE, -1, 0);

    int stackheap_size = getpagesize() * 10;
    stackheap = mmap(NULL, stackheap_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_ANON | MAP_PRIVATE, -1, 0);

    stackheap_end = stackheap + stackheap_size;
    returnstack_end = returnstack + returnstack_size;
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_region_records);

    // tests for identifying inaccessible regions
    /*
    SUITE_ADD_TEST(suite, test_basic_refs);
    SUITE_ADD_TEST(suite, test_chain);
    SUITE_ADD_TEST(suite, test_1_refs_3);
    SUITE_ADD_TEST(suite, test_circular_ref);
    SUITE_ADD_TEST(suite, test_ref_within_region);
    */

    //SUITE_ADD_TEST(suite, test_gc_no_pointer_rewrite);
    //SUITE_ADD_TEST(suite, test_gc_stack_update);
    //SUITE_ADD_TEST(suite, test_gc_internal_data_update);
    //SUITE_ADD_TEST(suite, test_gc_unaligned_data_update);
    //SUITE_ADD_TEST(suite, test_gc_code_relocation);
        

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);

    return 0;
}
