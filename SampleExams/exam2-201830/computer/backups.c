/* Copyright 2018 Rose-Hulman */
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


void copyFile(char* filename, int user_id, int from, int to) {
    // you shouldn't need to modify this function
    printf("Starting %s copy from BU%d to BU%d (user %d)\n", filename, from, to, user_id);
    sleep(1);
    printf("Ending   %s copy from BU%d to BU%d (user %d)\n", filename, from, to, user_id);
}

void backupFile(char* filename, int user_id) {

    // you can modify this function as you need to, including changing
    // its parameters and return types
    copyFile(filename, user_id, 1, 2);
    copyFile(filename, user_id, 2, 3);
    copyFile(filename, user_id, 3, 4);
    copyFile(filename, user_id, 4, 5);

}

int main(int argc, char **argv) {

    // you'll convert these serialized calls to threads in STEP 1
    backupFile("aaa.txt",1);
    backupFile("bbb.txt",77);
    backupFile("ccc.txt",999);
    
    printf("Everything finished\n");

}
