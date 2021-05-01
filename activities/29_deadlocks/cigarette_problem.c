#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

sem_t agentSem, tobaccoSem, paperSem, matchSem;

void *
agentA(void *ignored)
{
	sem_wait(&agentSem);
	sem_post(&tobaccoSem);
	sem_post(&paperSem);
}

void *
agentB(void *ignored)
{
	sem_wait(&agentSem);
	sem_post(&tobaccoSem);
	sem_post(&matchSem);
}

void *
agentC(void *ignored)
{
	sem_wait(&agentSem);
	sem_post(&paperSem);
	sem_post(&matchSem);
}

void
smoke(void)
{
	printf("Smoker is smoking ....\n");
	sleep(1);
	printf("Smoker is done smoking....\n");
}

void *
smokerPaper(void *ignored)
{
	sem_wait(&tobaccoSem);
	sem_wait(&matchSem);
	smoke();
	sem_post(&agentSem);
}

void *
smokerTobacco(void *ignored)
{
	sem_wait(&paperSem);
	sem_wait(&matchSem);
	smoke();
	sem_post(&agentSem);
}

void *
smokerMatches(void *ignored)
{
	sem_wait(&tobaccoSem);
	sem_wait(&paperSem);
	smoke();
	sem_post(&agentSem);
}

#define NUM_AGENTS 3
#define NUM_SMOKERS 3

int
main(int argc, char **argv)
{
	sem_init(&agentSem, 0, 1);
	sem_init(&tobaccoSem, 0, 1);
	sem_init(&paperSem, 0, 1);
	sem_init(&matchSem, 0, 1);

	pthread_t agents[3];
	pthread_t smokers[3];

	void *(*agentFns[3]) (void *) = {agentA, agentB, agentC};
	void *(*smokerFns[3]) (void *) = {smokerTobacco, smokerMatches, smokerPaper};

	for (int i = 0; i < NUM_AGENTS; i++) {
		pthread_create(&agents[i], NULL, agentFns[i], NULL);
	}

	for (int i = 0; i < NUM_SMOKERS; i++) {
		pthread_create(&smokers[i], NULL, smokerFns[i], NULL);
	}

	for (int i = 0; i < NUM_AGENTS; i++) {
		pthread_join(agents[i], NULL);
	}

	for (int i = 0; i < NUM_SMOKERS; i++) {
		pthread_join(smokers[i], NULL);
	}
}
