#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

/*
   In this problem, a group of savages have captures a number of rodents and
   would like to party and eat those rodents from a pot. They employ one cook
   that will cook these delicious rodents and add them to the pot.

   When a savage wants to eat, they go to the pot. If the pot has at least one
   delicious rodent in it, they will eat the rodent. If the pot is empty, the
   savage will wake up the cook and ask them to cook more delicious rodents until
   all rodents in captivity have been consumed.

   The cook wakes up only when the pot is empty. They will cook the rodents and
   add them to the pot. It will then tell the savages that there are delicious
   rodents in the pot ready to be eaten. Every time the cook wakes up, the will
   add 5 rodents to the pot.

   Implement the synchronization primitives between the savages and the cook.
   Your code should satisfy the following two conditions:
   1. The pot starts off as empty
   2. The savages cannot eat if the pot is empty
   3. The cook can only cook rodents if the pot is empty

HINT: I used three semaphores to solve this problem.

When you run your code, you should see something that looks like this:

$ ./savages
Savage waiting to eat
Cook is awake: Ready to cook delicious rodents...
Savage waiting to eat
Savage waiting to eat
Cook added 5 servings to the pot
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Cook is awake: Ready to cook delicious rodents...
Savage waiting to eat
Savage waiting to eat
Cook added 5 servings to the pot
Savage eating a rodent, nom nom nom nom....
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Cook is awake: Ready to cook delicious rodents...
Savage waiting to eat
Savage waiting to eat
Cook added 5 servings to the pot
Savage eating a rodent, nom nom nom nom....
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
Savage waiting to eat
Savage eating a rodent, nom nom nom nom....
Savage done eating, time to sleep
>>>>>>>>> ALL DONE <<<<<<<<
*/

#define MAX_MEALS_IN_POT 5

int rodents_in_captivity = 15;
int rodents_devoured = 0;

void *savage(void *ignored)
{
	while (rodents_devoured < 15) {
		printf("Savage waiting to eat\n");

		printf("Savage eating a rodent, nom nom nom nom....\n");
		rodents_devoured++;

		printf("Savage done eating, time to sleep\n");
		sleep(1);
	}
	return NULL;
}

void *cook(void *ignored)
{
	while (rodents_in_captivity > 0) {
		printf("Cook is awake: Ready to cook delicious rodents...\n");
		sleep(2);
		rodents_in_captivity -= MAX_MEALS_IN_POT;
		printf("Cook added %d servings to the pot\n", MAX_MEALS_IN_POT);
	}
	return NULL;
}

int
main(int argc, char **argv)
{
	/* DO NOT REMOVE THIS LINE */
	setbuf(stdout, NULL);

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

	printf(">>>>>>>>> ALL DONE <<<<<<<<\n");
}
