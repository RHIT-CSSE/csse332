
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_NUM 50
#define NUM_THREADS 3

/*

This is an implementation of the famous sieve of eratosthenes
algorithm which finds (small) prime numbers.

The idea is pretty simple - we have the nums array which represents
all the numbers we are going to consider up to our max.  Initially we
mark every element with 'P' because it could potentially be prime.  We
could start with only the knowledge that 0 1 are not prime and that 2
is prime, but to simplify lets assume that we know that 2 3 and 5 are
prime.

When we know a number is prime we iterate across the list of numbers,
starting from a particular prime and marking all its multiples.  So
since I know 5 is prime I'll mark 10, 15, 20, etc. as composite ('C'
in the nums array) - up to our max number.  This is what the sieve_for
function does.

PART 1 (25 points)

Right now we run sieve_for for 2 3 and 5 serially, but it would be
better if we ran them in paralell on seperate threads.

Modify sieve_for and use pthreads to start the 3 sieves in paralell.
Your output should look something like this:

    starting sieve for 2
    starting sieve for 3
    starting sieve for 5
    sieve 2 found composite 4
    sieve 5 found composite 10
    sieve 3 found composite 6
    sieve 5 found composite 15
    sieve 3 found composite 9
    sieve 2 found composite 8
    sieve 5 found composite 20
    sieve 3 found composite 12
    sieve 5 found composite 25
    sieve 5 found composite 30
    sieve 3 found composite 18
    sieve 2 found composite 14
    sieve 5 found composite 35
    sieve 3 found composite 21
    sieve 2 found composite 16
    sieve 5 found composite 40
    sieve 3 found composite 24
    sieve 5 found composite 45
    sieve for 5 finished
    sieve 3 found composite 27
    sieve 2 found composite 22
    sieve 3 found composite 33
    sieve 3 found composite 36
    sieve 2 found composite 26
    sieve 3 found composite 39
    sieve 2 found composite 28
    sieve 3 found composite 42
    sieve 2 found composite 32
    sieve 3 found composite 48
    sieve for 3 finished
    sieve 2 found composite 34
    sieve 2 found composite 38
    sieve 2 found composite 44
    sieve 2 found composite 46
    sieve for 2 finished
    primes are 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 49 

PART 2 (15 points)

This part is a little harder for the 15 points you get.

You can see the output for PART 1 is not right.  It thinks 49 is
prime.

The reason for this is after we identify another value as prime, we
also need to start a sieve for that number.  So after one of our sieve
threads completes, we should start a new sieve thread for the next
number > 5 we think is prime (7).  Once our second thread finishes, we
should start a sieve for the next prime that remains after 7, etc.

To make things simple, we will run our 3 threads in batches, i.e., we start the
first three threads for 2,3, and 5. Then we wait for them to finish before we
launch the next batch of three threads for 7, 11, and 13. And so on after that
in batches of three threads until we've started sieves for all primes less than
MAX_NUM (we could actually stop a little sooner than that, but let's not be fancy).
It should look like this:

    starting sieve for 2
    starting sieve for 3
    starting sieve for 5
    sieve 2 found composite 4
    sieve 3 found composite 6
    sieve 5 found composite 10
    sieve 3 found composite 9
    sieve 5 found composite 15
    sieve 2 found composite 8
    sieve 3 found composite 12
    sieve 5 found composite 20
    sieve 5 found composite 25
    sieve 3 found composite 18
    sieve 5 found composite 30
    sieve 2 found composite 14
    sieve 3 found composite 21
    sieve 5 found composite 35
    sieve 2 found composite 16
    sieve 3 found composite 24
    sieve 5 found composite 40
    sieve 3 found composite 27
    sieve 5 found composite 45
    sieve for 5 finished
    sieve 2 found composite 22
    sieve 3 found composite 33
    sieve 3 found composite 36
    sieve 2 found composite 26
    sieve 3 found composite 39
    sieve 2 found composite 28
    sieve 3 found composite 42
    sieve 2 found composite 32
    sieve 3 found composite 48
    sieve for 3 finished
    sieve 2 found composite 34
    sieve 2 found composite 38
    sieve 2 found composite 44
    sieve 2 found composite 46
    sieve for 2 finished
    starting sieve for 7
    starting sieve for 13
    starting sieve for 11
    sieve for 13 finished
    sieve for 11 finished
    sieve 7 found composite 49
    sieve for 7 finished
    starting sieve for 17
    starting sieve for 19
    starting sieve for 23
    sieve for 17 finished
    sieve for 23 finished
    sieve for 19 finished
    starting sieve for 29
    sieve for 29 finished
    starting sieve for 31
    sieve for 31 finished
    starting sieve for 37
    sieve for 37 finished
    starting sieve for 41
    sieve for 41 finished
    starting sieve for 43
    sieve for 43 finished
    starting sieve for 47
    sieve for 47 finished
    primes are 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 

The fanciest solutions involve reusing the pthread_ts for our threads
because we know we will only have NUM_THREADS parallel threads at
once.  But that approach can be a little tricky.

Easier is probably to allocate a thread_t for each entry in nums, even
though most of them will never actually be used.  You can also make a
special marking in the nums array (e.g. have 'P' for prime but we
haven't sieved it, and 'S' for prime but the sieve has been created).

You may also notice an unlikely bug - what would happen if the sieves
for 2 and 5 ran so fast and the sieve for 3 ran so slow that by the
time we need to start new threads 9 still looked prime?  Don't worry
about this bug.

 */

char nums[MAX_NUM];

void* sieve_for(long num) {

    printf("starting sieve for %ld\n", num);
    long current = num + num;
    while(current < MAX_NUM) {
        usleep(100); // a little wait to make the parallelism easier to notice
        if(nums[current] != 'C') printf("sieve %ld found composite %ld\n", num, current);
        nums[current] = 'C'; // C for composite
        current = current + num;
    }
    printf("sieve for %ld finished\n", num);
    return NULL;
}

int main(int argc, char **argv)
{
  /* TODO: YOUR IMPLEMENTATION GOES HERE */
  int i;
  pthread_t threads[NUM_THREADS];
  long params[] = {2, 3, 5};

  for(int i = 2; i < MAX_NUM; i++) {
      nums[i] = 'P'; //mark all numbers as potentially prime
  }


  // these are put in parallel in PART 1
  sieve_for(2);
  sieve_for(3);
  sieve_for(5);

  // here's where you add your loop that starts batches of threads once other
  // threads finish when you do part 2

  printf("primes are ");
  for(long i = 2; i < MAX_NUM; i++) {
      if(nums[i] == 'P') printf("%ld ", i);
  }
  printf("\n");
}
