#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/**
   In this system there are two kinds of processes: ordinary threads
   and the (single) low priority background thread.  

   Both ordinary threads and the background thread need to access the
   critical section and only one thread can access it at once.  The
   rule is that if ordinary thread wants access, the background
   process should never gain access to the critical section.  If no
   ordinary thread wants access, the background process should get
   access.  Ordinary threads also interefere with other ordinary
   threads (that is, only one ordinary thread can be in the critical
   section at once).

   Note that if the background process is currently IN the critical
   section, it should prevent ordinary threads from entering the
   critical section (otherwise its a concurrency bug).  But once
   ordinary threads have started waiting, the background process
   should not be able to run again till every ordinary thread has
   gotten access.

   Example output:

Background thread thread wants to enter the critical section
Background thread starts the critical section
Background thread finishes the critical section
Ordinary thread wants to enter the critical section
Ordinary thread starts the critical section
Ordinary thread wants to enter the critical section
Background thread thread wants to enter the critical section
Ordinary thread finishes the critical section
Ordinary thread starts the critical section
Ordinary thread finishes the critical section
Background thread starts the critical section
Ordinary thread wants to enter the critical section
Ordinary thread wants to enter the critical section
Background thread finishes the critical section
Ordinary thread starts the critical section
Background thread thread wants to enter the critical section
Ordinary thread finishes the critical section
Ordinary thread starts the critical section
Ordinary thread wants to enter the critical section
Ordinary thread wants to enter the critical section
Ordinary thread finishes the critical section
Ordinary thread starts the critical section
Ordinary thread finishes the critical section
Ordinary thread starts the critical section
Ordinary thread finishes the critical section
Background thread starts the critical section
Background thread finishes the critical section
Everything finished.

**/
   
void *ordinaryThread(void *arg) {

    printf("Ordinary thread wants to enter the critical section\n");

    printf("Ordinary thread starts the critical section\n");
    sleep(1);
    printf("Ordinary thread finishes the critical section\n");

    return (void *)0;
}

void *backgroundThread(void *arg) {

    // this thread never finishes
    while(1) {
        printf("Background thread thread wants to enter the critical section\n");

        printf("Background thread starts the critical section\n");
        sleep(1); // some stuff in the critical section
        printf("Background thread finishes the critical section\n");

        sleep(1); //background also does some stuff not in the critical section
    }
    return (void *)0;
}


int main(int argc, char **argv) {
  pthread_t threads[6];
  pthread_t background_t;

  // create the background thread
  pthread_create(&background_t, NULL, backgroundThread, NULL);

  sleep(1);
  
  // create two ordinary threads
  pthread_create(&threads[0], NULL, ordinaryThread, NULL);
  pthread_create(&threads[1], NULL, ordinaryThread, NULL);

  sleep(3); //pause which lets the background thread run again

  pthread_create(&threads[2], NULL, ordinaryThread, NULL);
  pthread_create(&threads[3], NULL, ordinaryThread, NULL);
  sleep(2); // a short sleep, but not enough to actually let the background thread run
  pthread_create(&threads[4], NULL, ordinaryThread, NULL);
  pthread_create(&threads[5], NULL, ordinaryThread, NULL);

  // wait for all ordinary threads to finish
  for(int i = 0; i < 6; i++) {
      pthread_join(threads[i], NULL);
  }

  sleep(1); //let the backgroud run a bit at the end before we quit
  
  printf("Everything finished.\n\n");
}
