/************************************************************
 * Program: driver.c
 * Author: Archana Chidanandan (May 10, 2005)
 * Modified: Alex Hanson (December, 13, 2006)
 * Delvin Defoe, March 17, 2008
 * Archana Chidanandan, December 8, 2009
 *
 * This program is a test driver for the queue implementation.
 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "./queue.h"

#define NUM_ITEMS 5

int main(int argc, char *argv[]) {
  Queue myQueue;

  myQueue.head = NULL;
  myQueue.tail = NULL;

  int numItems;
  int processId = 2;
  int arrivalTime = 0;
  int serviceTime = 10;
  int queueLength = 0;

  /* Remove a node from an empty queue. */
  printf("About to remove a node from an empty queue...\n");
  Node node = dequeue(&myQueue);
  printf("Done removing a node from an empty queue. \n\n");
  if (node.processId == -1) {
    printf("Queue is empty! No item to remove. dequeue() "
            "is correct.\n");
  } else {
    fprintf(stderr, "Queue is not empty! It should be! There is an error in "
           "function dequeue()\n");
  }

  /* Add NUM_ITEMS nodes to the queue. The remaining time is initially
   * equal to the service time. */
  printf("About to add %d nodes to the queue...\n", NUM_ITEMS);
  for (numItems=0; numItems < NUM_ITEMS; numItems++) {
    enqueue(&myQueue, processId+numItems, arrivalTime+numItems,
            serviceTime+numItems, serviceTime+numItems);
  }
  printf("Done adding nodes to the queue. \n\n");

  /* Test queueSize. */
  printf("The number of nodes in the queue as returned by "
         "queueSize() is %d and should be %d.\n\n",
         queueSize(myQueue), NUM_ITEMS);

  /* Print values of nodes in myQueue. */
  printf("About to print values in nodes in the queue...\n");
  queueLength = printQueue(myQueue);
  printf("Done printing values in nodes in the queue. "
         "printQueue() returned the number of items as %d\n\n",
         queueLength);

  /* Remove a node from the queue. It will be removed from the head of
   * the queue. */
  printf("About to remove a node from the queue...\n");
  node = dequeue(&myQueue);
  printf("Done removing a node from the queue. \n\n");

  /* Print values of nodes in myQueue. */
  printf("About to print values of nodes in the queue...\n");
  queueLength = printQueue(myQueue);
  printf("Done printing values of nodes in the queue. "
         "There were %d items\n\n", queueLength);

  /* Remove another node from the queue. */
  printf("About to remove another node from the queue...\n");
  node = dequeue(&myQueue);
  printf("Done removing another node from the queue. \n\n");

  /* Print values of nodes in myQueue. */
  printf("About to print values of nodes in the queue...\n");
  queueLength = printQueue(myQueue);
  printf("Done printing values of nodes in the queue. "
         "There should be only %d items and there are %d\n\n",
         NUM_ITEMS - 2, queueLength);

  /* Remove remaining nodes from the queue. */
  printf("About to remove the remaining nodes from the queue...\n");
  for (numItems=0; numItems < NUM_ITEMS-2; numItems++) {
    node = dequeue(&myQueue);
  }
  printf("Done removing the remaining nodes from the queue.\n\n");
  if (myQueue.head != NULL) {
    fprintf(stderr, "The header pointer is not set to NULL by dequeue()\n");
  } else {
    printf("The header pointer is set to NULL by dequeue()\n");
  }
  if (myQueue.tail != NULL) {
    fprintf(stderr, "The tail pointer is not set to NULL by dequeue()\n");
  } else {
    printf("The tail pointer is set to NULL by dequeue()\n");
  }

  /* Print values of nodes in myQueue. */
  printf("About to print values of nodes in the queue...\n");
  queueLength = printQueue(myQueue);
  printf("Done printing values of nodes in the queue. "
         "There should be no items and %d were found\n\n", queueLength);

  /* Preparing to test deleteQueue. */
  printf("About to add %d nodes to the queue to test deleteQueue...\n",
         NUM_ITEMS);
  for (numItems=0; numItems < NUM_ITEMS; numItems++) {
    enqueue(&myQueue, processId+numItems, arrivalTime+numItems,
            serviceTime+numItems, serviceTime+numItems);
  }
  printf("Done adding nodes to the queue. \n\n");

  /* Free any space created for the queue. */
  printf("About to free space allocated for the queue...\n");
  deleteQueue(&myQueue);
  printf("Done freeing space allocated for the queue.\n\n");
  if (myQueue.head != NULL) {
    fprintf(stderr, "The header pointer is not set to NULL by "
            "deleteQueue()\n");
  } else {
    printf("The header pointer is set to NULL by deleteQueue()\n");
  }
  if (myQueue.tail != NULL) {
    fprintf(stderr, "The tail pointer is not set to NULL by deleteQueue()\n");
  } else {
    printf("The tail pointer is set to NULL by deleteQueue()\n");
  }

  /* Print values of nodes in myQueue. */
  printf("About to print values of nodes in the queue...\n");
  queueLength = printQueue(myQueue);
  printf("Done printing values of nodes in the queue. \n"
         "There should be no items in the queue and %d items were found\n",
         queueLength);

  /* Remove a node from an empty queue. */
  printf("About to remove a node from an empty queue...\n");
  node = dequeue(&myQueue);
  printf("Done removing a node from an empty queue.\n\n");
  if (node.processId != -1) {
    fprintf(stderr, "Found an item in an empty queue! Either dequeue() "
            "or deleteQueue() does not work correctly\n\n");
  } else {
    printf("The queue is empty.\n\n");
  }

  /* Add a node to the queue. */
  printf("About to add a node to an empty queue...\n");
  enqueue(&myQueue, 5, 4, 3, 1);
  printf("Done adding a node to the empty queue.\n\n");

  /* Print values of nodes in myQueue. */
  printf("About to print values of nodes in the queue...\n");
  queueLength = printQueue(myQueue);
  printf("Done printing values of nodes in the queue. "
         "There should be one item in the and %d item was found.\n\n",
         queueLength);

  /* Remove a node from the queue. */
  printf("About to remove the last node from the queue...\n");
  node = dequeue(&myQueue);
  printf("Done removing the last node from the queue. \n\n");

  /* Free any space created for the queue.  The queue should be empty. */
  printf("About to free space allocated for the queue...\n");
  deleteQueue(&myQueue);
  printf("Done freeing space allocated for the queue.\n\n");
  if (myQueue.head != NULL) {
    fprintf(stderr, "The header pointer is not set to NULL by "
            "deleteQueue()\n");
  } else {
    printf("The header pointer is set to NULL by deleteQueue()\n");
  }
  if (myQueue.tail != NULL) {
    fprintf(stderr, "The tail pointer is not set to NULL by deleteQueue()\n");
  } else {
    printf("The tail pointer is set to NULL by deleteQueue()\n");
  }

  return 0;
}
