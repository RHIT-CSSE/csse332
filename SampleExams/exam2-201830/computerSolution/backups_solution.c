/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/**
   In this system, we have a series of 5 backup servers (BU1-BU5).

   A file to be backed up starts on BU1, then gets transferred from
   BU1 to BU2, then gets tranferred from BU2 to BU3, etc.  Each backup
   server can only talk to the server before and after it in the
   sequence.  To be fully backed up a file must be copied all the way
   to BU5.

   Compile this code like this:

      gcc -pthread backups.c -o backups 

   STEP 1 (8 points):

   Make files copy in parallel using threads.

   To do this you can add new structs, modify the parameters or return
   values of functions, etc.

   Modify main to show your code works with the 3 input examples we
   have given.

   NOTE: To do this correctly you'll have to figure out how to pass
   the two parameters backupFile needs to the threads.  If you can't
   figure out how to do that, you can attempt to solve the later parts
   of the problem by just hardcoding "" as the filename and 0 as the
   userid.  You'll lose most of the points for this part if you have
   resorted to hardcoding values though.

   Example output:

      Starting aaa.txt copy from BU1 to BU2 (user 1)
      Starting ccc.txt copy from BU1 to BU2 (user 999)
      Starting bbb.txt copy from BU1 to BU2 (user 77)
      Ending   bbb.txt copy from BU1 to BU2 (user 77)
      Ending   ccc.txt copy from BU1 to BU2 (user 999)
      Ending   aaa.txt copy from BU1 to BU2 (user 1)
      Starting bbb.txt copy from BU2 to BU3 (user 77)
      Starting aaa.txt copy from BU2 to BU3 (user 1)
      Starting ccc.txt copy from BU2 to BU3 (user 999)
      Ending   bbb.txt copy from BU2 to BU3 (user 77)
      Starting bbb.txt copy from BU3 to BU4 (user 77)
      Ending   ccc.txt copy from BU2 to BU3 (user 999)
      Starting ccc.txt copy from BU3 to BU4 (user 999)
      Ending   aaa.txt copy from BU2 to BU3 (user 1)
      Starting aaa.txt copy from BU3 to BU4 (user 1)
      Ending   bbb.txt copy from BU3 to BU4 (user 77)
      Starting bbb.txt copy from BU4 to BU5 (user 77)
      Ending   ccc.txt copy from BU3 to BU4 (user 999)
      Starting ccc.txt copy from BU4 to BU5 (user 999)
      Ending   aaa.txt copy from BU3 to BU4 (user 1)
      Starting aaa.txt copy from BU4 to BU5 (user 1)
      Ending   bbb.txt copy from BU4 to BU5 (user 77)
      Ending   ccc.txt copy from BU4 to BU5 (user 999)
      Ending   aaa.txt copy from BU4 to BU5 (user 1)

   STEP 2 (8 points)
   
   Now we want to add a restriction: while a file is being tranferred,
   both servers are fully committed to the operation and can't do
   anything else.

   So a file cannot be copied from BU1 to BU2 at the same time a file
   is being copied from BU2 to BU3 (BU2 can't be in 2 transfers at
   the same time).  

   Once a backup thread has "locked" a server, it should not
   relinquish the server until it has completed everything it needs
   with that server (eg. a single backup should copy B1 -> B2 and B2
   -> B3 beforing allowing another thread to access B2).

   Files can be copied in parallel if the copies don't share any
   servers.  So a file can be copied from BU1 to BU2 at the same time
   that a file is copied from BU3 to BU4.

   Use semaphores to enforce this restriction.

   Example output:

      Starting aaa.txt copy from BU1 to BU2 (user 1)
      Ending   aaa.txt copy from BU1 to BU2 (user 1)
      Starting aaa.txt copy from BU2 to BU3 (user 1)
      Ending   aaa.txt copy from BU2 to BU3 (user 1)
      Starting aaa.txt copy from BU3 to BU4 (user 1)
      Starting bbb.txt copy from BU1 to BU2 (user 77)
      Ending   bbb.txt copy from BU1 to BU2 (user 77)
      Ending   aaa.txt copy from BU3 to BU4 (user 1)
      Starting aaa.txt copy from BU4 to BU5 (user 1)
      Starting bbb.txt copy from BU2 to BU3 (user 77)
      Ending   aaa.txt copy from BU4 to BU5 (user 1)
      Ending   bbb.txt copy from BU2 to BU3 (user 77)
      Starting bbb.txt copy from BU3 to BU4 (user 77)
      Starting ccc.txt copy from BU1 to BU2 (user 999)
      Ending   bbb.txt copy from BU3 to BU4 (user 77)
      Starting bbb.txt copy from BU4 to BU5 (user 77)
      Ending   ccc.txt copy from BU1 to BU2 (user 999)
      Starting ccc.txt copy from BU2 to BU3 (user 999)
      Ending   bbb.txt copy from BU4 to BU5 (user 77)
      Ending   ccc.txt copy from BU2 to BU3 (user 999)
      Starting ccc.txt copy from BU3 to BU4 (user 999)
      Ending   ccc.txt copy from BU3 to BU4 (user 999)
      Starting ccc.txt copy from BU4 to BU5 (user 999)
      Ending   ccc.txt copy from BU4 to BU5 (user 999)


   STEP 3 (4 points)
   
   Note that this question is worth less points than the others, and
   is a bit on the difficult side.  You might want to make sure you're
   getting most of the credit on the other parts of the exam before
   attempting this one.

   We want to have the possibility of backup restore.  A restore
   should work like a backup except in reverse: B5->B4->B3->B2->B1.
   Just like a backup, a restore uses all system resources on both
   servers while in progress - so a server currently in the process of
   a restore transfer can't be involved in another simultaneous
   restore OR backup).

   Restores do NOT have the same restriction as backups with regard to
   locking a server until the transfer is fully complete.  That is, a
   restore can transfer from B5->B4, then allow a backup thread to
   access B4 before tranferring B4->B3.  It might be wise to meditate
   a bit about why this is important.

   However, we do want to ensure that multiple restores due not pile
   up on a single server.  Therefore, if a restore has been
   transferred to an intermediate server (by intermediate I mean
   B2, B3, B4), another restore should not be allowed to start transferring
   to that server until that restore has moved on to the next server.

   Use semaphores to enforce this restriction.

   Add 2 restore threads to your main to show that your code works.
   
   Sample output:

      Starting aaa.txt copy from BU1 to BU2 (user 1)
      Starting RE1.txt copy from BU5 to BU4 (user 111)
      Ending   aaa.txt copy from BU1 to BU2 (user 1)
      Starting aaa.txt copy from BU2 to BU3 (user 1)
      Ending   RE1.txt copy from BU5 to BU4 (user 111)
      Ending   aaa.txt copy from BU2 to BU3 (user 1)
      Starting aaa.txt copy from BU3 to BU4 (user 1)
      Starting bbb.txt copy from BU1 to BU2 (user 77)
      Ending   aaa.txt copy from BU3 to BU4 (user 1)
      Starting aaa.txt copy from BU4 to BU5 (user 1)
      Ending   bbb.txt copy from BU1 to BU2 (user 77)
      Ending   aaa.txt copy from BU4 to BU5 (user 1)
      Starting RE1.txt copy from BU4 to BU3 (user 111)
      Ending   RE1.txt copy from BU4 to BU3 (user 111)
      Starting bbb.txt copy from BU2 to BU3 (user 77)
      Starting RE2.txt copy from BU5 to BU4 (user 222)
      Ending   bbb.txt copy from BU2 to BU3 (user 77)
      Ending   RE2.txt copy from BU5 to BU4 (user 222)
      Starting ccc.txt copy from BU1 to BU2 (user 999)
      Starting bbb.txt copy from BU3 to BU4 (user 77)
      Ending   bbb.txt copy from BU3 to BU4 (user 77)
      Starting bbb.txt copy from BU4 to BU5 (user 77)
      Ending   ccc.txt copy from BU1 to BU2 (user 999)
      Starting ccc.txt copy from BU2 to BU3 (user 999)
      Ending   ccc.txt copy from BU2 to BU3 (user 999)
      Ending   bbb.txt copy from BU4 to BU5 (user 77)
      Starting ccc.txt copy from BU3 to BU4 (user 999)
      Ending   ccc.txt copy from BU3 to BU4 (user 999)
      Starting ccc.txt copy from BU4 to BU5 (user 999)
      Starting RE1.txt copy from BU3 to BU2 (user 111)
      Ending   ccc.txt copy from BU4 to BU5 (user 999)
      Ending   RE1.txt copy from BU3 to BU2 (user 111)
      Starting RE1.txt copy from BU2 to BU1 (user 111)
      Starting RE2.txt copy from BU4 to BU3 (user 222)
      Ending   RE2.txt copy from BU4 to BU3 (user 222)
      Ending   RE1.txt copy from BU2 to BU1 (user 111)
      Starting RE2.txt copy from BU3 to BU2 (user 222)
      Ending   RE2.txt copy from BU3 to BU2 (user 222)
      Starting RE2.txt copy from BU2 to BU1 (user 222)
      Ending   RE2.txt copy from BU2 to BU1 (user 222)
      
**/

sem_t semaphore[5];
sem_t restore_sem[3];

struct params {
    char* name;
    int user;
};

void copyFile(char* filename, int user_id, int from, int to) {
    // you shouldn't need to modify this function
    printf("Starting %s copy from BU%d to BU%d (user %d)\n", filename, from, to, user_id);
    sleep(1);
    printf("Ending   %s copy from BU%d to BU%d (user %d)\n", filename, from, to, user_id);
}

void backupFile(char* filename, int user_id) {

    sem_wait(&semaphore[0]);
    sem_wait(&semaphore[1]);
    copyFile(filename, user_id, 1, 2);
    sem_post(&semaphore[0]);
    sem_wait(&semaphore[2]);
    copyFile(filename, user_id, 2, 3);
    sem_post(&semaphore[1]);
    sem_wait(&semaphore[3]);    
    copyFile(filename, user_id, 3, 4);
    sem_post(&semaphore[2]);
    sem_wait(&semaphore[4]);        
    copyFile(filename, user_id, 4, 5);
    sem_post(&semaphore[3]);
    sem_post(&semaphore[4]);
}


void restoreFile(char* filename, int user_id) {

    sem_wait(&restore_sem[0]); // get sem for 4

    sem_wait(&semaphore[3]);
    sem_wait(&semaphore[4]);
    copyFile(filename, user_id, 5, 4);
    sem_post(&semaphore[3]);
    sem_post(&semaphore[4]);

    sem_wait(&restore_sem[1]); // get sem for 3
    sem_wait(&semaphore[2]);
    sem_wait(&semaphore[3]);
    copyFile(filename, user_id, 4, 3);
    sem_post(&semaphore[2]);
    sem_post(&semaphore[3]);

    sem_post(&restore_sem[0]); // free sem for 4
    sem_wait(&restore_sem[2]); // get sem for 2
    
    sem_wait(&semaphore[1]);
    sem_wait(&semaphore[2]);    
    copyFile(filename, user_id, 3, 2);
    sem_post(&semaphore[1]);
    sem_post(&semaphore[2]);

    sem_post(&restore_sem[1]); // free sem for 3
    
    sem_wait(&semaphore[0]);
    sem_wait(&semaphore[1]);    
    copyFile(filename, user_id, 2, 1);
    sem_post(&semaphore[0]);
    sem_post(&semaphore[1]);
    sem_post(&restore_sem[2]); // free sem for 2

}


void* startThread(void* param) {
    struct params *data = param;
    backupFile(data->name, data->user);
    return NULL;
}

void* startThread2(void* param) {
    struct params *data = param;
    restoreFile(data->name, data->user);
    return NULL;
}



int main(int argc, char **argv) {

    pthread_t x, y, z, v, w;
    struct params a,b,c,d,e;
    a.name = "aaa.txt";
    a.user = 1;

    b.name = "bbb.txt";
    b.user = 77;

    c.name = "ccc.txt";
    c.user = 999;

    d.name = "RE1.txt";
    d.user = 111;

    e.name = "RE2.txt";
    e.user = 222;

    
    for(int i = 0; i < 5; i++) {
        sem_init(&semaphore[i], 0, 1);
    }


    for(int i = 0; i < 3; i++) {
        sem_init(&restore_sem[i], 0, 1);
    }

    
    pthread_create(&x, NULL, startThread, &a);
    pthread_create(&y, NULL, startThread, &b);
    pthread_create(&z, NULL, startThread, &c);
    pthread_create(&w, NULL, startThread2, &d);
    pthread_create(&v, NULL, startThread2, &e);

    pthread_join(x, NULL);
    pthread_join(y, NULL); 
    pthread_join(z, NULL);
    pthread_join(v, NULL);
    pthread_join(w, NULL); 

    
    for(int i = 0; i < 5; i++) {
        sem_destroy(&semaphore[i]);
    }


    for(int i = 0; i < 3; i++) {
        sem_destroy(&restore_sem[i]);
    }

    
    

}
