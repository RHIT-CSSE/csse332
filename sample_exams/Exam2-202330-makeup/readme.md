# CSSE332 Makeup Exam 2 Spring 2022-23

# Prelude

This exam contains four problems:

1. `threadpool.c` is a thread creation and mutex locks problem.
   
2. `critical.c` is a condition variables concurrency problem.

3. `game.c` is a condition variables concurrency problem.

4. `lunch.c` is a condition variables concurrency problem.

We have provided you with a `Makefile` to speed up your compilation and linking
steps. Please do not edit the `Makefile` or change the optimization settings in
`gcc`.

The files `*_tests.c` and `prettyprint.h` are there to help you set things up.
__Do not edit these files__; the correctness of your solution should not be
dependent on the content of these files.

## Helper functions

To help you out a bit in debugging, we defined some colored variant of `printf`,
those can be found as macros in the `prettyprint.h` file. Your can use them in
the exact same way as you would `printf`, except that they will adjust the color
of your output. For example, to print `Hello from thread <tid>` in green, you
can use:
```c
// assume tid is an integer somewhere
pr_green("Hello from thread %d\n", tid);
```

You do not have to use these functions, they are there in case you need a hand
differentiating your output.

However, note that these macros assume that your terminal supports ANSI colors,
which most modern terminal emulators do. In case you are using a niche variant
of a terminal emulator and the colors do not show up, do not panic, just use
normal `printf` or use a more common terminal emulator.

# Problem 1: Thread pool (30 points)

In this problem, we are simulating a web-server that responds to client
requests. For simplicity, we assume that the requests are static and are stored
in a stack. Your job is to design the pool of threads that will process these
requests in parallel.  Each request consists of a single character that the
threads must process. No two threads should process the same request and no
request shall be processed more than once.

## Step 1: Creating the thread pool (10 points)

In the first step, create a pool of `NUM_THREADS` threads that will execute a
function, let's call it `worker_thread`. Make sure to specify the correct return
value and the correct argument type for that function.

Each thread __must__ have a unique id that you pass to it when creating it. Make
sure that your main thread waits for all your threads to come back. Initially,
make all your threads just print the following line:
```
Worker thread <tid> started."
```
where `<tid>` is the id of that thread.

Compile your code using `make` and run it, your output should look like:
```shell
-> % ./threadpool.bin
Worker thread 1 started.
Worker thread 0 started.
Worker thread 2 started.
Worker thread 3 started.
Worker thread 4 started.
Worker thread 5 started.
Worker thread 6 started.
Worker thread 7 started.
Worker thread 8 started.
Worker thread 9 started.
****** Everything finished ******
```

## Step 2: The worker thread (5 points)

Let's write an initial version of your worker threads. Have each thread pop an
element from the stack and print `Thread <tid> processing request <char>` where
`<tid>` is the ID of the thread and `<char>` is the character popped from the
stack, and then make the thread sleep for 1 second using `sleep(1);`.  Don't
worry about concurrency yet.

To pop an element from the stack, you can use the provided function `pop`.
```c
char c = pop();
```

Here's a sample output from running this on my machine. Note that each thread
processes exactly one job and then leaves.

```shell
-> % ./threadpool.bin
Worker thread 0 started.
Thread 0 processing request a
Worker thread 1 started.
Thread 1 processing request b
Worker thread 2 started.
Thread 2 processing request c
Worker thread 3 started.
Thread 3 processing request d
Worker thread 4 started.
Thread 4 processing request e
Worker thread 5 started.
Thread 5 processing request f
Worker thread 6 started.
Thread 6 processing request g
Worker thread 7 started.
Thread 7 processing request h
Worker thread 8 started.
Thread 8 processing request i
Worker thread 9 started.
Thread 9 processing request j
****** Everything finished ******
```

Note that the order of printing does not matter.

## Step 3: Making sure all requests are processed (15 points)

In our current implementation, if we have more requests than we have threads,
the thread will not execute more requests, they rather exit. Now, modify your
code for the worker thread to keep running until all requests have been
processed. You can use the provided function `is_empty()` to check if there are
more elements on the stack. Threads must exit only after the stack is empty.

However, we have a problem (was present also in step 2) with threads accessing
the stack at the same time; this is a race condition that might lead threads to
process the same request or even miss requests. Use mutex locks to solve any
concurrency issues that might arise in your implementation.

Note that while a thread is processing a request, it __should not__ block other
worker threads from accessing the stack, the other ones should be able to pick
up new jobs.

Here's an example output from running this on my machine:

```shell
-> % ./threadpool.bin
Worker thread 0 started.
Thread 0 processing request a
Worker thread 1 started.
Thread 1 processing request b
Worker thread 2 started.
Thread 2 processing request c
Worker thread 3 started.
Thread 3 processing request d
Worker thread 4 started.
Thread 4 processing request e
Worker thread 5 started.
Thread 5 processing request f
Worker thread 6 started.
Thread 6 processing request g
Worker thread 7 started.
Thread 7 processing request h
Worker thread 8 started.
Thread 8 processing request i
Worker thread 9 started.
Thread 9 processing request j
Thread 0 processing request k
Thread 1 processing request l
Thread 2 processing request m
Thread 3 processing request n
Thread 4 processing request o
Thread 5 processing request p
Thread 6 processing request q
Thread 7 processing request r
Thread 8 processing request s
Thread 9 processing request t
Thread 0 processing request u
Thread 1 processing request v
Thread 2 processing request w
Thread 3 processing request x
Thread 4 processing request y
Thread 5 processing request z
Thread 6 processing request 1
Thread 7 processing request 2
Thread 8 processing request 3
Thread 9 processing request 4
Thread 0 processing request 5
Thread 1 processing request 6
Thread 2 processing request 7
Thread 3 processing request 8
Thread 4 processing request 9
****** Everything finished ******
```

# Problem 2: Critical (20 points)

In this system, we have two types of threads: red and blue. We have a
critical section that can hold __at most__ three threads at the same time.
However, there cannot be more than two threads of the same type. In other words,
it is not allowed for the critical section to have three threads of the same
type at the same time. The order with which waiting threads enter the critical
section does not matter.

Here are the rules:

- No more than three threads are allowed in the critical section. It is okay to
  have less than 3 threads in the critical section.

- At most, two threads of the same type can be in the critical section. For
  example, `<blue, blue, red>`, `<red, red, blue>`, `<red, blue, red>` are all
  allowed combinations. However, `<red, red, red>` and `<blue, blue, blue>` are
  not allowed.

- The order in which the threads enter the critical section does not matter.

- When a thread enters the critical section, it should not wait for anyone else,
  it does its job (which is to `sleep(1);`) and then leaves.

Use mutex locks and condition variables to implement the above requirements.

## Sample output

Compile your code using `make`.  We provide you with 3 sample scenarios to test
your code. Note that those scenarios are not exhaustive, we will still look at
your code and judge it for correctness. You can find the description of each
scenario (or test case) in the `descriptions` at the top of the file. Also, any
time you run a scenario, it will print out its description on the console.

To run a specific scenario, provide its scenario number as a command line
argument to the `critical.bin` binary. First, compile your code using `make` and
then run `./critical.bin <scenario number>` to run a specific case.

Below is my sample output for each scenario.

### Test 1

```shell
-> % ./critical.bin 1
Running test number 1: All red threads, should go 2 by 2.

Red thread 1 arrived
Red thread 1 entering critical section
Red thread 2 arrived
Red thread 2 entering critical section
Red thread 3 arrived
Red thread 4 arrived
Red thread 5 arrived
Red thread 6 arrived
Red thread 1 leaving critical section
Red thread 2 leaving critical section
Red thread 5 entering critical section
Red thread 6 entering critical section
Red thread 5 leaving critical section
Red thread 4 entering critical section
Red thread 6 leaving critical section
Red thread 3 entering critical section
Red thread 4 leaving critical section
Red thread 3 leaving critical section

Everything finished...
```

### Test 2

```shell
-> % ./critical.bin 2
Running test number 2: All blue threads, should go 2 by 2.

Blue thread 1 arrived
Blue thread 1 entering critical section
Blue thread 2 arrived
Blue thread 2 entering critical section
Blue thread 3 arrived
Blue thread 4 arrived
Blue thread 5 arrived
Blue thread 6 arrived
Blue thread 7 arrived
Blue thread 8 arrived
Blue thread 1 leaving critical section
Blue thread 2 leaving critical section
Blue thread 7 entering critical section
Blue thread 4 entering critical section
Blue thread 7 leaving critical section
Blue thread 4 leaving critical section
Blue thread 6 entering critical section
Blue thread 5 entering critical section
Blue thread 6 leaving critical section
Blue thread 5 leaving critical section
Blue thread 3 entering critical section
Blue thread 8 entering critical section
Blue thread 3 leaving critical section
Blue thread 8 leaving critical section

Everything finished...
```

### Test 3

```shell
-> % ./critical.bin 3
Running test number 3: All threads arrive together, they go 3 by 3 by which ones go in non-determined.

Blue thread 1 arrived
Blue thread 1 entering critical section
Red thread 2 arrived
Red thread 2 entering critical section
Blue thread 3 arrived
Blue thread 3 entering critical section
Red thread 4 arrived
Blue thread 5 arrived
Red thread 6 arrived
Blue thread 7 arrived
Red thread 8 arrived
Blue thread 9 arrived
Red thread 10 arrived
Blue thread 11 arrived
Red thread 12 arrived
Blue thread 13 arrived
Red thread 14 arrived
Blue thread 1 leaving critical section
Blue thread 3 leaving critical section
Red thread 2 leaving critical section
Red thread 12 entering critical section
Blue thread 5 entering critical section
Red thread 4 entering critical section
Red thread 12 leaving critical section
Blue thread 5 leaving critical section
Red thread 4 leaving critical section
Blue thread 11 entering critical section
Red thread 10 entering critical section
Blue thread 9 entering critical section
Blue thread 11 leaving critical section
Red thread 10 leaving critical section
Blue thread 9 leaving critical section
Blue thread 13 entering critical section
Red thread 14 entering critical section
Red thread 8 entering critical section
Blue thread 13 leaving critical section
Red thread 14 leaving critical section
Blue thread 7 entering critical section
Red thread 8 leaving critical section
Red thread 6 entering critical section
Blue thread 7 leaving critical section
Red thread 6 leaving critical section

Everything finished...
```

# Problem 3: Game (30 points)

In this setup we have two teams, a home team and an away team, and a referee.
The game cannot start until all players have arrived on the field. Once all
players are in, the referee must blow the whistle before the players can start
playing. Once the game is over, the away team can exit the field directly while
the home team must wait for their star player to do an interview and then
leave the field. The star player of the home team is the one that has a jersey
number 0 (i.e., thread number 0).

Here are some rules:

- No player can start playing before the referee blows the whistle.

- The referee cannot blow the whistle before all players have arrived.

- We simulate the game happening with a `sleep(GAME_TIME);` statement for all
  players and the referee.

- Once the game is over, the referee and the away team can leave the field
  directly.

- The star player of the home team will conduct an interview with the press
  after the game ends. We simulate that with a `sleep(INTERVIEW_TIME);`
  statement.

- The home team cannot leave the field until their star player's interview is
  over.

Use mutex locks and condition variables to implement the above requirements.

## Sample output

Compile your code using `make`.  We provide you with 3 sample scenarios to test
your code. Note that those scenarios are not exhaustive, we will still look at
your code and judge it for correctness. You can find the description of each
scenario (or test case) in the `descriptions` at the top of the file. Also, any
time you run a scenario, it will print out its description on the console.

To run a specific scenario, provide its scenario number as a command line
argument to the `game.bin` binary. First, compile your code using `make` and
then run `./game.bin <scenario number>` to run a specific case.

Below is my sample output for each scenario.

### Test 1

```shell
-> % ./game.bin 1
Running test number 1: Players arrive first, referee is the last one to make it through.

Player 0 from home team arrived
Player 1 from home team arrived
Player 2 from home team arrived
Player 0 from away team arrived
Player 1 from away team arrived
Player 2 from away team arrived
Referee has arrived on the field

🏀🏀 REFEREE BLOWING THE WHISTLE, GAME CAN START 🏀🏀

Player 1 from away team starts playing
Player 0 from home team starts playing
Player 1 from home team starts playing
Player 2 from home team starts playing
Player 0 from away team starts playing
Player 2 from away team starts playing
Referee exiting the field
Player 1 from away team exiting the field
Star player of home team conducting interview
Player 0 from away team exiting the field
Player 2 from away team exiting the field
Interview is over
Player 0 from home team exiting the field
Player 1 from home team exiting the field
Player 2 from home team exiting the field

Everything finished...
```

### Test 2

```shell
-> % ./game.bin 2
Running test number 2: Referee arrives first, players arrive after that.

Referee has arrived on the field
Player 0 from home team arrived
Player 1 from home team arrived
Player 2 from home team arrived
Player 0 from away team arrived
Player 1 from away team arrived
Player 2 from away team arrived

🏀🏀 REFEREE BLOWING THE WHISTLE, GAME CAN START 🏀🏀

Player 1 from away team starts playing
Player 1 from home team starts playing
Player 2 from home team starts playing
Player 0 from away team starts playing
Player 2 from away team starts playing
Player 0 from home team starts playing
Referee exiting the field
Player 0 from away team exiting the field
Player 2 from away team exiting the field
Star player of home team conducting interview
Player 1 from away team exiting the field
Interview is over
Player 0 from home team exiting the field
Player 1 from home team exiting the field
Player 2 from home team exiting the field

Everything finished...
```

### Test 3

```shell
-> % ./game.bin 3
Running test number 3: Some players arrive before the referee, some players arrive after.

Player 0 from home team arrived
Player 0 from away team arrived
Player 1 from home team arrived
Referee has arrived on the field
Player 2 from home team arrived
Player 1 from away team arrived
Player 2 from away team arrived

🏀🏀 REFEREE BLOWING THE WHISTLE, GAME CAN START 🏀🏀

Player 1 from away team starts playing
Player 0 from away team starts playing
Player 1 from home team starts playing
Player 2 from away team starts playing
Player 2 from home team starts playing
Player 0 from home team starts playing
Referee exiting the field
Player 1 from away team exiting the field
Player 0 from away team exiting the field
Player 2 from away team exiting the field
Star player of home team conducting interview
Interview is over
Player 0 from home team exiting the field
Player 1 from home team exiting the field
Player 2 from home team exiting the field

Everything finished...
```

# Problem 4: Lunch (20 points)

Imagine a group of friends who meet in a particular place to go together to
lunch.  They don't want to wait too long to eat, so they might go in separate
groups if some friends are late to arrive.  These friends also like to argue
about which restaurant to go to.

When a friend arrives they should begin arguing (even if at this point they are
by themselves).  Arguing takes `ARGUE_TIME` to complete.  If `ARGUE_TIME`
elapses and no new friends arrive, the friends reach a decision about which
restaurant to go to, and that group of friend heads off.  Any friends arriving
after the decision has been made must be part of a subsequent group.

If while the friends are arguing a new friend arrives, that new friend's
preferences means the argument must start from scratch.  The argument begins
again and `ARGUE_TIME` must elapse before the friends can go to lunch (and of
course if more friends arrive, we must again reset).

Use mutex locks and condition variables to implement the above requirements.

## Sample output

Compile your code using `make`.  We provide you with 4 sample scenarios to test
your code. Note that those scenarios are not exhaustive, we will still look at
your code and judge it for correctness. You can find the description of each
scenario (or test case) in the `descriptions` at the top of the file. Also, any
time you run a scenario, it will print out its description on the console.

To run a specific scenario, provide its scenario number as a command line
argument to the `lunch.bin` binary. First, compile your code using `make` and
then run `./lunch.bin <scenario number>` to run a specific case.

Below is my sample output for each scenario.

### Test 1

```shell
-> % ./lunch.bin 1
Running test number 1: Threads keep coming and they must wait for all of them, even if two seconds or more have passed.

Friend 1 arrived.
Friend 2 arrived.
Friend 3 arrived.
Friend 4 arrived.
Friend 5 arrived.
Friend 6 arrived.
Friend 7 arrived.
Arguing done! Group 1 decided with 7 people
Friend 7 leaving in group 1
Friend 6 leaving in group 1
Friend 3 leaving in group 1
Friend 2 leaving in group 1
Friend 4 leaving in group 1
Friend 5 leaving in group 1
Friend 1 leaving in group 1

Everything finished...
```

### Test 2

```shell
-> % ./lunch.bin 2
Running test number 2: Threads arrive in two groups and should leave in those two groups.

Friend 1 arrived.
Friend 2 arrived.
Friend 3 arrived.
Friend 4 arrived.
Friend 5 arrived.
Arguing done! Group 1 decided with 5 people
Friend 5 leaving in group 1
Friend 1 leaving in group 1
Friend 2 leaving in group 1
Friend 3 leaving in group 1
Friend 4 leaving in group 1
Friend 6 arrived.
Friend 7 arrived.
Arguing done! Group 2 decided with 2 people
Friend 7 leaving in group 2
Friend 6 leaving in group 2

Everything finished...
```

### Test 3

```shell
-> % ./lunch.bin 3
Running test number 3: Threads arrive with shorty durations in between, they should be leaving in groups that are less than 2 seconds apart.

Friend 1 arrived.
Friend 2 arrived.
Arguing done! Group 1 decided with 2 people
Friend 2 leaving in group 1
Friend 1 leaving in group 1
Friend 3 arrived.
Friend 4 arrived.
Arguing done! Group 2 decided with 2 people
Friend 4 leaving in group 2
Friend 3 leaving in group 2
Friend 5 arrived.
Friend 6 arrived.
Friend 7 arrived.
Arguing done! Group 3 decided with 3 people
Friend 7 leaving in group 3
Friend 5 leaving in group 3
Friend 6 leaving in group 3

Everything finished...
```

### Test 4

```shell
-> % ./lunch.bin 4
Running test number 4: Similar to test 3, but now threads in first group cut it close and break up, last group definitely breaks up.

Friend 1 arrived.
Arguing done! Group 1 decided with 1 people
Friend 1 leaving in group 1
Friend 2 arrived.
Arguing done! Group 2 decided with 1 people
Friend 2 leaving in group 2
Friend 3 arrived.
Friend 4 arrived.
Arguing done! Group 3 decided with 2 people
Friend 4 leaving in group 3
Friend 3 leaving in group 3
Friend 5 arrived.
Arguing done! Group 4 decided with 1 people
Friend 5 leaving in group 4
Friend 6 arrived.
Arguing done! Group 5 decided with 1 people
Friend 6 leaving in group 5
Friend 7 arrived.
Arguing done! Group 6 decided with 1 people
Friend 7 leaving in group 6

Everything finished...
```

# Submission:

To generate your files for submission, run the `make submit` rule, it should
look something like this:

```shell
-> % make submit
 Cleaning up submission directory 
make clean
make[1]: Entering directory '/removed/for/privacy/'
rm -rf *.bin *.o
make[1]: Leaving directory '/removed/for/privacy/'

 Generating tarball of modified files 
tar cvf makeup_exam2.tar lunch.c game.c critical.c threadpoolc
lunch.c
game.c
critical.c
threadpool.c

 Generating patch file as backup 
git diff > makeup_exam2.patch

 Please do not forget to submit to Gradescope 
```

Submit your tarball and your patch file to Gradescope.

``` {=html}
<style>
body { min-width: 60% !important; }
</style>
```

