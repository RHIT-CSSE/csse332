---
title: Condition Variables Homework II
layout: post
date: Tuesday October 19
readtime: true
gh-repo: rhit-csse332/csse332
gh-badge: [star,watch,follow]
---

# Introduction

In this assignment, we will have further practice with condition variables and
mutex locks. There are five problems, you are only required to solve __four__ of
these five problems.
## Band

This problem refers to the code in the file `band.c`. Imagine a group of friends
are getting together to play music. They are, however, arriving at different
times; arriving can happen at any time (e.g., when some other friends are
playing). They share a single stage on which at most one band can play at a
time.

There are three different kinds of friends: drummers, singers, and guitarists,
and the follow the following set of rules:
1. It takes one of each kind to make a band.
1. Only 1 band can be playing at once.
1. Once conditions (1) and (2) are met, the players can start playing and stop
   playing in any order.
1. All 3 players must stop playing before a new set of 3 can start playing.

### Example output

```
drummer arrived
drummer arrived
guitarist arrived
guitarist arrived
singer arrived
drummer playing
guitarist playing
singer playing
singer arrived
singer arrived
drummer arrived
guitarist arrived
drummer finished playing
guitarist finished playing
singer finished playing
singer playing
guitarist playing
drummer playing
singer finished playing
guitarist finished playing
drummer finished playing
guitarist playing
drummer playing
singer playing
guitarist finished playing
drummer finished playing
singer finished playing
Everything finished.
```

## Little Red Hen

This problem refers to the code in `littleredhen.c`. This system has four
threads: (1) the duck, (2) the cat, (3) the dog, and (4) little red hen. The
duck, the cat, and the dog eat bread, while the little red hen is the one that
makes the bread.

The little red hen makes seven loaves per batch, but she only has the patience
for six batches. The little red hen will only make a batch if there are no
loaves left.

The other three animals (the duck, the cat, and the dog) each want to eat 14
loaves of bread, but only one of them can be in the kitchen at a time (to avoid
fights over who gets what bread).

When the duck, cat, or dog notice that there are no loaves of bread available,
they complain to the little red hen and wait (__in the kitchen__) for the next batch
to be ready.

Use condition variables and mutex locks to enforce the above constraints. Note
that the global `numLoaves` variable should be left as is (i.e., you cannot make
it a semaphore or a condition variable).

## Sample output

Below is a sample output from running a trace that satisfies the above
constraints.

```
Sleepy Cat          : Hey, Little Red Hen, make some more bread!
Little Red Hen      : A fresh batch of bread is ready.
Sleepy Cat          : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Hey, Little Red Hen, make some more bread!
Little Red Hen      : A fresh batch of bread is ready.
Lazy Dog            : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Hey, Little Red Hen, make some more bread!
Little Red Hen      : A fresh batch of bread is ready.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Hey, Little Red Hen, make some more bread!
Little Red Hen      : A fresh batch of bread is ready.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Sleepy Cat          : Hey, Little Red Hen, make some more bread!
Little Red Hen      : A fresh batch of bread is ready.
Sleepy Cat          : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Sleepy Cat          : Hey, Little Red Hen, make some more bread!
Little Red Hen      : A fresh batch of bread is ready.
Little Red Hen      : I'm fed up with feeding you lazy animals! No more bread!
Sleepy Cat          : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Noisy Yellow Duck   : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : Mmm, this loaf is delicious.
Sleepy Cat          : Mmm, this loaf is delicious.
Noisy Yellow Duck   : I've had my fill of bread. Thanks, Little Red Hen!
Lazy Dog            : Mmm, this loaf is delicious.
Lazy Dog            : I've had my fill of bread. Thanks, Little Red Hen!
Sleepy Cat          : I've had my fill of bread. Thanks, Little Red Hen!
Everything finished.
```

## Priority

This problem refers to the code in `priority.c`. In this system, there are
threads numbered 1 through 6 and a critical section. The thread numbers are
priorities, where thread 6 is the one with the highest priority, thread 5 is
next, etc.

If the critical section is empty, any thread can enter. While a thread is in the
critical section, no other thread can enter, regardless of its priority. When a
thread leaves the critical section and there are threads waiting, __the highest
priority waiting thread is allowed to enter the critical section__.

Make sure in your code that a newly arriving thread cannot jump into the
critical section as the current thread finishes, bypassing priority rules.

Write a solution that satisfies the above constraints using mutex lock and
condition variables.

## Three Jobs

This problem refers to the code in `threeJobs.c`. In this system, imagine there
is a shared memory space called house.

There are 3 different kinds of operations on house: carpenters, painters, and
decorators.  For any particular kind of operation, there can be an unlimited
number of threads doing the same operation at once (e.g., unlimited carpenter
threads, etc.).  However, only one kind of operation can be done at a time (so
even a single carpenter should block all painters and vice versa).

Use mutex locks and condition variables to enforce this constraint.  You don't
have to worry about starvation (e.g. that constantly arriving decorators might
prevent carpenters from ever running) - though maybe it would be fun to
consider how you would solve in that case.

This is similar to the readers/writers problem BTW.

### Sample output

Below is a sample of the output generated by a trace of the program that
satisfies the above constraint.

```
starting carpentry
starting carpentry
starting carpentry
finished carpentry
finished carpentry
finished carpentry
starting painting
starting painting
starting painting
finished painting
finished painting
finished painting
starting decorating
starting decorating
starting decorating
finished decorating
finished decorating
finished decorating
Everything finished.
```

## Swap A B

This problem refers to the code in `swapab.c`.

In this problem, you must implement 6 separate functions. The first 4 are pretty
easy to understand:
```c
void lockA();
void lockB();

void unlockA();
void unlockB();
```

Basically, A and B can be though of as two independent mutex locks. If you call
`lockA()` and some other thread has already locked A, `lockA` will be stuck and
will not return until the thread with A locked calls `unlockA()`.
`lockB` and `unlockB` act in the same way, however, the lock state of B has no
effect on A and vice versa.

I say A and B can be "thought of as two independent mutexes" rather than "are 2
mutexes" because you can implement these functions in any way you wish, as
long as you provide the normal guarantees.  So you can have 1 underlying mutex
lock and just some condition variables - or whatever.

You can also assume that no single thread will ever attempt to
acquire both mutex locks at once:
```c
lockA();
lockB(); // undefined behavior!  Not allowed
```

If you wish, you can implement `lockA`/`lockB` to spit out an error if someone
attempts this, but you can also just assume that this rule will not be broken.
Note that this rule applies to callers of your lock/unlock functions - you can
do whatever you wish, so long as you behave correctly whenever the lock/unlock
functions are called in a legal way.

### The swap functions.

The two other function you must implement are
```c
swapAB();
swapBA();
```

A swap function allows the caller to excahnce one mutex lock for another. For
example, if you call `swapAB()` when your thread has mutexA, then when the
function returns,  your thread will have released A but will have acquired B. 

The key requirement of the `swapAB` is the following:

> There should never be a time when the caller of `swapAB` has already released
A but has not yet acquired B. Put another way, if B is not available, the
function will wait for B while holding A, and not swap until it is guaranteed
to get B.

It is possible that 2 threads both want to swap (e.g., thread 1 is trying to
`swapAB` and is waiting because B is claimed, but then thread 2, which has B
executed `swapBA`).
In this case, the swap should happen conceptually "at the same time": both the
swap functions return, and neither of the mutex locks should be in an available
state where they could be stolen by a thread attempting a `lockA` or `lockB`.

### Hint
One thing to note and always remember is that the thread the locks a mutex lock
should be the one that unlocks it. Attempting to unlock another thread's mutex
lock results in undefined behavior. This problem can be circumvented by using a
semaphore, which you are allowed to use in this problem.
