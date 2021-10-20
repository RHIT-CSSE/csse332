/*
   So in this problem you must implement 6 seperate functions. The
   first 4 are pretty easy to understand:
   void lockA();
   void lockB();
   void unlockA();
   void unlockB();
   Basically A and B can be thought of as two independent mutexes.  If
   you call lockA() and some other thread has already locked A, lockA
   will be stuck and will not return until the thread with A locked
   calls unlockA().  lockB and unlockB act the same way.  The lock
   state of B has no effect on A and vice versa.
   I say A and B can be "thought of as two independent mutexes" rather
   than "are 2 mutexes" because you can implement these functions in
   any way you wish, as long as you provide the normal guarentees.  So
   you can have 1 underlying mutex and just some condition variables -
   or whatever.
   You can also assume that no single thread will ever attempt to
   aquire both "mutexes" at once:
   lockA();
   lockB(); // undefined behavior!  Not allowed
   If you wish, you can implement your lockA/lockB to error if someone
   attempts this, but you can also just assume that this rule will not
   be broken.  Note that this rule applies to callers of your
   lock/unlock functions - you can do whatever you wish, so long as you
   behave correctly whenever the lock/unlock functions are called in a
   legal way.
   The two other functions you must implement are:
   swapAB();
   swapBA();
   So the swap function allows the caller to exchange one "mutex" for
   another.  For example, you call swapAB() when your thread has mutexA
   and when the function returns your thread will have released A but
   will have aquired B.  The key requirement of the swapAB is this:
   there should never be a time when the caller of swapAB has already
   released A but has not yet aquired B.  Put another way, if B is not
   available the function will wait for B holding A and not "swap"
   until it is guarenteed to get B.
   It is possible that 2 threads both want to swap (e.g. thread 1 is
   trying to swapAB and is waiting because B is claimed, but then
   thread 2 which has B executes a swapBA).  In that case the swap
   should happen conceptually "at the same time": both the swap
   functions can return, and neither of the mutexes should ever be in
   an available state where they could be stolen by a thread attempting
   a lockA or lockB.

   One thing to note and always remember is that the thread the locks a mutex lock
   should be the one that unlocks it. Attempting to unlock another thread's mutex
   lock results in undefined behavior. This problem can be circumvented by using a
   semaphore, which you are allowed to use in this problem.
   */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

int swap_mode = 0;

void lockA() {
}

void lockB() {
}

void unlockA() {
}

void unlockB() {
}

void swapAB() {
}

void swapBA() {
}

void assert_i_have_a() {
	static int a_count = 0;
	// my use of a_count in this function could
	// theoretically be considered unprotected
	// but only if more than 1 thread has a
	if(a_count != 0) {
		printf("ERROR multiple threads seem to have gotten A\n");
	}

	a_count++;
	usleep(20);
	if(a_count != 1) {
		printf("ERROR multiple threads seem to have gotten A\n");
	}
	a_count--;
}

void assert_i_have_b() {
	static int b_count = 0;
	if(b_count != 0) {
		printf("ERROR multiple threads seem to have gotten B\n");
	}

	b_count++;
	usleep(5);
	if(b_count != 1) {
		printf("ERROR multiple threads seem to have gotten B\n");
	}
	b_count--;
}


void swapper() {
	for(int i = 0; i < 10; i++) {
		lockA();
		assert_i_have_a();
		unlockA();
		lockB();
		assert_i_have_b();
		unlockB();
		lockA();
		assert_i_have_a();
		swapAB();
		assert_i_have_b();
		swapBA();
		assert_i_have_a();
		unlockA();
	}
}

int main(int argc, char **argv) {
	pthread_t threads[10];
	for(int i = 0; i < 10; i++) {
		pthread_create(&threads[i], NULL, (void * (*)(void *)) swapper, NULL);
	}
	for(int i = 0; i < 10; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything done.\n");
}
