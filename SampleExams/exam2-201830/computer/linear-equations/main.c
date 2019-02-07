/* Copyright 2018 Rose-Hulman */
#include <unistd.h>
#include <sys/wait.h>
#include "./shm.h"

/* In this problem you will be adding code mostly to 2 files--this
 * file and solver.c. The shm module has all the code and data
 * structures that you need for working with shared memory (take a
 * look at shm.c for what we've provided).  You are expected to modify
 * the sharedMemoryStruct as needed.  That's the only change you need
 * to make in the shm module.
 * 
 * STEP 1 (4 points)
 * Begin in this file by creating a shared memory region and attaching
 * it to its address space. The shared memory key is stored in shm.h.
 * 
 * 
 * Next, prompt the user for 4 coordinates in the form x1, y1, x2, y2.
 * This is already done for you in the main function below.
 * 
 * Save the coordinates in the shared memory region. This is where
 * you should modify the sharedMemoryStruct to do so.
 * 
 * STEP 2 (3 points)
 * Create a new child process using fork() to run the solver program.
 * 
 *  STEP 3 (7 points)
 * solver should use the shared memory key to attach the shared memory
 * region to its address space.  solver should then use the coordinates
 * stored in shared memory to compute the slope and intercept of the line
 * segment connecting the points defined by the coordinates.  See solver.c
 * more more details.
 * 
 * STEP 4 (3 points)
 * After solver exits, this program should display to the console the equation
 * of the line connecting the points defined by the coordinates in shared
 * memory.
 * 
 * 
 * STEP 5 (3 points)
 * This program should then detach the shared memory region from its address 
 * space and remove it.  Note: detaching a shared memory region is not the same
 * as removing it.
 * 
 * Here's what a the results for a sample run of this program looks like:
 * 
 * 
 *  >> ./main
    Enter coordinates for 2 points as comma separated list of numbers: x1, y1, x2, y2:
    2, 5, 4, 13
    main saved your coordinates in shared memory region...

    Starting linear equation solver...
    Receive points (2.00, 5.00), (4.00, 13.00)
    Saving slope and intercept...
    linear equation solver has exited...
    equation: y = 4.00x + -3.00

    main has detached its shared memory region...
    main has removed its shared memory region...
    main is exiting...
 * */
int main(int argc, char *argv[]) {
    double x1, y1, x2, y2;
    sharedMemoryStruct *ShmPTR;
    char child[10] = "solver";

    /*  Uncomment this code if you get a weird shared memory error
        where you can't resize a shared region because it persists
        between runs of your code. Be sure to comment when no longer
        needed.
    */
    // int tempID = locateSharedMemory(SHM_KEY);
    // removeSharedMemory(tempID);

    /* Initialize shared memory values */
    printf("Enter coordinates for 2 points as comma ");
    printf("separated list of numbers: x1, y1, x2, y2: \n");
    scanf("%lf, %lf, %lf, %lf", &x1, &y1, &x2, &y2);
    ShmPTR->c = 0.5;
    printf("main saved your coordinates in shared memory region... \n\n");

    printf("Starting linear equation solver...\n");
    /* Create a new process using fork and run the solver program
     * in the child process using an "exec" function. The man pages
     * should be your friend here.  */
  
    /* Wait for the child process to exit then display equation */
    printf("linear equation solver has exited...\n");

    printf("main is exiting...\n");

    return 0;
}
