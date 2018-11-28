#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
/*

So in this example we're going to do something that's unusual in C but
more common in more-OO languages - have a hetergenous collection of
object types.

Here's the premise: we want do add some numbers together.  But we want
to add two kinds of numbers - everyday good old doubles, and fractions
(which we need to keep as fractions to prevent floating point
rounding).  Here's the fraction struct:

*/
struct Fraction {
  int numerator;
  int denominator;
};

/*

We're going to have lists that are heterogenous (i.e. have both kinds
of numbers).  Summing a list will produce a double (obviously,
therefore the sum will be rounded but we're willing to live that).

How will we represent this list?  As an array of structs like this:

 */
const char DOUBLE_TYPE = 'd';
const char FRACTION_TYPE = '%';

struct Number {
    char type;
    void* value; //points to either a Fraction struct or an double

    // uncomment this for Step 3
    // double (*to_double)(struct Number*);
};

/*

Step 1: Implement the sum function

Remember that similar to java, an integer divsion in C always yields
an int unless you cast first.

 */

double number_sum(struct Number* num_array, int size) {

    double sum;
    for(int i = 0; i < size; i++) {
        struct Number *current = num_array + i;
        if(current->type == 'd') {
            sum = sum + *((double*) current->value);
        }
        if(current->type == '%') {
            struct Fraction *frac = current->value;
            double result = (double) frac->numerator/frac->denominator;
            sum = sum + result;
        }
    }
    return sum;
}

void test_number_sum(CuTest *tc) {


    double f = 5;
    double f2 = 0.75;
    struct Fraction frac, frac2;
    frac.numerator = 1;
    frac.denominator = 2;

    frac2.numerator = 3;
    frac2.denominator = 4;

    struct Number nums[4];
    nums[0].type = DOUBLE_TYPE;
    nums[0].value = &f;
    nums[1].type = FRACTION_TYPE;
    nums[1].value = &frac;

    nums[2].type = DOUBLE_TYPE;
    nums[2].value = &f2;
    nums[3].type = FRACTION_TYPE;
    nums[3].value = &frac2;

    
    CuAssertDblEquals(tc, 5.5, number_sum(nums, 2),0.0001);
    CuAssertDblEquals(tc, 1.5, number_sum(nums + 2, 2),0.0001);
    CuAssertDblEquals(tc, 7, number_sum(nums, 4),0.0001);
}


//these are defined below but I want them upfront because I refer to
//them in the allocate functions for Step 3

double to_double_double(struct Number* double_number);
double to_double_fraction(struct Number* frac_number);


/*

Step 2: Allocating number contents with dynamic memory

Building number objects is sort of annoying.  What we want is two
functions, both of which take a pointer to an existing number object
and which use malloc to allocate dynamic memory to fill the number
with an appropiate type - using the given parameters.

Of course, we need to write another function that lets us free the
number contents as well.  There only needs to be one of these, because
each number is freed in the same way.

Some C programmers might find the way these functions internally call
malloc a little sketchy - plus we're allocating much smaller chunks
that you'd really want to in practice.

 */

void allocate_double(struct Number* num_to_fill, double value) {
    
}

void allocate_fraction(struct Number* num_to_fill, int numerator, int denominator) {
    
}

void free_allocation(struct Number* num_to_free) {
    
}


void test_allocate(CuTest *tc) {


    struct Number nums[4];
    allocate_double(&nums[0], 10);
    allocate_fraction(&nums[1], 1, 2);

    allocate_double(&nums[2], 100);
    allocate_fraction(&nums[3], 1, 10);

    CuAssertDblEquals(tc, 10.5, number_sum(nums, 2),0.0001);
    CuAssertDblEquals(tc, 100.1, number_sum(nums + 2, 2),0.0001);
    CuAssertDblEquals(tc, 110.6, number_sum(nums, 4),0.0001);

    for(int i = 0; i < 4; i++) {
        free_allocation(&nums[i]);
    }
}

/*

Step 3: Virtual functions

As things stand, our code works but we can't add new types to our
heterogenous collection without modifying many parts of the code.
Rather than having to rely on the type field, what we'd like is for
each number to have a function pointer to a function that will convert
it to a double.  Then, if new types are added we can just associate
those objects with some newly created conversion.  Uncomment the test
case below to see how it ought to work.

The make this work you'll need to modify the number struct and your
allocate_fraction and allocate_double functions.  I've implemented the
conversion functions for your convenience.

 */

double to_double_double(struct Number* double_number) {
    double *val = double_number->value;
    return *val;
}

double to_double_fraction(struct Number* frac_number) {
    struct Fraction* val = frac_number->value;
    return (double) val->numerator/(double) val->denominator;
}


void test_virtual_convert(CuTest *tc) {

  //ucomment this when you're ready
   /* 
    struct Number nums[4];
    allocate_double(&nums[0], 10);
    allocate_fraction(&nums[1], 1, 2);

    allocate_double(&nums[2], 100);
    allocate_fraction(&nums[3], 1, 10);
    
    CuAssertDblEquals(tc, 10, nums[0].to_double(&nums[0]),0.0001);
    CuAssertDblEquals(tc, 0.5, nums[1].to_double(&nums[1]),0.0001);
    
    CuAssertDblEquals(tc, 100, nums[2].to_double(&nums[2]),0.0001);
    CuAssertDblEquals(tc, 0.1, nums[3].to_double(&nums[3]),0.0001);

    */


}



int main(int argc, char *argv[]) {
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_number_sum);
    SUITE_ADD_TEST(suite, test_allocate);
    SUITE_ADD_TEST(suite, test_virtual_convert);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}
