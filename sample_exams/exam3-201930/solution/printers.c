#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*

  In this system there are 3 3D printers numbered 0 1 2.
  
  Any printer can print any job, but each job must be sent to exactly
  one printer.  Once a printer is working a job, no additional jobs
  can be sent to that printer until the job finishes.  The system
  needs to print which printer a parituclar job is assigned to (but,
  if there's more than one printer available, it doesn't match which
  printer it is).

  We want to ensure that a job is never sent to a printer that is
  currently working another job.  We also want to ensure maximal
  resource utilization - i.e. a job is never waiting while there is
  some printer avilable.  

  For full credit, this solution should work without busywaits
  (i.e. unassigned jobs should not be constantly reading memory while
  they wait for a printer to become available).

 */

#define NUM_PRINTERS 3
#define NUM_JOBS 10

char* currently_printing[NUM_PRINTERS]; // the code uses
                                        // currently_printing[0] to
                                        // refer to what printer 0 is
                                        // currently printing etc.

sem_t mutex;
sem_t avail_printers;

void* thread_that_prints(void* name) {
    // imagine this is doing work
    sleep(1);
    
    sem_wait(&avail_printers);
    sem_wait(&mutex);

    int printer;
    for(printer = 0; printer < NUM_PRINTERS; printer++) {
        if(currently_printing[printer] == NULL) {
            usleep(100); //this makes one of the concurrency bugs obvious
            currently_printing[printer] = (char*) name;
            break;
        }
    }

    sem_post(&mutex);
    
    printf("Job '%s' assigned to printer %d\n", (char*) name, printer);
    sleep(2);
    printf("Job '%s' finished, printer %d free\n", (char*) name, printer);
    currently_printing[printer] = NULL;
    sem_post(&avail_printers);
    
    return NULL;
    
}

int main(int argc, char **argv) {

    char* job_names[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
    pthread_t job_threads[NUM_JOBS];
    
    sem_init(&mutex, 0, 1);
    sem_init(&avail_printers, 0, NUM_PRINTERS);
    
    for(int i = 0; i < NUM_PRINTERS; i++) {
        currently_printing[i] = NULL;
    }
    for(int i = 0; i < NUM_JOBS; i++) {
        pthread_create(&job_threads[i], NULL, thread_that_prints, job_names[i]);
    }
    for(int i = 0; i < NUM_JOBS; i++) {
        pthread_join(job_threads[i], NULL);
    }
    printf("done");
    return 0;
}
