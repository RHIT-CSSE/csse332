# Step 1: Makefile 10/10

4 points - can build slow\_gcc, rebuilds when slow\_gcc.c changes
3 points - build .os, only rebuilds each .o when needed
3 points - correctly rebuilds everything when .h changes

-2 executable rules named wrong so it won't detect they're built
-3 Does not handle rebuilds correctly for changes to handy.h
-1 incorrectly makes executable depend on both .c and .o

**NOTE**: be sure that even if the makefile does not exist, you
compile slow_gcc by hand or it will cause other things

# Step 2: Running in parallel (20/20)

Can be handy to set max_gccs = 6 if they've tried the last part

# Step 3: Actually Execing gcc (20/20)
# Step 4: Doing the final compile (15/15)

Be sure to check the .cs have been switched .os

# Step 5: Dealing with Failures (15/15)

It's fine if they let everything finish before checking for failures.
Also fine if it checks sooner.

-5 correctly detect failure, but does not abort the final build

# Step 6: Cleanup on Control-C (10/10)

You can change the exec to use echo to make things more obvious
There are lots of bugs that arise from non-direct children getting signaled
I gave full credit to a student who used remove rather than more forking

2 points - a signal handler that runs, regardless of the rest
2 points - signal handler has access to filenames, regardless of the rest
2 points - correctly either folks or executes all files at once
4 points - perfect (if the code looks right but we have non direct children feel free to give all but this)

2/10 for removing using a "*.o" (fairly sure this doesn't work)

# Step 7: Limiting the Number of Parallel Builds (10/10)

-3 waits in groups max_gcc rather than starting a new gcc as soon as one finishes 
