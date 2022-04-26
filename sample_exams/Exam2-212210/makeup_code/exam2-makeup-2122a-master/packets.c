#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**
 * In this system, we have a processor that processes incoming network packets
 * as batches of 4 packets at a time. When a packet arrives, a few conditions
 * might happen:
 * 
 * 1) There are less than 4 packets in the processor's buffer, then the packet
 * must wait for at least 4 packets to arrive.
 * 
 * 2) The packet is the 4th packet to arrive, then two things might happen:
 *  
 *  2.1) There are no other packets processing, then the packet, along with 3
 *  others (for a total of 4 packets) will enter the processor to be processed.
 *  2.2) There are packets being processed, then the packet must wait until the
 *  ones in the processor finish processing and then enter alongside its batch.
 * 
 * Ask me questions if the above is not clear.
 * 
 * Implement the program described above using condition variables and mutex
 * locks. Please note that you are NOT allowed to use a semaphore to solve this
 * problem. 
 * 
 * Hint: I used a single lock and single condition variable to solve this
 * problem.
 */

#define NUM_PKTS 24

void* pkt(void *arg) {
  printf("Packet %u arrived through the network...\n", *(unsigned*)arg);

  // TODO: Add code here!

  // if waiting
  //  printf("Packet %u buffered at the processor...\n", *(unsigned *)arg);

  printf("Packet %u entered the processor...\n", *(unsigned*)arg);
  sleep(1);

  // TODO: Add code here!

}

int
main(int argc, char **argv)
{
  int i;
  pthread_t pkts[NUM_PKTS];
  unsigned pktid[NUM_PKTS];

  srand(time(NULL));

  for(i = 0; i < NUM_PKTS; i++) {
    pktid[i] = i+1;
    pthread_create(&pkts[i], 0, pkt, &pktid[i]);

    sleep(rand()%2);
  }

  for(i = 0; i < NUM_PKTS; i++) {
    pthread_join(pkts[i], 0);
  }

  return 0;
}