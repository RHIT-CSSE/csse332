
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

/*

In this system we have task threads which need to do numbered jobs.
Unfortunately we can't pass the job numbers as parameters to the
threads.

Instead, after the threads are created main will communicate the job
numbers by calling the special function assign_job.  It will never
assign more jobs than there are currently available task threads.

When the task threads finish starting up, they call a function called
get job which returns a job number that has been assigned.  It doesn't
matter which task thread gets the job, as long as each task thread
gets a unique assigned job.

The way assign_job/get job work is that that assign_job sets a global
called job_to_do.  get_job retrieves the value in that global and sets
job_to_do to 0, indicating that the job is taken (0 is never a valid
job number).

But as it stands the system is vulnerable because multiple tasks might
actidentally take the same job or multiple calls to assign_job might
overwrite the job_to_do before a task thread has taken it.

Here's the rules: 

1.  if get_job is called but no job is available, the function should
wait (i.e. not return) until a valid job is assigned

2.  if assign_job is called but the last assigned job has not yet been
taken, the function should wait till that previous job is taken before
putting the new job in the global

3.  Only one task thread should ever get a particular job 

4.  It should not be possible for a job to get missed (i.e. not
assigned to a task thread, even though there is a task thread
available to do it)

5.  Once a task thread finishes its job, it will exit

Solve this problem by modifying get_job and assign_job.  You should
not need to modify the thread functions or main.

As always, a solution that busy-waits is not correct.

Example output:

    task_thread_started
    task_thread_started
    task thread got job 99
    task thread got job 14
    task thread finished job 99
    task_thread_started
    task thread finished job 14
    task thread got job 77
    task_thread_started
    task thread got job 4
    task thread finished job 77
    task thread finished job 4


 */


int job_to_do = 0;

int get_job() {

    // you'll need to do some work to ensure this is safe
    int my_job = job_to_do;
    job_to_do = 0;
    return my_job;

}

void assign_job(int job_num) {

    // you'll need to do some work to ensure this is safe
    job_to_do = job_num;

}




void *task_thread(void *arg)
{

    // you shouldn't need to modify this function, just get_job and assign_job
    sleep(1); // leave this in here to catch a certian flavor of bug
    printf("task_thread_started\n");

    int my_job = get_job();
    printf("task thread got job %d\n", my_job);
    sleep(1);
    printf("task thread finished job %d\n", my_job);
}



int main()
{
    
    pthread_t tid[100];
    int i = 0;


    pthread_create(&tid[i++],NULL,task_thread,NULL);
    pthread_create(&tid[i++],NULL,task_thread,NULL);
    sleep(2);
    assign_job(99);
    assign_job(14);
    pthread_create(&tid[i++],NULL,task_thread,NULL);
    assign_job(77);
    pthread_create(&tid[i++],NULL,task_thread,NULL);
    assign_job(4);    

    for(int j = 0; j < i; j++) {
        pthread_join(tid[j],NULL);
    }


    return 0;
}
