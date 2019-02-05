#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHM_KEY 1006

typedef struct {
  double c;
} sharedMemoryStruct;

int createSharedMemoryRegion(key_t key);
sharedMemoryStruct* attachSharedMemoryRegion(int ShmID);
int locateSharedMemory(key_t key);
void detachSharedMemory(sharedMemoryStruct *ShmPTR);
void removeSharedMemory(int ShmID);
