#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "CuTest.h"

/*

This function takes two pointers to integers, and returns the sum of
the integers being pointed to.

Look at the test cases for example of usage

 */
int add_with_pointers(int *val1, int *val2) {
    return 0;
}

void test_add_with_pointers(CuTest *tc) {
    int a = 7;
    int b = 11;
    CuAssertIntEquals(tc, 18, add_with_pointers(&a, &b));
    CuAssertIntEquals(tc, 22, add_with_pointers(&b, &b));
}


/*

This function takes two pointers to integers.  The first integer is
intended to be smaller, the second integer is intended to be larger.

If the order is correct, the function should do nothing.
If the order is wrong, the function should swap the two values.

Look at the test cases for example of usage
 */

void ensure_correct_order(int *should_be_smaller, int *should_be_bigger) {

  
}

void test_ensure_correct_order(CuTest *tc) {
    int a = 7;
    int b = 11;
    ensure_correct_order(&a, &b);
    CuAssertIntEquals(tc, 7, a);
    CuAssertIntEquals(tc, 11, b);

    ensure_correct_order(&b, &a);
    CuAssertIntEquals(tc, 11, a);
    CuAssertIntEquals(tc, 7, b);

}


/*

This function takes two pointers to integers and tests their equality:

The function returns 2 if the two values point to the same location in
memory.

The function returns 1 if the two values point to different locations
in memory, but those locations contain equal integers.

The function returns 0 if neither 1 nor 2 is true.

Look at the test cases for example of usage

 */

int special_equals(int *val1, int *val2) {
  return 0;
}

void test_special_equals(CuTest *tc) {
    int a = 7;
    int b = 11;
    int c = 11;
    CuAssertIntEquals(tc, 2, special_equals(&a, &a));
    CuAssertIntEquals(tc, 1, special_equals(&b, &c));
    CuAssertIntEquals(tc, 0, special_equals(&a, &b));
    CuAssertIntEquals(tc, 0, special_equals(&a, &c));
}


/*

This function takes two equal length c-style strings and determines
which of them has a Q.  The address of the string with a Q should be
set in the output field.  If neither string contains a Q, the output
address should be set to NULL.

Remember that in C, all strings are terminated with a '\0' character
so that's what you should check to see if you're at the end of the
string.

Also remember you can treat pointers like arrays so if you want to get
the 5th character of a string, you can do either of these:

  char* letters = "abcde";
  char second_letter = letters[1]; //fine
  char second_letter_with_pointer_math = *(letters + 1); //also fine
  printf("letters %c %c\n", second_letter, second_letter_with_pointer_math);

You can assume:
1.  The strings are equal length
2.  At most one string will contain a Q

You can use C string search functions if you wish, but I think it's
easier to just write a little for loop.

Look at the test cases for example of usage

 */

void string_with_q(char *s1, char* s2, char** output) {

  
}

void test_string_with_q(CuTest *tc) {
    char* s1 = "abQ";
    char* s2 = "abc";
    char* output;

    string_with_q(s1, s2, &output);
    CuAssertPtrEquals(tc,s1,output);
    s1 = "xyz";
    string_with_q(s1, s2, &output);
    CuAssertPtrEquals(tc,NULL,output);

    //a few more test cases in a slightly shorter style
    string_with_q("12345","zzQzz",&output);
    CuAssertStrEquals(tc,"zzQzz",output);
    string_with_q("Q2345","zzzzz",&output);
    CuAssertStrEquals(tc,"Q2345",output);
    string_with_q("","",&output);
    CuAssertPtrEquals(tc,NULL,output);
    
}


/*

The next functions utilize a simple linked list struct.  I've provided
functions that create and destroy linked lists.

 */

struct ll_node {
    int val;
    struct ll_node *next;
};

struct ll_node* make_sample_list(int size) {
    if(size == 0)
        return NULL;
    struct ll_node *new_node = malloc(sizeof(struct ll_node));
    (*new_node).val = 1;
    //using recursion isn't most efficient, but it is simple
    struct ll_node* rest_of_list = make_sample_list(size - 1);
    (*new_node).next = rest_of_list;
    return new_node;
}

void free_list(struct ll_node* head) {
    if(head == NULL)
        return;
    free_list((*head).next);
    free(head);
}

/*

Computes the sum of all values in the list.  An empty list is a NULL head 
and it's sum is 0.

Look at the test cases for example of usage

 */
int sum_linked_list(struct ll_node* head) {
    
    return 0;
}

void test_sum_linked_list(CuTest *tc) {
    struct ll_node* head = make_sample_list(3);
    CuAssertIntEquals(tc, 3, sum_linked_list(head));
    free_list(head);
    head = make_sample_list(10);
    CuAssertIntEquals(tc, 10, sum_linked_list(head));
    (*(*head).next).val = 100;
    CuAssertIntEquals(tc, 109, sum_linked_list(head));
    free_list(head);
    head = NULL; //empty list
    CuAssertIntEquals(tc, 0, sum_linked_list(head));
}


int main(int argc, char *argv[]) {
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_add_with_pointers);
    SUITE_ADD_TEST(suite, test_special_equals);
    SUITE_ADD_TEST(suite, test_ensure_correct_order);
    SUITE_ADD_TEST(suite, test_string_with_q);
    SUITE_ADD_TEST(suite, test_sum_linked_list);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}
