---
layout: post
title: The Producer Consumer Problem
readtime: true
date: Tue Jan 17 07:20:09 2023 
---

# Learning Objectives

At the end of this lecture, you should be able to:

- Use mutex locks and condition variables to solve a famous concurrency problem.

---

# Topics

In this lecture, we will cover the following topics:

- Mutex locks.
- Condition variables.
- The producer consumer problem.

---

# Notes


<!-- vim-markdown-toc GFM -->

* [The producer-consumer problem](#the-producer-consumer-problem)
    * [Synchronization condition](#synchronization-condition)
* [Solution 1: Broken](#solution-1-broken)
  * [Problem with the solution?](#problem-with-the-solution)
* [Solution 2: Better and still broken](#solution-2-better-and-still-broken)
* [Solution 3: Now it’s correct](#solution-3-now-its-correct)
* [Generalization and activity](#generalization-and-activity)

<!-- vim-markdown-toc -->

# The producer-consumer problem

- In this problem, we have one or more producer threads and one or more consumer
  threads
- There is a shared buffer between them
- Producer threads put data into the buffer until it is full
- Consumer threads fetch data out of the buffer until it is empty
- Example: A multi-threaded web server
  - Another good example: pipes
    - Some processes will put things into the pipe
    - Some processes will read things from the pipe
    - The processes have different roles
- Since we have a buffer as a shared resource between producers and consumers,
  then we have a **race condition** that we must prevent
- Let’s take a look at the code, assume for now that there is a single value
  that we increment from producers and decrement from consumers

```c
int buffer;
int count = 0;

void put(int value) {
	assert(count == 0);
	count = 1;
	buffer = value;
}

int get() {
	assert(count == 1);
	count = 0;
	return buffer;
}
```

```c
int loops;

void *producer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		put(i);
	}
}

void *consumer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		get();
		/* ... */
	}
}
```

### Synchronization condition

- The consumer shall only read data from the buffer when the `count` is 1,
  otherwise sleep and wait until the buffer is full
- The producer shall only put data into the buffer when the `count` is 0,
  otherwise sleep and wait until the buffer is empty

# Solution 1: Broken

- So how do you think we are going to satisfy the synchronization conditions?
- Yes, let’s use a condition variable and a mutex
  - The mutex will take care of resolving the race condition
  - The condition variable will allow us to synchronize the production and
    consumption of the data in the buffer
    - Sleep until the world is a better place
    - Or actually, sleep until you receive a hint that the world is a better
      place

```c
int loops;
pthread_cond_t cond;
pthread_mutex_t mutex;

void *producer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		pthread_mutex_lock(&mutex);
		if(count == 1) {
			pthread_cond_wait(&cond, &mutex);
		}
		put(i);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}
```

```c
void *consumer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		pthread_mutex_lock(&mutex);
		if(count == 0)
			pthread_cond_wait(&cond, &mutex);
		printf("%d\n", get());
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}
```

## Problem with the solution?

- The problem here is very subtle but very hard to find.
- We have to think about the scheduler as our enemy, it will pick the worst time
  to schedule things so as it will break our code.
- Imagine we have one producer and two consumers, assume that the buffer is
  initially empty.
- The first consumer runs, finds that the buffer is empty and goes to sleep
  waiting on the condition variable
- The producer then runs, finds that the buffer is empty, puts a value in there
  and sends a signal on the condition variable
- Once the signal is received, the consumer thread is moved from the sleeping
  queue to the run queue, **but not run yet**
- Before the first thread gets to run, another consumer thread goes in quickly
  and consumes the data in the buffer
- The first consumer thread then runs and finds that the buffer is empty but it
  is in the reading code
- Why is this happening?
  - Because a `pthread_cond_signal` is a hint that the state of the world has
    changed
    - But it is no guarantee that the state of the world is going to still
      be changed when the thread wakes up
    - The world has changed as some point, but there’s no guarantee that it
      did not go back into the bad state.
    - This is referred to as the *Mesa* semantics.
- So what are we supposed to do?
  - Yes, we need to check again for the condition when we wake up from the
    consumer thread (and similarly from the producer thread)

# Solution 2: Better and still broken

- So to solve the above problem, we need to check again on the state of the
  world before we go into the critical section
- So how do we do that? Yup, by changing the `if` statement into a `while` loop.

```c
int loops;
pthread_cond_t cond;
pthread_mutex_t mutex;

void *producer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		pthread_mutex_lock(&mutex);
		while(count == 1) {
			pthread_cond_wait(&cond, &mutex);
		}
		put(i);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}
```

```c
void *consumer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		pthread_mutex_lock(&mutex);
		while(count == 0)
			pthread_cond_wait(&cond, &mutex);
		printf("%d\n", get());
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}
```

- Okay this looks better, but have we completely solved all of our problems?
- Hmm, let’s examine this further.
- Let’s assume we again have two consumers $T_1$ and $T_2$ and a producer $P$.
- First, the two consumer threads will run and both will go into sleep on the
  condition variable in a queue
- Good, now the producer runs, it checks that `count == 0` and goes into the
  critical section
- The producer calls `put` to generate a data value into the buffer and then
  wakes up one of the consumers (actually, the first one that came into the
  queue)
- One consumer then wakes up, consumes the value that was put into the buffer,
  and signals on the condition variable
- And wait for it! What happens now?
- Who do we really want to wake up?
  - The producer, right?!
- Who do we really end up waking up?
  - The other consumer, so what happens in that case?
- A deadlock is created, all three threads are waiting on each other for the
  condition variable but there’s no one to signal on it.
- This is really bad, so let’s think this through.

# Solution 3: Now it’s correct

- How can we solve the above problem? Hmm, let’s think about it.
- The problem was that when we signal, there are two types of threads sleeping
  on the condition variable, producers and consumers
    - So which one of them are we going to wake up?
- We need a way to differentiate between the sleeping threads
- Solution?
    - Use two condition variables
- The producers wait a `empty` condition variable
- The consumer wait on a `fill` condition variable

```c
int loops;
pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void *producer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		pthread_mutex_lock(&mutex);
		while(count == 1) {
			pthread_cond_wait(&empty, &mutex);
		}
		put(i);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
	}
}
```

```c
void *consumer(void *arg) {
	int i;
	for(i = 0; i < loops; i++) {
		pthread_mutex_lock(&mutex);
		while(count == 0)
			pthread_cond_wait(&fill, &mutex);
		printf("%d\n", get());
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
	}
}
```

# Generalization and activity

- This last solution is our correct solution, so now let’s generalize it to a
  bigger buffer
- Solve `prodcons.c` in the activities directory.

