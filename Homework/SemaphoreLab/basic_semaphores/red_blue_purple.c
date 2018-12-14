/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*

  In this hypothetical scenario, we have two output channels: the red channel
  and the blue channel.  In this system, it's important that two
  different threads never try to output to the same channel at the same
  time (but one thread can write to red, at the same time a different
  thread can write to blue).

  There are 4 kinds of commands:
  WhiteCommand - needs neither channel
  RedCommand - needs to output to the red channel
  BlueCommand - needs to output to the blue channel
  PurpleCommand - needs to output to both channels

  Each thread function consists of a series of commands.  Use
  semaphores to ensure that the rule about channels is never violated but
  otherwise the threads execute in parallel as much as possible.

  To make this work properly you'll edit the red/blue/purple commands
  and main.  Don't edit the thread functions themselves (or you can,
  but we should be able to test with our own different thread
  functions and they should preserve the rules correctly regardless of
  what order the commands occur within them).

  HINT: to see if your code works correctly, it's easiest to look at
  only 1 color appears at a time.  To do this, run your code and dump
  the output to file.  Then use grep to only show lines matching the
  color you care about.

  You can use the provided makefile to compile your code.
  
  ./red_blue_purple > testout.txt
  grep Red testout.txt

  If your code is right you should see an alternating Start/End

 */

void redCommand() {
    printf("Start: Red\n");
    usleep(100);
    printf("End  : Red\n");
}

void blueCommand() {
    printf("Start: Blue\n");
    usleep(100);
    printf("End  : Blue\n");
}

void purpleCommand() {
    printf("Start: Blue\n");
    printf("Start: Red\n");
    usleep(100);
    printf("End  : Red\n");
    printf("End  : Blue\n");
}

void whiteCommand() {
    usleep(100);
}

void* thread1(void* p) {
    redCommand();
    redCommand();
    whiteCommand();
    redCommand();
    purpleCommand();
    redCommand();
    printf("thread 1 done\n");
    pthread_exit(NULL);
}

void* thread2(void* p) {
    redCommand();
    blueCommand();
    blueCommand();
    whiteCommand();
    purpleCommand();
    redCommand();
    blueCommand();
    printf("thread 2 done\n");
    pthread_exit(NULL);
}

void* thread3(void* p) {
    redCommand();
    purpleCommand();
    purpleCommand();
    purpleCommand();
    printf("thread 3 done\n");
    pthread_exit(NULL);
}



int main(int argc, char **argv) {
    pthread_t threads[3];

    // to fully test your code, feel free to try other configurations
    // (e.g. a couple thread1s running at the same time, etc.)
    pthread_create(&threads[0], NULL, thread1, NULL);
    pthread_create(&threads[1], NULL, thread2, NULL);
    pthread_create(&threads[2], NULL, thread3, NULL);


    int i;
    for (i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Everything finished\n");

    return 0;
}
