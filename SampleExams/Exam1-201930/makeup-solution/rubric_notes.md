# Step 1: Makefile 10/10

4 points - can build slow\_sender, rebuilds when slow\_sender.c changes
3 points - build .o, only rebuilds each .o when needed
3 points - correctly rebuilds everything when .h changes

-2 executable rules named wrong so it won't detect they're built
-3 Does not handle rebuilds correctly for changes to common.h
-1 incorrectly makes executable depend on both .c and .o

**NOTE**: be sure that even if the makefile does not exist, you
compile slow_sender by hand or it will cause other things

# Step 2: Running in parallel 20/20

- check that children are run in parallel and all started before the
  parent moves on.

# Step 3: Exec the sender 20/20

- check that exec is called properly
- check that the parrent waits for all of the children to complete

# Step 4: Collect benchmark data 15/15

- check that an extra fork is added to collect the benchmark data
- check that a wait is used to ensure that the sender is done before
  collecting the time

-5 for extra layers of fork

# Step 5: Exec the receiver 10/10

- check that an additional fork and exec are used to run the receiver
- check that a wait is used to ensure that the receiver is done before
  collecting the time
- check that sender and receiver run in parallel

# Step 6: Shared memory and dealing with failures 15/15

- check that shared memory is set up correctly before any children are
  forked
- check that the status of the sender and receiver are collected
- check that the approprate value is stored in an appropriate location
  in the shared memory
- check that the parent prints the minimum positive result

-2 doesn't use proper macros to check exit status

# Step 7: Create a pipe and pass the file descriptor to the sender and receiver 10/10

- creates pipe before forking process for sender and receiver
- closes appropriate end of the pipe in sender and receiver
- closes both ends of the pipe in the parent after the sender and
  receiver are started
- pass correct argument to execed proccesses