#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#define bool unsigned char
#define TRUE 1
#define FALSE 0

sem_t agentSem, tobaccoSem, paperSem, matchSem;
sem_t tobaccoSmoker, paperSmoker, matchSmoker;
sem_t mutex;
bool isTobacco, isPaper, isMatch;
const char *smokerNames[] = {"Tobacco Smoker", "Matches Smoker", "Paper Smoker"};

void *
agentA(void *ignored)
{
	while(1) {
		sem_wait(&agentSem);
		sem_post(&tobaccoSem);
		sem_post(&paperSem);
	}
	return NULL;
}

void *
agentB(void *ignored)
{
	while(1) {
		sem_wait(&agentSem);
		sem_post(&tobaccoSem);
		sem_post(&matchSem);
	}
	return NULL;
}

void *
agentC(void *ignored)
{
	while (1) {
		sem_wait(&agentSem);
		sem_post(&paperSem);
		sem_post(&matchSem);
	}
	return NULL;
}

void
smoke(int index)
{
	printf("Smoker %s is smoking ....\n", smokerNames[index]);
	sleep(1);
	printf("Smoker %s is done smoking....\n", smokerNames[index]);
}

void *
smokerPaper(void *arg)
{
	int index = *(int *)arg;
	while(1) {
		sem_wait(&paperSmoker);
		smoke(index);
		sem_post(&agentSem);
	}
	return NULL;
}

void *
smokerTobacco(void *arg)
{
	int index = *(int *)arg;
	while(1) {
		sem_wait(&tobaccoSmoker);
		smoke(index);
		sem_post(&agentSem);
	}
	return NULL;
}

void *
smokerMatches(void *arg)
{
	int index = *(int *)arg;
	while(1) {
		sem_wait(&matchSmoker);
		smoke(index);
		sem_post(&agentSem);
	}
	return NULL;
}

void *
tobaccoPusher(void *ignored)
{
	while (1) {
		sem_wait(&tobaccoSem);
		sem_wait(&mutex);
		if (isMatch)
		{
			isMatch = FALSE;
			// have tobacco and matches, wake up the paper smoker
			sem_post(&paperSmoker);
		}
		else if (isPaper)
		{
			isPaper = FALSE;
			// have tobacco and paper, wake up the match smoker
			sem_post(&matchSmoker);
		}
		else
		{
			// first one to wake up, simply say that we have tobacco now
			isTobacco = TRUE;
		}
		sem_post(&mutex);
	}
}

void *
paperPusher(void *ignored)
{
	while (1) {
		sem_wait(&paperSem);
		sem_wait(&mutex);
		if (isMatch)
		{
			isMatch = FALSE;
			// now we have matches and paper, so wake up tobacco smoker
			sem_post(&tobaccoSmoker);
		}
		else if (isTobacco)
		{
			isTobacco = FALSE;
			// now we have tobacco and paper, so wake up match smoker
			sem_post(&matchSmoker);
		}
		else
		{
			// first one to wake up, simply say that we have tobacco now
			isPaper = TRUE;
		}
		sem_post(&mutex);
	}
	return NULL;
}

void *
matchPusher(void *ignored)
{
	while (1) {
		sem_wait(&matchSem);
		sem_wait(&mutex);
		if (isTobacco)
		{
			isTobacco = FALSE;
			// now we have matches and tobacoo, wake up paper smoker
			sem_post(&paperSmoker);
		}
		else if (isPaper)
		{
			isPaper = FALSE;
			// now we have paper and matches, wake up tobacco smoker
			sem_post(&tobaccoSmoker);
		}
		else
		{
			// first one to wake up, simply say that we have matches now
			isMatch = TRUE;
		}
		sem_post(&mutex);
	}
	return NULL;
}

#define NUM_AGENTS 3
#define NUM_SMOKERS 3

int
main(int argc, char **argv)
{
	isTobacco = isPaper = isMatch = FALSE;

	sem_init(&agentSem, 0, 1);
	sem_init(&tobaccoSem, 0, 0);
	sem_init(&paperSem, 0, 0);
	sem_init(&matchSem, 0, 0);

	sem_init(&mutex, 0, 1);
	sem_init(&tobaccoSmoker, 0, 0);
	sem_init(&paperSmoker, 0, 0);
	sem_init(&matchSmoker, 0, 0);

	pthread_t agents[3];
	pthread_t smokers[3];
	pthread_t pushers[3];

	int smokersIndex[3] = {0, 1, 2};
	void *(*agentFns[3]) (void *) = {agentA, agentB, agentC};
	void *(*smokerFns[3]) (void *) = {smokerTobacco, smokerMatches, smokerPaper};
	void *(*pusherFns[3]) (void *) = {tobaccoPusher, matchPusher, paperPusher};

	for (int i = 0; i < NUM_AGENTS; i++) {
		pthread_create(&agents[i], NULL, agentFns[i], NULL);
	}

	for (int i = 0; i < NUM_SMOKERS; i++) {
		pthread_create(&smokers[i], NULL, smokerFns[i], &smokersIndex[i]);
	}

	for (int i = 0; i < NUM_AGENTS; i++) {
		pthread_create(&pushers[i], NULL, pusherFns[i], NULL);
	}

	for (int i = 0; i < NUM_AGENTS; i++) {
		pthread_join(agents[i], NULL);
	}

	for (int i = 0; i < NUM_SMOKERS; i++) {
		pthread_join(smokers[i], NULL);
	}

	for (int i = 0; i < NUM_AGENTS; i++) {
		pthread_join(pushers[i], NULL);
	}
}
