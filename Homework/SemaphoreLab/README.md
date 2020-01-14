---
title: Threads and Semaphore Basics
layout: togit
---

# Threads and Semaphore Basics

# Thread Factoring (20 points)

Look in the thread\_factoring project in the Homework/SemaphoreLab
directory.

Start by understanding the code in threadExample.c (you may have seen
this example already, but if not review it).

Then use threads to parallelize the problem given in factoring.  If
you do it correctly you should be able to see the correct number of
threads working in parallel.

Here's some sample output from my solution.  Note that when given two 
threads, my solution will make one thread handle evens and one
thread handle odds.  Your approach does not have to split up the problem
this way. It divides the number of values each thread considers evenly.

    Give a number to factor.
    143
    How man threads should I create?
    2
    running thread
    running thread
    thread 2 testing 3
    thread 2 testing 5
    thread 2 testing 7
    thread 2 testing 9
    thread 2 testing 11
    11 is a factor
    thread 2 testing 13
    13 is a factor
    thread 2 testing 15
    thread 2 testing 17
    thread 2 testing 19
    thread 2 testing 21
    thread 1 testing 2
    thread 1 testing 4
    thread 1 testing 6
    thread 1 testing 8
    thread 1 testing 10
    thread 1 testing 12
    thread 1 testing 14
    thread 1 testing 16
    thread 2 testing 23
    thread 2 testing 25
    thread 2 testing 27
    thread 2 testing 29
    thread 2 testing 31
    thread 2 testing 33
    thread 2 testing 35
    thread 2 testing 37
    thread 2 testing 39
    thread 2 testing 41
    thread 2 testing 43
    thread 2 testing 45
    thread 2 testing 47
    thread 2 testing 49
    thread 1 testing 18
    thread 1 testing 20
    thread 1 testing 22
    thread 1 testing 24
    thread 1 testing 26
    thread 1 testing 28
    thread 1 testing 30
    thread 1 testing 32
    thread 1 testing 34
    thread 1 testing 36
    thread 1 testing 38
    thread 1 testing 40
    thread 1 testing 42
    thread 1 testing 44
    thread 1 testing 46
    thread 1 testing 48
    thread 1 testing 50
    thread 1 testing 52
    thread 1 testing 54
    thread 1 testing 56
    thread 1 testing 58
    thread 1 testing 60
    thread 1 testing 62
    thread 1 testing 64
    thread 1 testing 66
    thread 1 testing 68
    thread 1 testing 70
    thread 2 testing 51
    thread 2 testing 53
    thread 2 testing 55
    thread 2 testing 57
    thread 2 testing 59
    thread 2 testing 61
    thread 2 testing 63
    thread 2 testing 65
    thread 2 testing 67
    thread 2 testing 69
    thread 2 testing 71

You will need to pass parameters to your thread functions to solve
this problem.  Just use void pointers just as you did in your userspace
threading assignment.


# Thread Sorting (40 points)

Look at the thread\_sorting project in the lab directory. The
Costs.txt file in this project should be used as a sample file for
this part of the lab. Don't forget to compile with the -pthread
compilation flag.

In this program you will demonstrate your understanding of thread
creation, some thread synchronization, and POSIX thread
management. 

We want to read in an input file of integers and sort it.  We'll do
this with 3 different sorting methods (all the sorting code is given
to you).  We will do this sorting in parallel using threads.  We will
print some info about how fast the various sort methods were, then
we'll merge all the various sorted sub-files together into one sorted
list and output it to a file.

The program is in the file threads.c and takes three command line arguments: 
    
1.  the number of threads to create (n),
2.  an input file
3.  an output file
    
in that order. 

Build the file like this:

    gcc -pthread -ggdb threadSort.c -o threadSort


1.  We've given you code that handles the input arguments and file
    reading.  Your first goal will be the read the numbers from the
    input file into some structures that are convenient for
    processing (might I suggest a two dimension array?).

2.  Use a loop to create n parallel threads, distributing the number
    of values evenly across each thread. 
    
    a. Each thread will sort its group of values using a different
     sorting algorithm. In each thread, call one of three sorting
     algorithms (provided in threads.c). One third of the threads
     created should use a brute force sorting algorithm.  One third
     should use bubble sort. And finally, one third should use merge
     sort.
    
    b. Each thread should track the time it takes to operate (use
     gettimeofday), starting upon creation and ending once the
     sorting is complete.
    
    c. Each thread should print its index (an int that
     represents the order in which the thread was created) and
     total time (as described above) upon completion.

4.  In a loop, the parent process should wait (using pthread\_join)
    for each thread to complete. Once all of the of the n threads have
    completed, the parent should call a user defined function that
    calculates the maximum, minimum, and mean values for the execution
    times of each sorting algorithm.  It should then print these
    values to the console.

5.  At this point the values should be semi-sorted: the first
    third should be sorted via the brute force method, the second
    third by the bubble sort method, and the third third by the merge
    sort method.

6.  Finally, merge the results of each thread to create one sorted
    list.  The nicest way to do this is create a new output array to
    sort all the output and then repeatedly copy individual sorted
    segments into the region, then merge them with the Merge function.
    
    You do not need to use parallelism in this part.
    
    You could also copy them all into the output and then merge them
    with a single sort that doesn't take advantage of the fact that
    the sub-lists are sorted, but this is the cowards way out and we'll
    take off a small number of points if you do.

You only need to submit threadSort.c for this part.

# Basic semaphores 1 (20 points)


## The problem

Compile and run add\_a\_lot.c.  You will see that it uses multiple
threads to repeatedly add to a global variable.  However, the final
sum is not correct because as multiple threads edit the variable they
interfere.

To be clear, a variable add looks something like this in assembly:

1.  Load the variable from memory into the register
2.  Add, possibly outputting in some other register
3.  Store the variable in the output register into memory

However, if we have multiple threads acting at once, the interleaving
can cause the count to fail.

1.  Load the variable from memory into the register (thread 1)
2.  Load the variable from memory into the register (thread 2)
3.  Add, outputting in some other register (thread 2)
4.  Store the variable in the output register into memory (thread 2)
5.  Add, outputting in some other register (thread 1)
6.  Store the variable in the output register into memory (thread 1)
    and discarding Thread 2's change


## The solution - Semaphores

You might be tempted to try to use some tricky stuff involving special
booleans to prevent concurrent modification.  We'll explain more in
future classes, but for now realize that these kind of things tend to
have their own problematic edge cases unless built very very
carefully.  Let's not do that ourselves - instead we'll use
semaphores.

Semaphores are one of a variety of concurrency synchronization
mechanisms your system might provide.  They are very powerful and
flexible, but in this assignment we'll use them in the simplest
possible way.


## Operation

Semaphores contain an internal count.  There are two operations:

1.  sem\_post - increases the count.  This has the potential to unblock
    waiting processes.
2.  sem\_wait - attempt to decrement the count.  If the count is greater
    than 1, this works and the process continues.  If the count is 0,
    the process blocks (i.e. does not return) until some other thread
    increments the count after which the thread can continue.

These are all carefully designed so that, regardless of the
interleaving of calls, the semaphore behaves consistently.  For
example, if say semaphore count\_1 is one, and two processes simultaneously
attempt to sem\_wait() on it, exactly one will be allowed to continue and
exactly one will be blocked waiting for a sem\_post.

How can we use these two operations to protect our code?  We could
make our data an actual semaphore, but that is not very flexible.
Instead, we want to use a semaphore to ensure that multiple threads
never enter our "critical section" (that is, our global increment) at
the same time.

See if you can figure out how to do this on your own without peeking
at the next section.



## Using Semaphores to protect a critical section

So the idea is that we're going to create a semaphore and initialize
its count to 1.

Then in the code we want to protect from interference, we'll to this:

1.  sem\_wait - if the count is zero that means another thread is in
    the critical section so we'll block
2.  Now we know we're the only thread in the critical section so we
    can safely update the global variables
3.  sem\_post - increment the count because we're out of the critical
    section and we want to let any other threads that are blocked to
    proceed


## Doing this in code

1.  Include the proper header
    
        #include <semaphore.h>

2.  Declare a global semaphore
    
        sem_t semaphore;

3.  In your main, initialize the semaphore with an initial count of 1
    
        sem_init(&semaphore, 0, 1);

4.  Before the code you'd like to protect starts, first wait to ensure
    you don't proceed till you have "locked" the semaphore:
    
        sem_wait(&semaphore);
    
    You'll want to make the critical section as small as possible to
     allow for maximal concurrency.

5.  After the code you'd like to protect finishes, "unlock" the
    semaphore:
    
        sem_post(&semaphore);

6.  Once you're sure all the threads are finished using the semaphore,
    clear it out (this usually happens in main after some joins):
    
        sem_destroy(&semaphore);


## Solve it!

Use the steps above to solve the problem in add\_a\_lot.c.


<a id="orgeac34cc"></a>

# Basic Semaphores 2: RedBluePurple (50 points)

For a detailed description of the problem look at the comments in the
red\_blue\_purple.c file.


<a id="orge9975d8"></a>

# Rubric

<table border="2" cellspacing="0" cellpadding="6" rules="groups" frame="hsides">


<colgroup>
<col  class="org-left" />

<col  class="org-left" />

<col  class="org-right" />
</colgroup>
<tbody>
<tr>
<td class="org-left">Thread Factoring</td>
<td class="org-left">creates threads and they run</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Factoring</td>
<td class="org-left">correctly divides work and displays well</td>
<td class="org-right">10</td>
</tr>

<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Loop and create n parallel threads to sort parts of the data</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Each thread uses timeofday to measure total execution time and prints index and execution time</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Computes and prints min max and mean times for each algorithm</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Thread Sorting</td>
<td class="org-left">Final merges to create one sorted list (-5 if you don't take advantage of the sortedness with Merge)</td>
<td class="org-right">10</td>
</tr>


<tr>
<td class="org-left">Basic Semaphores 1</td>
<td class="org-left">Follows instructions and eliminates concurrency bug using semaphores</td>
<td class="org-right">20</td>
</tr>


<tr>
<td class="org-left">Basic Semaphores 2</td>
<td class="org-left">Semaphores used correctly (e.g. created, freed with proper functions)</td>
<td class="org-right">25</td>
</tr>


<tr>
<td class="org-left">Basic Semaphores 2</td>
<td class="org-left">All concurrency bugs elliminated</td>
<td class="org-right">25</td>
</tr>
</tbody>
</table>

