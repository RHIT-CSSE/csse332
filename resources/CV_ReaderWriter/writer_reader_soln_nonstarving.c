/* Adapted from https://gist.github.com/rajabishek/6209a575f00b122fe490 */
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue = PTHREAD_COND_INITIALIZER;
int data = 0, running_w = 0, running_r = 0;
int waiting_w = 0;

void *reader(void *arg)
{
    int f;
    f = *((int*)arg);
    while(1){
        pthread_mutex_lock(&lock);
        while (running_w > 0 || waiting_w > 0) {
            pthread_cond_wait(&queue, &lock);
            // if (running_w > 0 || waiting_w > 0)
            //     pthread_cond_signal(&queue);
            if (running_w == 0)
                break;
        }
        running_r = running_r + 1;
        pthread_mutex_unlock(&lock);
        printf("Data read by the reader%d is %d\n",f,data);
        sleep(1);
        pthread_mutex_lock(&lock);
        running_r = running_r - 1;
        pthread_cond_signal(&queue);
        pthread_mutex_unlock(&lock);

        /* usleep(200); */
    }
}

void *writer(void *arg)
{
    int f;
    f = *((int*) arg);
    while(1){
        pthread_mutex_lock(&lock);
        waiting_w++;
        while (running_w > 0 || running_r > 0)
            pthread_cond_wait(&queue, &lock);
        waiting_w--;
        running_w = running_w + 1;
        pthread_mutex_unlock(&lock);
        printf("Writer%d starts to write..%d\n",f, running_w);
        data++;
        sleep(1);
        printf("Writer%d finishes to write %d\n",f,data);
        pthread_mutex_lock(&lock);
        running_w = running_w - 1;
        pthread_cond_signal(&queue);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

int main()
{
    int i,b; 
    pthread_t rtid[5],wtid[5];
    //create writer
    for(i=0;i<=1;i++)
        pthread_create(&wtid[i],NULL,writer,(void *)&i);
    //create readers
    for(i=0;i<=4;i++)
        pthread_create(&rtid[i],NULL,reader,(void *)&i);

    pthread_join(wtid[0],NULL);


    return 0;
}
