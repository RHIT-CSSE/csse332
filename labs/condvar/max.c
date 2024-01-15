#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread(void *arg)
{
	char *letter = (char *)arg;
	printf("%c wants to enter the critical section\n", *letter);

	printf("%c is in the critical section\n", *letter);
	sleep(1);
	printf("%c has left the critical section\n", *letter);

	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[8];
	int i;
	char *letters = "abcdefgh";

	for(i = 0; i < 8; ++i) {
		pthread_create(&threads[i], NULL, thread, &letters[i]);

		if(i == 4)
			sleep(4);
	}

	for(i = 0; i < 8; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything finished...\n");

	return 0;
}
