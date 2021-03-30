#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

/*

  In this system there are NUM_PRINTERS 3D printers numbered 0 1 etc.
  
  Any printer can print any job, but each job must be sent to exactly
  one printer.  Once a printer is working a job, no additional jobs
  can be sent to that printer until the job finishes.  The system
  needs to print which printer a parituclar job is assigned to (but,
  if there's more than one printer available, it doesn't matter which
  printer gets assigned the job).

  We want to ensure that a job is never sent to a printer that is
  currently working another job.  We also want to ensure maximal
  resource utilization - i.e. a job is never waiting while there is
  some printer avilable.  

  For full credit, this solution should work without busywaits
  (i.e. unassigned jobs should not be constantly reading memory while
  they wait for a printer to become available).

  Note that the jobs are not required to start/complete in any
  particular order (e.g. note e starts after f in the example).

  Example output:

Job 'a' assigned to printer 0
Job 'b' assigned to printer 1
Job 'c' assigned to printer 2
Job 'b' finished, printer 1 free
Job 'd' assigned to printer 1
Job 'a' finished, printer 0 free
Job 'f' assigned to printer 0
Job 'c' finished, printer 2 free
Job 'h' assigned to printer 2
Job 'd' finished, printer 1 free
Job 'g' assigned to printer 1
Job 'f' finished, printer 0 free
Job 'e' assigned to printer 0
Job 'h' finished, printer 2 free
Job 'i' assigned to printer 2
Job 'e' finished, printer 0 free
Job 'j' assigned to printer 0
Job 'g' finished, printer 1 free
Job 'i' finished, printer 2 free
Job 'j' finished, printer 0 free
done
 */

#define NUM_PRINTERS 3
#define NUM_JOBS 10

char* currently_printing[NUM_PRINTERS]; // the code uses
                                        // currently_printing[0] to
                                        // refer to what printer 0 is
                                        // currently printing etc.

void* thread_that_prints(void* name) {
    // imagine this is doing work
    sleep(1);
    
    //find a printer
    int printer = 0;

    // note this loop is a busywait, feel free to modify the code to
    // get rid of it
    while(1) {
        if(currently_printing[printer] == NULL) {
            usleep(100); //this makes one of the concurrency bugs obvious
            currently_printing[printer] = (char*) name;
            break;
        }
        printer = (printer + 1) % NUM_PRINTERS;
    }
    printf("Job '%s' assigned to printer %d\n", (char*) name, printer);
    usleep(10000 + (random() % 10000) ); // printing takes a random amount of time
    printf("Job '%s' finished, printer %d free\n", (char*) name, printer);
    currently_printing[printer] = NULL;
    return NULL;
    
}

int main(int argc, char **argv) {

    char* job_names[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
    pthread_t job_threads[NUM_JOBS];
    
    for(int i = 0; i < NUM_PRINTERS; i++) {
        currently_printing[i] = NULL;
    }
    for(int i = 0; i < NUM_JOBS; i++) {
        pthread_create(&job_threads[i], NULL, thread_that_prints, job_names[i]);
    }
    for(int i = 0; i < NUM_JOBS; i++) {
        pthread_join(job_threads[i], NULL);
    }
    printf("done\n");
    return 0;
}
