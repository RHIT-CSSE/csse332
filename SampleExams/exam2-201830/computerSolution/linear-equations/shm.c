#include "shm.h"

/* Create a shared memory region */
int createSharedMemoryRegion(key_t key) {
    int ShmID = shmget(key, sizeof(sharedMemoryStruct), IPC_CREAT | 0666);
    if (ShmID < 0) {
        perror("*** shmget error -- could not create shared memory region ***\n");
        exit(1);
    }
    return ShmID;
}

/* Attach a shared memory region */
sharedMemoryStruct* attachSharedMemoryRegion(int ShmID) {
    sharedMemoryStruct *ShmPTR = (sharedMemoryStruct *)shmat(ShmID, NULL, 0);
    if (ShmPTR == NULL) {
        perror("*** shmat error -- could not attach shared memory region ***\n");
        exit(1);
    }
    return ShmPTR;
}

/* Locate a shared memory region */
int locateSharedMemory(key_t key) {
    int ShmID = shmget(key, sizeof(sharedMemoryStruct), 0666);
    if (ShmID < 0) {
        perror("*** shmget error -- could not locate shared memory region ***\n");
        exit(1);
    }
    return ShmID;
}

/* Detach shared memory region */
void detachSharedMemory(sharedMemoryStruct *ShmPTR) {
    if (shmdt((void *) ShmPTR) < 0) {
        perror("*** shmdt error -- not valid start of shared memory region ***\n");
    }
}

/* Remove shared memory region */
void removeSharedMemory(int ShmID) {
    if (shmctl(ShmID, IPC_RMID, NULL) < 0) {
        perror("Could not remove shared memory region for given ID.\n");
    }
}