#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
 * In a water park, people take a canoe for a short tour for fun. Here are some
 * constraints/regulations we have to work with when coordinating the canoe
 * tour.
 * 
 * 1) There is only one canoe in this park, i.e., only one tour at a time. 
 * 
 * 2) The tourists include both adults and kids (different pthreads in our
 * program). An adult can ride the canoe alone, while a cannot. A kid must take
 * the tour with an adult. The capacity of the canoe is exactly one adult or one
 * adult and one kid. 
 * 
 * 3) Assume all the adults are fiendly that always voluntarily take a kid with
 * them if there is a kid waiting. However, if there is no kid waiting when an
 * adult starts the tour, the adult can go alone without delay and wait.
 * 
 * 4) In the kid and adult functions, both will print out messages before and
 * after finishing a tour. Particularly, for the adult function, it will print
 * out different messages after finishing canoeing to identify whether or not
 * this adult took the tour with a kid.
 * 
 * Hint: I used a total of 2 mutex locks and one condition variable to solve
 * this problem.
 * 
 * Here is a sample output that I got from running my solution:
 * 
 *  $ ./canoe 
an Adult is waiting...
                 ---> an Adult is canoeing.
a Kid is waiting...
                 an Adult finished canoeing. ---> 
a Kid is waiting...     a Kid is waiting...     a Kid is waiting...     an Adult is waiting...
                 ---> an Adult is canoeing.
an Adult is waiting...
                ---> a Kid is canoeing.
an Adult is waiting...  an Adult is waiting...  an Adult is waiting...
                 an Adult finished canoeing with a Kid. ---> 

                 ---> an Adult is canoeing.

                ---> a Kid is canoeing.

                 an Adult finished canoeing with a Kid. ---> 

                 ---> an Adult is canoeing.

                ---> a Kid is canoeing.

                 an Adult finished canoeing with a Kid. ---> 

                 ---> an Adult is canoeing.

                ---> a Kid is canoeing.

                 an Adult finished canoeing with a Kid. ---> 

                 ---> an Adult is canoeing.

                 an Adult finished canoeing. ---> 
All done
 */


void *adult(void *arg)
{
	printf("an Adult is waiting...\t");

  // TODO : Add code here!

	printf("\n\t\t ---> an Adult is canoeing.\n");
	sleep(1);

  // TODO: Add code here!

	// if with kid {
	// 	printf("\n\t\t an Adult finished canoeing with a Kid. ---> \n");
	// } else {
	// 	printf("\n\t\t an Adult finished canoeing. ---> \n");
	// }

	return NULL;
}

void *kid(void *arg)
{
	printf("a Kid is waiting...\t");

  // TODO: Add code here!

	printf("\n\t\t---> a Kid is canoeing.\n");
}

int
main(int argc, char **argv)
{
  int i = 0;
  pthread_t threads[10];

  // TODO: Add code here

  pthread_create(&threads[i++], NULL, adult, NULL);
  pthread_create(&threads[i++], NULL, kid, NULL);
  sleep(1);
  pthread_create(&threads[i++], NULL, kid, NULL);
  pthread_create(&threads[i++], NULL, kid, NULL);
  pthread_create(&threads[i++], NULL, kid, NULL);
  sleep(1);
  pthread_create(&threads[i++], NULL, adult, NULL);
  pthread_create(&threads[i++], NULL, adult, NULL);
  pthread_create(&threads[i++], NULL, adult, NULL);
  pthread_create(&threads[i++], NULL, adult, NULL);
  pthread_create(&threads[i++], NULL, adult, NULL);
  for (int j = 0; j < 10; j++) pthread_join(threads[j], NULL);
  printf("All done\n");
  return 0;
}