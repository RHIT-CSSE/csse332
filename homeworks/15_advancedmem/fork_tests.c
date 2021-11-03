#include <stdio.h>
#include "CuTest.h"
#include "forking_forth.h"
#include "forth/forth_embed.h"

void test0_oneforth(CuTest *tc) {
    initialize_forths();
    int pid = create_forth(".\" hello\" ");
    struct run_output result = run_forth_until_event(pid);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "hello", result.output);
}

void test1_twoforths(CuTest *tc) {
    initialize_forths();
    // this forth is being set to use binary numbers
    int pid = create_forth(" 2 BASE ! 1 1 + . ");
    // this (different forth) is using decimal numbers
    int pid2 = create_forth("1 1 + . ");
    struct run_output result = run_forth_until_event(pid);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "10 ", result.output);
    result = run_forth_until_event(pid2);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "2 ", result.output);
}

void test2_yields(CuTest *tc) {
    initialize_forths();
    int pid = create_forth("2 1 . YIELD .");
    int pid2 = create_forth("4 3 . YIELD .");
    struct run_output result = run_forth_until_event(pid);
    CuAssertIntEquals(tc, FCONTINUE_YIELD, result.result_code);
    CuAssertStrEquals(tc, "1 ", result.output);
    result = run_forth_until_event(pid2);
    CuAssertIntEquals(tc, FCONTINUE_YIELD, result.result_code);
    CuAssertStrEquals(tc, "3 ", result.output);
    result = run_forth_until_event(pid);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "2 ", result.output);
    result = run_forth_until_event(pid2);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "4 ", result.output);
}

void test3_page_count(CuTest *tc) {
    initialize_forths();

    CuAssertIntEquals(tc, 0, get_used_pages_count());

    int pid = create_forth("3 4 +");

    // the part after the yield is forth's equiavlent of malloc(5000)
    int pid2 = create_forth("5 6 + YIELD 5000 ALLOT 4999 + @");

    struct run_output result = run_forth_until_event(pid);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    result = run_forth_until_event(pid2);
    CuAssertIntEquals(tc, FCONTINUE_YIELD, result.result_code);

    // each forth uses 2 pages of heap, 1 stack, and 1 return stack
    CuAssertIntEquals(tc, 8, get_used_pages_count());

    // after the yield in pid2 we allocate 5000 bytes on the heap
    // which should force us to use another page of heap
    result = run_forth_until_event(pid2);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertIntEquals(tc, 9, get_used_pages_count());
}

// this test is designed to pass whether or not fork actually pushes a
// parent/child return value onto the forth stacks.  But of course
// without that value to differentiate, the parent and the children
// must act identically
void test4_identical_fork(CuTest *tc) {
    initialize_forths();
    int parent_id = create_forth("20 20 FORK DROP . ");
    struct run_output result = run_forth_until_event(parent_id);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    CuAssertTrue(tc, result.forked_child_id != -1);
    result = run_forth_until_event(result.forked_child_id);
    CuAssertStrEquals(tc, "20 ", result.output);
    result = run_forth_until_event(parent_id);
    CuAssertStrEquals(tc, "20 ", result.output);
}

void test5_fork_return(CuTest *tc) {
    initialize_forths();
    int parent_id = create_forth(": TESTFUNC 20 20 FORK "
                                 " IF .\" parent\""
                                 " ELSE .\" child\" THEN ; "
                                 " TESTFUNC ");
    struct run_output result = run_forth_until_event(parent_id);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    CuAssertTrue(tc, result.forked_child_id != -1);
    result = run_forth_until_event(result.forked_child_id);
    CuAssertStrEquals(tc, "child", result.output);
    result = run_forth_until_event(parent_id);
    CuAssertStrEquals(tc, "parent", result.output);
}

// a double fork should produce 4 processes as usual
void test6_double_fork(CuTest *tc) {
    initialize_forths();
    int parent = create_forth("20 20 FORK FORK . . ");
    struct run_output result = run_forth_until_event(parent);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int child = result.forked_child_id;
    CuAssertTrue(tc, child != -1);
    result = run_forth_until_event(parent);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int child2 = result.forked_child_id;
    CuAssertTrue(tc, child2 != -1);
    result = run_forth_until_event(child);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int grandchild = result.forked_child_id;
    CuAssertTrue(tc, grandchild != -1);
    result = run_forth_until_event(parent);
    CuAssertStrEquals(tc, "1 1 ", result.output);
    result = run_forth_until_event(grandchild);
    CuAssertStrEquals(tc, "0 0 ", result.output);
    result = run_forth_until_event(child);
    CuAssertStrEquals(tc, "1 0 ", result.output);
    result = run_forth_until_event(child2);
    CuAssertStrEquals(tc, "0 1 ", result.output);
}

void test7_copy_on_write(CuTest *tc) {
    initialize_forths();
    int parent_id = create_forth(" 10 FORK + . YIELD VARIABLE VAR");
    struct run_output result = run_forth_until_event(parent_id);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int child_id = result.forked_child_id;
    CuAssertTrue(tc, child_id != -1);

    // we've forked, and we've edited the stack of both pages so we
    // should see 5 pages (4 for parent, 1 for stack copy)
    CuAssertIntEquals(tc, 5, get_used_pages_count());

    // we get an additional page when we run the child and the child
    // edits the return stack
    result = run_forth_until_event(child_id);
    CuAssertStrEquals(tc, "10 ", result.output);
    CuAssertIntEquals(tc, 6, get_used_pages_count());

    // we finally get a 3rd additional page when "VARIABLE VAR"
    // allocates some heap

    result = run_forth_until_event(child_id);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertIntEquals(tc, 7, get_used_pages_count());
    
}

void test8_copy_on_write_parent_edit(CuTest *tc) {
    initialize_forths();
    int parent_id = create_forth(" 10 FORK + . ");
    struct run_output result = run_forth_until_event(parent_id);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int child_id = result.forked_child_id;
    CuAssertTrue(tc, child_id != -1);

    // we've forked, and we've edited the stack of both pages so we
    // should see 5 pages
    CuAssertIntEquals(tc, 5, get_used_pages_count());

    // running the parent should cause return stack to be copied too
    result = run_forth_until_event(parent_id);
    CuAssertStrEquals(tc, "11 ", result.output);
    CuAssertIntEquals(tc, 6, get_used_pages_count());

}

void test9_double_fork_copy_on_write(CuTest *tc) {
    initialize_forths();
    int parent = create_forth("20 20 FORK FORK . . YIELD VARIABLE VAR");
    struct run_output result = run_forth_until_event(parent);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int child = result.forked_child_id;
    CuAssertTrue(tc, child != -1);
    result = run_forth_until_event(parent);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int child2 = result.forked_child_id;
    CuAssertTrue(tc, child2 != -1);
    result = run_forth_until_event(child);
    CuAssertIntEquals(tc, FCONTINUE_FORK, result.result_code);
    int grandchild = result.forked_child_id;
    CuAssertTrue(tc, grandchild != -1);
    result = run_forth_until_event(parent);
    CuAssertStrEquals(tc, "1 1 ", result.output);
    result = run_forth_until_event(grandchild);
    CuAssertStrEquals(tc, "0 0 ", result.output);
    result = run_forth_until_event(child);
    CuAssertStrEquals(tc, "1 0 ", result.output);
    result = run_forth_until_event(child2);
    CuAssertStrEquals(tc, "0 1 ", result.output);

    // at this point we should have copied the stack and return stack
    // 3 times
    CuAssertIntEquals(tc, 10, get_used_pages_count());

    // now run the parent which edits the shared heap page
    run_forth_until_event(parent);
    CuAssertIntEquals(tc, 11, get_used_pages_count());
}


int main(int argc, char *argv[]) {
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    // these are all the tests but you'll probably want to comment
    // them out and add them back one step at a time

    SUITE_ADD_TEST(suite, test0_oneforth);
    // SUITE_ADD_TEST(suite, test1_twoforths);
    // SUITE_ADD_TEST(suite, test2_yields);
    // SUITE_ADD_TEST(suite, test3_page_count);
    // SUITE_ADD_TEST(suite, test4_identical_fork);
    // SUITE_ADD_TEST(suite, test5_fork_return);
    // SUITE_ADD_TEST(suite, test6_double_fork);
    // SUITE_ADD_TEST(suite, test7_copy_on_write);
    // SUITE_ADD_TEST(suite, test8_copy_on_write_parent_edit);
    // SUITE_ADD_TEST(suite, test9_double_fork_copy_on_write);
                                      
    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}
