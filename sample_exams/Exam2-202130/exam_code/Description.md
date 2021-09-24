# Exam 2 - 202130

This exam is composed of four problems, three of which are semaphore problems
and one is a `pthreads` problem. 

## Rubric for semaphore problems
* You can get 25% points if your semaphore-based solution reliably
  produces correct output with the given data (and small
  modifications).  0 points if doing this requires weird sleeps or
  other hacky non-general purpose code.
* You can get 50% points if you did the above and your solution is
  plausible and does not do something obviously bad.  It's impossible
  to enumerate every obviously bad thing but here's a big one : having
  multiple concurrent threads modify a global variable.  Basically
  this is for solutions that basically are OK but miss some edge
  cases.
* You can get 75% points if you have a fully working solution but
  you utilize busywaits.
* A fully correct solution, even if it's not the one we envisioned, is
  100%.

---

# Problem 1 (30 points)

This problem refers to the file `reverse_verify.c`

Give the text file `lotr-part.txt`, the program will reverse the file content
character (byte) by character, and then verify if the file is successfully
reversed.

We provide you with the non-parallel solution. If you compile and run the code,
you will see something that looks as follows:
```shell
$ ./reverse_verify
Non-paralle method finished in 3686 usecs.
All done
```
Note that the time cost may vary on your machine.

Your task is to accelerate the program by parallelizing it using `pthreads`.
Note that you will also need to print out the time cost for the parallel method
for comparison. We provide you with the code for calculating the run time and
printing it out.

## Step 1: Parallelize reverse (15 points)

The code for opening and reading the file is provided for you, free of cost. You
should start with adding code to the reverse function to support parallel
operations. As you can see in the code for the `reverse` function, it simply reads
the data in `file_buff` backwards and writes it into `output_buff`.

Your job is to do the following:
1. Add code to the `reverse` function to make it runnable by `pthreads`. When
   run by `pthreads`, a `pthread` will pass a non-NULL pointer as the input
   argument to this function. Note that the number of reverse threads is fixed
   and predefined to be 2 (as shown in `NUM_REV_THREADS`).
1. Add code in the `main` function to set up the `pthreads` (including creating
   and joining). After joining the threads, __add one line of code__
   `verify(NULL)` to verify the reverse results.
1. Uncomment the code that measures the run time and prints out the time cost.
   The time cost should include both `reverse` (the parallelized version now) and
   `verify` (still non-parallel).

After this step, the program output should like this:
```shell
$ ./reverse_verify
Non-parallel method finished in 3797 usecs.
    Parallel method finished in 2621 usecs.
All done
```

You should see the run time of the parallel method is significantly improved
over the non-parallel version with a speedup of 1.44 (3797/2621). However, note
that the results on your machine might be different.

## Step 2: Parallelize verify (15 points)

In this step, you need to use __ADDITIONAL__ threads to parallelize the verify
function.

Similar to the previous step, you need to add code in the `verify` function as
well as in `main`. The number of `verify` threads is also pre-defined in
`NUM_VERI_THREADS` and is equal to the number of `reverse` threads.

In order to maximize parallelism, you want to start, i.e., call
`pthread_create`, all threads together (2 `reverse` threads and 2 `verify`
threads).

However, a `verify` thread must wait until the part of the data it will check
has been reversed. Otherwise, the data is not ready and will result in an error
when verifying. __You need to use semaphores to solve this problem__.

__Hint__: I used an array of semaphores with size equal to `NUM_VERI_THREADS`.

When calculating the time cost, don't forget to comment out the `verify(NULL)`
used in the previous step. Instead, you want to use threads to do the
verification.

After parallelizing `verify` and adding semaphores to solve the concurrency
issues, the output should be something that looks like this:
```shell
$ ./reverse_verify
Non-parallel method finished in 3689 usecs.
    Parallel method finished in 2093 usecs.
All done
```
As you can see, the run time for the parallel method is further improved with
the speedup equal to 1.76 (3689/2093). The speedup may vary on your computer
but should be better compared to the previous case.

If your semaphore solution is faulty, most likely you will see the error
message: "Reversing is not correct. Quiting program now...."

---

# Problem 2 (20 points)

This problem refers to the file `canoe.c`.

In a water park, people take a canoe for a short tour for fun. Here are some
constraints/regulations we have to work with when coordinating the canoe tour.

1. There is only one canoe in this park, i.e., one tour at a time.
1. The tourists include both adults and kids (they are different pthreads in our
   program). An adult can ride the canoe alone, while a kid can not. A kid must
   take the tour with an adult. The capacity of the canoe is exactly an adult
   and a kid.
1. Assume that all the adults are friendly. They always voluntarily take a kid
   with them if there are kids waiting. But if there are no kids waiting when an
   adult starts the tour, the adult can go alone without any delay or waits.
1. In the `Kid` and `Adult` functions, both will print out messages before and
   after finishing the tour. Particularly, for the `Adult` function, it will
   print out different messages (see the commented out code) after finishing
   canoeing to identify whether or not this adult took the tour with a kid or
   not.

__Hint__: I used a total of three semaphores to solve this problem.

If your implementation is correct, try to run the program using the command:
```shell
$ make canoe
$ ./canoe | grep canoe
```
then you should see something that looks like this:
```shell
                ---> an Adult is canoeing.
                an Adult finished canoeing.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                ---> a Kid is canoeing.
                an Adult finished canoeing with a Kid.--->
                ---> an Adult is canoeing.
                an Adult finished canoeing.--->
```

Given the `pthreads` setup, the program should always finish without halting or
blocking. If your program is halted, it indicates possible concurrency issues
that you will need to fix.

---

# Problem 3 (25 points)

This problem refers to the code in `chopsticks.c`.

A factory uses two kinds of machines, stick producer and stick bundler, to make
chopsticks. Two stick producers (i.e., two pthreads) will produce sticks and
feed them into a bundler (another pthread).

For every two sticks produced by the stick producers, the bundler will pack
them into a pair of chopsticks. Note that the two sticks can come from either
of the two stick producers or from both of them. In other words, it is not
necessary that each stick producer will contribute exactly one stick to the
bundler at a time.

The stick producers won't produce more sticks until the two newly-produced
sticks are bundled by the bundler. In other words, for every two sticks the
stick producers produce, they will stop and wait for the bundler to finish
bundling these two sticks and then resume producing the next two sticks.

The program will stop after it finishes processing 8 sticks in total, i.e.,
the stick producers will stop when the TOTAL (produced by both) count of sticks
reaches 8.


Hint: I used in total three semaphores to solve this problem.

If your implementation is correct, try to run program using the commands
below (it will take a few seconds to finish):
```shell
$ make chopsticks
$ ./chopsticks
```
then you should see something that looks like this:
```shell
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
One stick produced from #1 machine...
One stick produced from #2 machine...
Two sticks were bundled as a pair of chopsticks
bundler done
producer 1 done
producer 2 done
All done
```

The producers may not alternate perfectly, which is okay. Technically, the
producer numbers in the output can be totally random.

**Note that you want to see all three of the machines done at the end**. If not,
then your program may be blocked by an unwanted `sem_wait` that you will need to
fix.

---

# Problem 4 (25 points)

This problem refers to the code found in the `priority_tunnel.c` file.

There are three types of carts (A, B, and C) that want to go through a
tunnel. When passing through the tunnel, the carts must follow certain
policies that are listed below:

1. When there is no cart in the tunnel, whoever arrives first can enter (i.e.,
   First Come First Serve).
2. If there is a cart currently passing the tunnel, the late-arrived carts with
   the same type as the current passing cart(s) can enter the tunnel without
   waiting. The carts with a different type will have to wait. For example, if
   there is a B cart currently in the tunnel, then any B cart that arrives
   before the current B cart finishes passing can directly go into the tunnel.
   However, A carts and C carts will have to wait until all the B carts finish.
3. With the two policies above, we could incur starvation. For example, if there
   are B carts in the tunnel, then as long as there are continuing B carts
   arriving before the last B cart in the tunnel leaves, the carts of other
   types (A and C) will never enter. Therefore, we need to enable priority among
   the carts to prevent starvation of certain types of carts. The priority order
   we want is A>B>C. And the way priority works is that, when a cart with a
   higher priority arrives and waits for entering the tunnel, it will prevent
   later-arriving low priority cars from entering the tunnel. For example, assume
   there are B carts in the tunnel, if there is a cart A arriving, then any B
   carts that arrive after that cart A can't go into the tunnel, even if they
   have the same type as the carts currently in the tunnel.  Similarly, A carts
   will prevent later-arriving C carts, and B carts will also prevent
   later-arriving C carts.

__Hint__: My minimalistic solution uses in total three semaphores as I only use
one semaphore to protect all global variables.

If your implementation is correct, try to run the program using
```shell
$ ./priority_tunnel |  grep tunnel
```
It might take a few seconds to finish, and you should then see something that
look as follows:
```shell
Competing tunnel between B and C...
                ---> a <C> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
Competing tunnel between A and B...
                ---> a <B> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
                ---> a <B> cart goes into the tunnel.
Competing tunnel between A and C...
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> an <A> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
                ---> a <C> cart goes into the tunnel.
```

It is okay to have a few low-priority carts go before high-priority carts. It is
because they arrived slightly earlier. But the overall printout order should
follow the priority order.