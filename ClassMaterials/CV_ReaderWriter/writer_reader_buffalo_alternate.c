/* Adapted from https://gist.github.com/rajabishek/6209a575f00b122fe490 */
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

/*

This is a solution I built mostly in class so that students could
watch me struggle.

It attempts to prevent any type of starvation by forcing the
readers/writers to alternate.

The basic idea is that there is a "batch" of readers who cannot
immediately read.  Each reader is given a unique number that orders
them.  When a writer finishes writing, it updates the batch cutoff to
the most recently added reader and another writer will not execute
until readers through that batch cutoff have finished reading.

As far as I have been able to test, I think this solution does it
(although I wouldn't be super surprised if a bug had slipped in).

This required a more advanced approach than I'd ask you to do on exam.
  
 */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readers = PTHREAD_COND_INITIALIZER;
pthread_cond_t writers = PTHREAD_COND_INITIALIZER;

int last_reader_num = 0;
int finished_readers = 0;
int batch_cutoff = 0;

int writers_waiting = 0;
int writers_in_cs = 0;

int data = 0;

void *reader(void *arg)
{
    int f;
    f = *((int*)arg);
    while(1){

        pthread_mutex_lock(&mutex);
        int my_reader_num = ++last_reader_num;
        printf("reader%d wants to read\n",f);

        if(writers_waiting == 0 && !writers_in_cs) {
            // if there's no writers at all, just
            // add this entry to the last batch
            // and go right to the critical section

            // if there is a bug in this solution my guess would be
            // its with this perhaps not fully considered part
            batch_cutoff = my_reader_num;
        } else {
            while(batch_cutoff < my_reader_num) {
                pthread_cond_wait(&readers, &mutex);
            }
        }
                
        pthread_mutex_unlock(&mutex);
        //critical section
        printf("Data read by the reader%d is %d\n",f,data);
        sleep(1);
        //critical section
        pthread_mutex_lock(&mutex);
        finished_readers++;
        if(finished_readers == batch_cutoff) {
            pthread_cond_signal(&writers);
        }
        pthread_mutex_unlock(&mutex);

        sleep(f);
    }
}

void *writer(void *arg)
{
    int f;
    f = *((int*) arg);
    while(1){

        pthread_mutex_lock(&mutex);
        printf("Writer%d wants to write\n",f);

        writers_waiting++;
        while(writers_in_cs > 0 || finished_readers != batch_cutoff) {
            pthread_cond_wait(&writers, &mutex);
        }
        writers_waiting--;
        writers_in_cs++;
        pthread_mutex_unlock(&mutex);
        
        //critical section
        printf("Writer%d starts to write..\n",f);
        data++;
        sleep(1);
        printf("Writer%d finishes to write %d\n",f,data);
        //critical section
        pthread_mutex_lock(&mutex);
        writers_in_cs--;
        batch_cutoff = last_reader_num;
        if(finished_readers == batch_cutoff) {
            // there are no readers, so another writer might as well
            // go forward
            pthread_cond_signal(&writers);
        } else {
            pthread_cond_broadcast(&readers);
        }
        pthread_mutex_unlock(&mutex);
        
        //sleep(1);
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
