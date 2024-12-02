
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>

#define SHOWER_TIME 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waiting = PTHREAD_COND_INITIALIZER;
bool in_shower = false;

int waiting_num = 0;

/*

  So these threads do their best thinking in the shower.

  Threads want to use the shower but only one thread can use the
  shower at once.  Once in the shower a thread showers for at least
  SHOWER_TIME.  Then the thread checks to see if any other threads
  want to use the shower, if yes the thread finishes in the shower and
  exits.

  If no other threads want to use the shower, the thread will stay in
  the shower and begin the process again.  This means that a thread
  will never leave the shower unless another thread arrives.

  Hint: this problem can actually be solved with just mutexes, but a
  cv solution is also good.

  Example output:

thread 1 wants to use the shower
thread 1 is using shower
thread 1 decides to stay in the shower a little longer
thread 3 wants to use the shower
thread 2 wants to use the shower
thread 4 wants to use the shower
thread 1 is finished with shower
thread 3 is using shower
thread 3 is finished with shower
thread 2 is using shower
thread 2 is finished with shower
thread 4 is using shower
thread 4 decides to stay in the shower a little longer
thread 4 decides to stay in the shower a little longer
<would run forever but main quits>

 */

void *thread(void *arg)
{
    int num = (int) arg; //ignore the sketchy way I'm passing a parameter here
    printf("thread %d wants to use the shower\n", num);
    // I reccommend you leave the above print statement outside of
    // any lock, otherwise a certian kind of bug will be less obvious

    pthread_mutex_lock(&mutex);
    waiting_num++;
    while(in_shower) {
        pthread_cond_wait(&waiting, &mutex);
    }
    waiting_num--;
    in_shower = true;
    pthread_mutex_unlock(&mutex);
    
    printf("thread %d is using shower\n", num);
    while(1) {

        sleep(SHOWER_TIME);
        pthread_mutex_lock(&mutex);
        if(waiting_num > 0) {
            printf("thread %d is finished with shower\n", num);

            in_shower = false;
            pthread_cond_signal(&waiting);
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        // I really ought to check to see if somebody else wants to
        // use the shower here, but that is up to you
        printf("thread %d decides to stay in the shower a little longer\n", num);
    
    }
}



int main()
{
    
    pthread_t tid[100];
    int i = 0;

    
    pthread_create(&tid[i++],NULL, thread, (void*) 1);
    sleep(3);
    pthread_create(&tid[i++],NULL, thread, (void*) 2);
    pthread_create(&tid[i++],NULL, thread, (void*) 3);
    pthread_create(&tid[i++],NULL, thread, (void*) 4);
    sleep(10);

    //note that we don't wait for the last thread, otherwise this
    //program would never be done
    for(int j = 0; j < i - 1; j++) {
        pthread_join(tid[j],NULL);
    }


    return 0;
}
