#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#include <sys/time.h>

/*
 * In this problem, we will try to perform the vector dot product operation in a
 * parallel manner. The dot product of two arrays is simply the sum of the
 * products of their elements. For example, if v1 = [1,2,3] and v2=[3,4,5] then
 * the dot product is equal to 1*3 + 2*4 + 3*5 = 3 + 8 + 15 = 26.
 *
 * This question refers to the following components:
 *	- v1: an input array of size VEC_SIZE
 *	- v2: another input array of size VEC_SIZE
 *	- temp: a temporary array to hold the element-by-element product of v1
 *		and v2
 *	- result: the sum of the elements in the array temp.
 *
 *
 * For simplicity, we will perform this action in two steps, we first compute
 * the element by element product of both arrays and store that in temp, and
 * then sum the element of temp and save the outcome in the result variable.
 *
 * We already provide you with code to perform these two steps sequentially.
 * Your job is to parallelize these two functions so that we can run the code in
 * parallel.
 *
 * STEP 1: PARALLELIZE `product_handler`
 *
 * We will first start by adding code to parallelize the element-by-element
 * product operation. Take a look at the sequential code that is provided for
 * you and make sure you understand it. Ask questions if you need to.
 *
 * Do the following:
 * - Add code to the `product_handler` function to make it runnable by pthreads.
 *   When run as a thread function, a pthread will pass a non-NULL pointer as
 *   the input argument to this function.
 * - Add code in the main function to set up pthreads (including create and
 *   join). After joining pthreads, add one line of code `sum_handler(NULL);` to
 *   cal the sum function to run sequentially after the product has been
 *   computed.
 * - Uncomment the code that measures the run-time and prints out the time cost.
 *   The time cost SHOULD INCLUDE the thread creation, the computation of the
 *   temp array, and the summation of temp using `sum_handler(NULL);`.
 *
 * After you complete this step, the output of this program should look like:
 *	$ ./dotproduct
 *	Non-parallel method computed dot product in 42697 uses.
 *	Parallel method computed dot product in 35680 uses.
 *
 *
 * STEP 2: PARALLELIZE `sum_handler`
 *
 * In this step, we will add code to parallelize the summation of the elements
 * in the temp array. First, take a look at the code in `sum_handler` and make
 * sure you understand it. Ask questions if you need to.
 *
 * Similar to the previous step, you need to add code to the `sum_handler`
 * function as well as in main (to create and join the pthreads).
 *
 * To maximize parallelism, you MUST create (i.e., call `pthread_create`) all
 * pthreads (NUM_PRODUCT_THREADS product threads and NUM_SUM_THREADS sum
 * threadS).
 *
 * However, there are two things we must take care of in this case:
 * 1. A sum thread should should wait until the part of temp that it must sum is
 *    ready. In other words, the sum thread should not run until the product
 *    thread that generates the part of temp that it is responisble for has
 *    already completed.
 * 2. There is a race condition on the `result` variable if you try to use the
 *    code that we provide you use as is. You need to use semaphores to synchronize
 *    access to the the `result` variable. (Hint: we have done this problem or
 *    something similar in class).
 *
 * After completing this step, you should see:
 *
 *	$ ./dotproduct
 *	Non-parallel method computed dot product in 42798 uses.
 *	Parallel method computed dot product in 35790 uses.
 *
 * As you can see, we didn't gain too much in terms of parallelism, but we had
 * some fun and practice ;)
 */

#define VEC_SIZE 5000000
#define NUM_PRODUCT_THREADS 2
#define NUM_SUM_THREADS 2

double *v1;
double *v2;

double *temp;
double result = 0.0;

sem_t sum_go[2], sum_lock;

struct threads_args {
	int index;
	int start;
	int end;
};

void *product_handler(void *arg)
{
	/* DO NOT MODIFY THE CODE BELOW */
	if (arg == NULL) {
		for(int i = 0;i < VEC_SIZE;i++) {
			temp[i] = v1[i] * v2[i];
		}

		return NULL;
	}
	/* DO NOT MODIFY THE CODE ABOVE */

	/* ################################################# */
	/* ADD YOUR CODE HERE */
	struct threads_args *targs = (struct threads_args *)arg;
	for(int i = targs->start; i < targs->end; i++) {
		temp[i] = v1[i] * v2[i];
	}

	sem_post(&sum_go[targs->index]);
	return NULL;
}

void *sum_handler(void *arg)
{
	/* DO NOT MODIFY THE CODE BELOW */
	if (arg == NULL) {
		for(int i = 0;i < VEC_SIZE;i++) {
			result += temp[i];
		}
		return NULL;
	}
	/* DO NOT MODIFY THE CODE ABOVE */

	/* ################################################# */
	/* ADD YOUR CODE HERE */
	struct threads_args *targs = (struct threads_args *)arg;
	double local = 0.0;
	sem_wait(&sum_go[targs->index]);
	for (int i = targs->start; i < targs->end; i++) {
		local += temp[i];
	}

	sem_wait(&sum_lock);
	result += local;
	sem_post(&sum_lock);

	return NULL;
}

int
main(int argc, char **argv)
{
	v1 = malloc(VEC_SIZE * sizeof(double));
	v2 = malloc(VEC_SIZE * sizeof(double));
	temp = malloc(VEC_SIZE * sizeof(double));

	struct timeval startt, endt;
	suseconds_t time_cost;
	srand(time(NULL));

	for(int i = 0; i < VEC_SIZE; i++) {
		v1[i] = 1.0 * i;
		v2[i] = 1.0 * (VEC_SIZE - i);
	}

	gettimeofday(&startt, NULL);
	product_handler(NULL);
	sum_handler(NULL);
	gettimeofday(&endt, NULL);
	time_cost = endt.tv_usec - startt.tv_usec;
	printf("Non-parallel method computed dot product in %ld uses.\n",
	       time_cost);

	/* clear out the temporary buffer */
	memset(temp, 0, VEC_SIZE * sizeof(double));
	/* reset the result */
	result = 0.0;

	/* DO NOT MODIFY THE CODE ABOVE */

	/* ################################################# */
	/* ADD YOUR CODE HERE */

	/* initialization and setup code */
	pthread_t prod_threads[NUM_PRODUCT_THREADS];
	pthread_t sum_threads[NUM_SUM_THREADS];
	sem_init(&sum_lock, 0, 1);
	for (int i = 0; i < NUM_SUM_THREADS; i++) {
		sem_init(&sum_go[i], 0, 0);
	}


	/* uncomment this line for getting time cost */
	gettimeofday(&startt, NULL);

	/* ################################################# */
	/* start thread creation here */
	for(int i = 0; i < NUM_PRODUCT_THREADS; i++) {
		struct threads_args arguments = {
			.start = i * VEC_SIZE / 2,
			.end = (i+1) * VEC_SIZE / 2
		};
		pthread_create(&prod_threads[i], NULL, product_handler, &arguments);
	}

	for(int i = 0; i < NUM_SUM_THREADS; i++) {
		struct threads_args arguments = {
			.start = i * VEC_SIZE / 2,
			.end = (i+1) * VEC_SIZE / 2
		};
		pthread_create(&sum_threads[i], NULL, sum_handler, &arguments);
	}


	for(int i = 0; i < NUM_PRODUCT_THREADS; i++)
		pthread_join(prod_threads[i], NULL);

	for(int i = 0; i < NUM_SUM_THREADS; i++)
		pthread_join(sum_threads[i], NULL);
	/* computation COMPLETES here */
	/* ################################################# */

	/* uncomment the next three lines for time computation */

	gettimeofday(&endt, NULL);
	time_cost = endt.tv_usec - startt.tv_usec;
	printf("Parallel method computed dot product in %ld uses.\n",
	       time_cost);

	sem_destroy(&sum_lock);
	for(int i = 0;i < VEC_SIZE; i++) {
		sem_destroy(&sum_go[i]);
	}

	free(v1);
	free(v2);
	free(temp);
}
