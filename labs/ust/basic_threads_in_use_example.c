#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "basic_threads.h"

/*

This is some code that illustrates how the basic_threads library is
intended to be used.

*/

int percent_complete;

void gui_thread() {
  int last_update = 0;
  while(percent_complete < 100) {
    // we only want to update in 10% intervals
    if(percent_complete / 10 != last_update / 10) {
      last_update = percent_complete;
      printf("processing %d percent complete...\n", percent_complete);
    }
    yield();
  }
  printf("processing done\n");
}

void processing_thread() {
  for(percent_complete = 0; percent_complete < 100; percent_complete += rand() % 10) {
    int time_to_work = rand() % 2;
    sleep(time_to_work);
    yield();
  }
}

int main() {
  initialize_basic_threads();
  percent_complete = 0;
  create_new_thread(gui_thread);
  create_new_thread(processing_thread);
  schedule_threads();

  printf("program quitting.\n");
}
