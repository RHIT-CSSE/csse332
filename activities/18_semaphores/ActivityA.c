#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>


int *array;
int array_size;
int share;
volatile unsigned long long sum;

void *run_thread(void *arg)
{
	int start_index = *((int *)arg);
	pthread_id_np_t tid;
	tid = pthread_getthreadid_np();
	printf("Thread %d starting from %d\n", tid, start_index);
	for(int i = start_index; i < start_index+share; ++i) {
		sum += array[i];
	}
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[2];
	int start_index[2];
	if (argc > 1)
		array_size = atoi(argv[1]);
	else
		array_size = 1024;

	if (array_size % 2) {
		fprintf(stderr, "Please enter a multiple of two for the size\n");
		exit(EXIT_FAILURE);
	}

	/* initialize the array */
	array = malloc(array_size * sizeof(int));
	for(int i = 0; i < array_size; ++i) {
		array[i] = 1;
	}

	share = array_size / 2;
	start_index[0] = 0;
	start_index[1] = share;
	pthread_create(&thread[0], NULL, run_fn, &start_index[0]);
	pthread_create(&thread[1], NULL, run_fn, &start_index[1]);

	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);

	printf("The value of the sum is %llu\n", sum);
}
