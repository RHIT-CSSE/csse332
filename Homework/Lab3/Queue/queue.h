/* File: queue.h
 *
 * Author: Archana Chidanandan (4 Dec 2005)
 * Modified by Delvin Defoe, March 16, 2008
 *
 * This header file defines the structure of a queue to be used potentially 
 * with a variety of other projects. It also contains the prototpyes of 
 * functions used to manage the queue implemented in queue.c
*/
#ifndef QUEUE_QUEUE_H_
#define QUEUE_QUEUE_H_

#define FALSE 0
#define TRUE 1

typedef int Bool;

typedef struct node {
  int processId;
  int arrivalTime;
  int serviceTime;
  int remainingTime;
  struct node *next; /* points to the next node */
} Node;


typedef struct queue {
  Node *head;
  Node *tail;
} Queue;


/****************************************************
 * Function: deleteQueue
 * Remove all the nodes from the queue, freeing the
 * space allocated to each node. The head and tail 
 * pointers must each be set to NULL.
 ****************************************************/
void deleteQueue(Queue *Aqueue);


/************************************************************
 * Function: enqueue
 * Add a node to the tail of the queue. The space for the node 
 * will be allocated on the heap. The tail pointer will point 
 * to this node and the head pointer will continue to point to 
 * the node at the start of the queue. 
 * Return TRUE if successful, FALSE otherwise.
 ************************************************************/
Bool enqueue(Queue *Aqueue, int processId, int arrivalTime,
             int serviceTime, int remainingTime);


/***************************************************
 * Function: printQueue
 * Display the values in each node in the queue. 
 * Starts at the head.
 * Return the number of nodes in the queue.
 ***************************************************/
int printQueue(Queue Aqueue);



/****************************************************
 * Function: queueSize
 * Return the number of nodes in the queue.
 ***************************************************/
int queueSize(Queue Aqueue);



/***************************************************
 * Function: dequeue
 * Remove a node from the head of the queue and return its value.
 * Free the space at the heap occupied by this node.
 * The head pointer is updated to point to the next node.
 * If this is the last item in the queue, then set the head
 * and tail pointers to NULL.
 * If the queue is empty, return an item with process id = -1.
 ***************************************************/
Node dequeue(Queue *Aqueue);

#endif  // QUEUE_QUEUE_H_

