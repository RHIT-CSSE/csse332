/* Adapted from https://gist.github.com/rajabishek/6209a575f00b122fe490 */
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>


sem_t mutex,writeblock;
int data = 0,rcount = 0;

void *reader(void *arg)
{
    int f;
    f = *((int*)arg);
    while(1){
        sem_wait(&mutex);
        rcount = rcount + 1;
        if(rcount==1)
            sem_wait(&writeblock);
        sem_post(&mutex);
        printf("Data read by the reader%d is %d\n",f,data);
        sleep(1);
        sem_wait(&mutex);
        rcount = rcount - 1;
        if(rcount==0)
            sem_post(&writeblock);
        sem_post(&mutex);

        /* usleep(200); */
    }
}

void *writer(void *arg)
{
    int f;
    f = *((int*) arg);
    while(1){
        sem_wait(&writeblock);
        printf("Writer%d starts to write..\n",f);
        data++;
        sleep(1);
        sem_post(&writeblock);
        printf("Writer%d finishes to write %d\n",f,data);
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
