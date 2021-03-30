#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

/* 
   In this system, there are two kinds of threads: passenger_thread
   representing travelers who need to use the bus and bus_thread representing
   the bus.  There are many passender_threads but only one bus.

   A passenger will work for some amount of time and then begin
   waiting at the bus stop.  If a bus is arrived at the bus top when
   the passenger begins waiting and has space, the passenger will
   board the bus.  The bus does not move and allows passengers to
   board until:

     1. The bus is full (use MAX_SEATS to figure out when this 
     happens)
     2. The last passenger in the system has boarded the bus (use
     NUM_PASSENGERS to figure out when this happens)

   Once one of these conditions is met the bus departs.  Once
   departed, passengers can longer board the bus.  The bus takes some
   travel time and the arrives at the destination.  Once the bus has
   arrived at the destination the passengers can disembark and finish.
   Note that the passengers are assumed to disembark immediately - the
   bus doesn't have to wait before starting to return to the bus stop.
   Returning to the bus stop takes travel time.  Once the travel time
   passes the bus arrives can begin loading boarding passengers again
   (including any who are waiting from before, or have arrived while
   the bus was in transit).

   Example output:

Bus arrived and waiting for passengers
Passenger 4 waiting for the bus
Passenger 4 boarding the bus
Passenger 5 waiting for the bus
Passenger 5 boarding the bus
Passenger 6 waiting for the bus
Passenger 6 boarding the bus
Passenger 2 waiting for the bus
Passenger 2 boarding the bus
Bus departing
Passenger 1 waiting for the bus
Passenger 3 waiting for the bus
Bus dropping of passengers & returning
Passenger 4 disembarking bus
Passenger 5 disembarking bus
Passenger 6 disembarking bus
Passenger 2 disembarking bus
Bus arrived and waiting for passengers
Passenger 1 boarding the bus
Passenger 3 boarding the bus
Bus departing
Bus dropping of passengers & returning
Passenger 1 disembarking bus
Passenger 3 disembarking bus
done

*/

#define NUM_PASSENGERS 6
#define MAX_SEATS 4

void* passenger_thread(void *num_p) {
    int num = *((int*) num_p);

    usleep(10000 + (random() % 10000) ); // passengers arrive at various times
    
    printf("Passenger %d waiting for the bus\n", num);

    printf("Passenger %d boarding the bus\n", num);

    printf("Passenger %d disembarking bus\n", num);
    return NULL;
}

void* bus_thread(void* ignored) {

    while(1) {
        printf("Bus arrived and waiting for passengers\n");
        printf("Bus departing\n");
        sleep(1); // travel time
        printf("Bus dropping off passengers & returning\n");
        sleep(1); // travel time to go back
    }
}

int main(int argc, char **argv) {

    int nums[NUM_PASSENGERS];
    pthread_t threads[NUM_PASSENGERS];
    pthread_t bus;
    
    pthread_create(&bus, NULL, bus_thread, NULL);

    for(int i = 0; i < NUM_PASSENGERS; i++) {
        nums[i] = i + 1;
        pthread_create(&threads[i], NULL, passenger_thread, &nums[i]);
    }

    for(int i = 0; i < NUM_PASSENGERS; i++) {
        pthread_join(threads[i], NULL);
    }
    // when all the passengers have finished we assume everything is done
    
    printf("done\n");
    return 0;
}
