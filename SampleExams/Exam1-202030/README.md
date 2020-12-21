
In this system we are going to imagine a system thats wants to process
tasks as quickly as possible.  A master program boss.bin (which you will
mostly write) spawns subprocesses called workers.  The workers will
repeatedly run a different program called task.bin.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Makefile](#makefile)
    - [Valgrind output (7.5 points)](#valgrind-output-75-points)
    - [Cool Library (7.5 points)](#cool-library-75-points)
- [Part 1 (25 points)](#part-1-25-points)
- [Part 2 (20 points)](#part-2-20-points)
- [Part 3 (20 points)](#part-3-20-points)
- [Part 4 (10 points)](#part-4-10-points)
- [Part 5 (5 points)](#part-5-5-points)
- [Part 6 (5 points)](#part-6-5-points)

<!-- markdown-toc end -->


# Makefile 

I've already provided a Makefile that will build worker.bin and
boss.bin for you.  We've also created an all rule that builds
everything.

You'll add some "nice to have" features.

## Valgrind output (7.5 points)

Imagine we're concerned about memory leaking in task.bin.  As a
result, everytime task.bin is updated we always want to update
task_bin_mem_report.txt which shows the valgrind results for running
task.bin.  This report can be generated manually like this:

    valgrind ./task.bin 2> task_bin_mem_report.txt

This report should be re-created if and only if task.bin has changed
since the report was last created.

Make the Makefile produce this report.  Using "make
task_bin_mem_report.txt" or "make all" should ensure the report is up
to date.

HINT: This will look very much like the rules already in the given
Makefile.  We're just outputing task_bin_mem_report.txt rather than
task.bin or boss.bin.

## Cool Library (7.5 points)

Imagine that task.c and boss.c included coollibrary.h and their build
depended on linking with the code in coollibrary.c.  This isn't true -
there's only one function in coolibrary and it isn't being used.  But
still - modify the Makefile appropriately as if it were.

For full credit:

- task.c coollibrary.c boss.c should all be individually compiled
  using the -c gcc flag.

- the .o files produced should then be linked with gcc into task.bin
  and boss.bin

- the dependencies should be specified correctly so that no unneeded
  files are rebuilt (HINT: .bin files should only depend on .os - .o
  files should depend on .c and .h files)

- editing a .h file should cause the code that uses that .h to be
  rebuilt

# Part 1 (25 points)

Have use fork to have the boss process start WORK_NUM worker processes
in paralell.  The worker processes should never finish - they should
just print on startup and infinite loop forever.  Simiarly the boss
process should never finish.  You'll need to press ^C to exit.

    boss process (pid 2453) started
    worker process (pid 2454) started
    worker process (pid 2455) started
    worker process (pid 2456) started
    ^C

Note that your process ids will not match mine.

# Part 2 (20 points)

Use exec to have the workers run task.bin.  At this stage it's ok if
that makes your workers exit.  However, the boss should not exit even
if all its workers finish.

    boss process (pid 2730) started
    worker process (pid 2731) started
    worker process (pid 2732) started
    worker process (pid 2733) started
    task started
    task started
    task started
    task finished
    task finished
    task finished
    ^C
    
# Part 3 (20 points)

Now change the workers so that once they run task.bin in a separate
process, and once that process finished run task.bin again...forever.
Use an additional fork to accomplish this.

HINT: Make sure your works use wait to ensure the previous task is
complete before they start a new task.  Otherwise you can start a
massive number of processes that can overwhelm and crash your system.

    boss process (pid 4847) started
    worker process (pid 4848) started
    worker process (pid 4849) started
    worker process (pid 4851) started
    task started
    task started
    task started
    task finished
    task finished
    task finished
    task started
    task started
    task started
    task finished
    task finished
    task finished
    task started
    task started
    task started
    ^C

# Part 4 (10 points)

Turns out under some conditions task.bin will fail, returning with
exit code 99 (uncomment the line of task.c and recompile to make this
true).  If this occurs it is no longer safe for the worker that
started the task to continue running so the worker should terminate
itself as well.

Note that the boss should continue to run, even if all its workers
terminate.

    boss process (pid 5671) started
    worker process (pid 5672) started
    worker process (pid 5673) started
    worker process (pid 5675) started
    task started
    task started
    task.bin crashed with exit 99. worker (pid 5673) aborting
    task started
    task finished
    task started
    task finished
    task started
    task.bin crashed with exit 99. worker (pid 5672) aborting
    task finished
    task started
    task finished
    task started
    task finished
    task started
    task.bin crashed with exit 99. worker (pid 5675) aborting

Note that after that last worker aborts, no more task started will
print because all the workers are dead.

# Part 5 (5 points)

Now make it so that when a worker dies, the boss starts a new worker
to replace them.  One easy way to do this is with a signal handler for
the SIGCHLD signal, though it is not the only possible solution (other
approaches do complicate what you need to do for the next part
though).

HINT: Although you want the child signal handler installed on the
boss, you don't want it installed on the workers.  A boss's child
terminating is a crash (bad, needs special actions).  A worker's child
terminating is just business as usual.  The easiest way to do this is
to register the handler in the boss, then have the workers register a
different do-nothing handler (or SIG_IGN) when then start.

HINT: If you were unable to get Part 4 working so your workers don't
die, you can just make it so your workers only run a certain number of
tasks before quitting.

    boss process (pid 6224) started
    worker process (pid 6225) started
    worker process (pid 6226) started
    task started
    worker process (pid 6228) started
    task started
    task started
    task finished
    task finished
    task started
    task started
    task.bin crashed with exit 99. worker (pid 6226) aborting
    worker process (pid 6233) started
    task started
    task.bin crashed with exit 99. worker (pid 6233) aborting
    worker process (pid 6235) started
    task started
    task finished
    ^C

# Part 6 (5 points)

Make the boss print out a running count of the number of tasks
completed.  You'll should use shared memory for this.

For full credit, the updated count should print out every time a task
finishes.  For partial credit, it can just print out occasionally (maybe
every time a child crashes?).

NOTE: For full credit, the boss must do the task printing not the
children.  However, I'll only take of 1 point if you have the children
print.

You do NOT have to worry about the case where two children finish at
the exact same time and therefore don't update the count correctly.

    boss process (pid 6470) started
    worker process (pid 6471) started
    worker process (pid 6472) started
    worker process (pid 6474) started
    task started
    task started
    task started
    task.bin crashed with exit 99. worker (pid 6474) aborting
    signal handler
    worker process (pid 6477) started
    task started
    task finished
    task finished
    1 tasks finished
    2 tasks finished
    task started
    task started
    task finished
    3 tasks finished
    ^C 
