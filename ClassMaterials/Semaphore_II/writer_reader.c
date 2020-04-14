/* Adapted from https://gist.github.com/rajabishek/6209a575f00b122fe490 */
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>


int data = 0;
sem_t writeblock, mutex;
int readers_in_cs = 0;

void *reader(void *arg)
{
    int f;
    f = *((int*)arg);
    while(1){

        sem_wait(&mutex)
        if(readers_in_cs == 0) {
            sem_wait(&writeblock);
        }
        readers_in_cs++;
        sem_post(&mutex);
        
        //critical section
        printf("Data read by the reader%d is %d\n",f,data);
        sleep(1);
        //critical section
        sem_post(&writeblock);

        sem_wait(&mutex);
        readers_in_cs--;
        if(readers_in_cs == 0) {
            sem_post(&writeblock);
        }
        sem_post(&mutex);

        
        usleep(200);
    }
}

void *writer(void *arg)
{
    int f;
    f = *((int*) arg);
    while(1){

        sem_wait(&writeblock);
        //critical section
        printf("Writer%d starts to write..\n",f);
        data++;
        sleep(1);
        printf("Writer%d finishes to write %d\n",f,data);
        //critical section
        sem_post(&writeblock);
        
        sleep(1);
    }
}

int main()
{
    int i,b; 
    pthread_t rtid[5],wtid[5];
    sem_init(&mutex,0,1);
    sem_init(&writeblock,0,1);
    //create writer
    for(i=0;i<=1;i++)
        pthread_create(&wtid[i],NULL,writer,(void *)&i);
    //create readers
    for(i=0;i<=4;i++)
        pthread_create(&rtid[i],NULL,reader,(void *)&i);

    pthread_join(wtid[0],NULL);


    return 0;
}
