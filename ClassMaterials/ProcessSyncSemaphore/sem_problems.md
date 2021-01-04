
# Part A

Imagine these two thread functions run in parallel.  For various important reasons, t1 needs to do steps 1 and 3, and t2 needs to do step 2.  However, we need to ensure the steps always occur in order Step 1 - 3.  Add semaphores to the system to ensure the correct order:

    void* t1(void* ignored) {
        printf("step 1\n");
        printf("step 3\n");
    }
    
    void* t2(void* ignored) {
        printf("step 2\n");
    }


# Part B

Imagine there an is a train junction, where a north south track
intersects an east west track.  Here are the rules:

i.	You can assume, due to other protections in the system, that there
    is only 1 north/south thread and one east/west thread running at
    one time.  After one north/south train passes through the
    junction, though, another may be started and wish to pass though,
    etc.

ii.	Trains are long, and they take a while to move through the
    junction.  While a north/south train is in the junction, an east
    west train must not enter the junction.  The rule is the same for
    east/west trains. 
    
Your goal in this part is not to solve the problem.  Instead, your
goal is to look at the various code solutions below and explain
why they do not work.  Assume all variables are global, assume all
semaphores are initialized to 1.

1.

    while(train_in_junction == 1);
    train_in_junction = 1;
    enter_junction();
    train_in_junction = 0;

2.
    
    // code for north south trains
    sem_wait(&ns_sem);
    enter_junction();
    sem_post(&ns_sem);
    // code for east west trains in the same, but they have their 
    // own ew_sem semaphore

3.

    while(1) {
        sem_wait(&sem);
        if(train_in_junction == 0)
            sem_post(&sem);
            break;
        sem_post(&sem);
    }
    sem_wait(&sem)
    train_in_junction = 1;
    sem_post(&sem);
    enter_junction();
    sem_wait(&sem)
    train_in_junction = 0;
    sem_post(&sem);

4.

    sem_wait(&sem)
    while(train_in_junction == 1);
    train_in_junction = 1;
    sem_post(&sem);
    enter_junction();
    sem_wait(&sem)
    train_in_junction = 0;
    sem_post(&sem);

