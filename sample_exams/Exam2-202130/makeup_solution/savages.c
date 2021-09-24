#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

/*
   In this problem, a group of savages have captures a number of humans and
   would like to party and eat those humans from a pot. They employ one cook
   that will cook these delicious humans and add them to the pot.

   When a savage wants to eat, they go to the pot. If the pot has at least one
   delicious human in it, they will eat the human. If the pot is empty, the
   savage will wake up the cook and ask them to cook more delicious humans until
   all humans in captivity have been consumed.

   The cook wakes up only when the pot is empty. They will cook the humans and
   add them to the pot. It will then tell the savages that there are delicious
   humans in the pot ready to be eaten.

   Implement the synchronization primitives between the savages and the cook.
   Your code should satisfy the following two conditions:
   1. The pot starts off as empty
   2. The savages cannot eat if the pot is empty
   3. The cook can only cook humans if the pot is empty

HINT: I used three semaphores to solve this problem.

When you run your code, you should see something that looks like this:

$ ./savages
Savage waiting to eat
Cook is awake: Ready to cook delicious humans...
Savage waiting to eat
Savage waiting to eat
Cook added 5 servings to the pot
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Cook is awake: Ready to cook delicious humans...
Savage waiting to eat
Savage waiting to eat
Cook added 5 servings to the pot
Savage eating a human, nom nom nom nom....
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Cook is awake: Ready to cook delicious humans...
Savage waiting to eat
Savage waiting to eat
Cook added 5 servings to the pot
Savage eating a human, nom nom nom nom....
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a human, nom nom nom nom....
Savage done eating, time to sleep
>>>>>>>>> ALL DONE <<<<<<<<
*/

#define MAX_MEALS_IN_POT 5

int servings = 0;
int humans_in_captivity = 15;
int humans_devoured = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void *savage(void *ignored)
{
	while (humans_devoured < 15) {
		printf("Savage waiting to eat\n");
		sem_wait(&mutex);
		if (servings == 0) {
			sem_post(&empty);
			sem_wait(&full);
			servings += MAX_MEALS_IN_POT;
		}

		printf("Savage eating a human, nom nom nom nom....\n");
		servings--;
		humans_devoured++;
		sem_post(&mutex);

		printf("Savage done eating, time to sleep\n");
		sleep(1);
	}
	return NULL;
}

void *cook(void *ignored)
{
	while (humans_in_captivity > 0) {
		sem_wait(&empty);
		printf("Cook is awake: Ready to cook delicious humans...\n");
		sleep(2);
		humans_in_captivity -= MAX_MEALS_IN_POT;
		printf("Cook added %d servings to the pot\n", MAX_MEALS_IN_POT);
		sem_post(&full);
	}
	return NULL;
}

int
main(int argc, char **argv)
{
	/* DO NOT REMOVE THIS LINE */
	setbuf(stdout, NULL);
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, 0);
	sem_init(&full, 0 , 0);

	const int num_savages = 3;
	pthread_t savages[num_savages];

	pthread_t cookThread;
	pthread_create(&cookThread, NULL, cook, NULL);

	for(int i = 0; i < num_savages; i++) {
		pthread_create(&savages[i], NULL, savage, NULL);
	}

	for(int i = 0; i < num_savages; i++) {
		pthread_join(savages[i], NULL);
	}
	pthread_join(cookThread, NULL);

	sem_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	printf(">>>>>>>>> ALL DONE <<<<<<<<\n");
}
