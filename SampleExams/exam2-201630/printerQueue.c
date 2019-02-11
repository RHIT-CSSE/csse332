/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/**
   In this system, there are 3 printer threads and one application
   thread.  The application generates strings that must be printed by
   a printer.  To do a print, the application puts a string pointer in
   the thingToPrint global variable and then (somehow) signals that it
   is ready to print.  A single printer thread then takes the
   thingToPrint and begins printing it.  The application should not
   modify thingToPrint while the printer thread is reading it - but
   once printing starts it can submit jobs to the other print threads.
   If the application wants to submit a job but all the printers are
   in use, the application should wait.

   Write a solution using semaphores that enforces these constraints.
**/

char* thingToPrint;

void *printer(void *arg) {
  char* currentJob;
  while(1) {
    currentJob = thingToPrint;
    printf("Printing %s \n", currentJob);
    sleep(1);
    printf("Finished Printing %s \n", currentJob);
  }
  return NULL;
}

void *application(void *arg) {
  char* jobs[] = {"Job One",
                   "Job Two",
                   "Job Three",
                   "Job Four",
                   "Job Five"};
  int i;
  
  for(i = 0; i < 5; i++) {
    printf("%s ready to print\n", jobs[i]);
    thingToPrint = jobs[i];
  }
  printf("All print jobs scheduled\n");
  sleep(2); //wait for any current jobs to finish
  return NULL;
}


int main(int argc, char **argv) {
  pthread_t app, p1, p2, p3;

  pthread_create(&app, NULL, application, NULL);
  pthread_create(&p1, NULL, printer, NULL);
  pthread_create(&p2, NULL, printer, NULL);
  pthread_create(&p3, NULL, printer, NULL);

  // printers will never finish so we only must
  // wait for the app
  pthread_join(app, NULL);



  printf("Everything finished.\n");
}
