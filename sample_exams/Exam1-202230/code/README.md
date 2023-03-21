# CSSE332 Exam 1 212230

# Part 0: Makefiles (10 points)

In the directory makefile you can see the starting code for User Space
Threading 1 (slightly modified).

There are two different executables, one of which would be built like this:

    gcc basic_threads.c in_use_example.c -o in_use_example.bin
    
And one built like this:

    gcc basic_threads.c CuTest.c tests.c -o tests.bin

Note that although they will compile/run with the given commands, they
won't actually do anything interesting (i.e. any threading) because
this has empty functions in basic_threads.c.  However compiling to a
runnable state will be sufficient for our purposes.
    
* [5 points] Build a Makefile that compiles these two programs
  correctly when you type "make in_use_example.bin" and "make
  tests.bin".

* [5 points] ONLY FOR in_use_example.c.  Enhance the Makefile so that
  it incrementally builds .o files from each of the .c files and then
  links these .os into the final .bin.
  
DO NOT USE any features of Makefiles we did not discuss in class
(e.g. generic rules like %.o etc).  Please write this Makefile from
scratch just using the basics.

# Part 1: One Lucky Child (20 points)

So this is an exam where we will create "lucky" processes.  A process
is considered "lucky" if its PID ends in 7 (i.e. `getpid() % 10 ==
7`).  We want to create a child that is lucky and (in later steps) a
whole chain of lucky children.

Create a parent process that repeatedly forks children until a lucky
child is created.  Unlucky children should immediately exit.  Once a
lucky child is created, both parent and child should infinite loop.
Here's my example output (note that your process ids will be
different, but the final lucky child's process id should end in 7).

    $ ./lucky_nums.bin                  
    unlucky child pid 102891 exiting
    unlucky child pid 102892 exiting
    unlucky child pid 102893 exiting
    unlucky child pid 102894 exiting
    unlucky child pid 102895 exiting
    unlucky child pid 102896 exiting
    lucky child pid 102897 created
    child running forever
    parent 102890 thinks it found a good process 102897 
    parent running forever
    ^C

A few notes:

* Building this exam so is simple you don't even really need a Makefile:
        
        gcc -ggdb lucky_nums.c -o lucky_nums.bin

* In this example I included a print about unlucky children exiting to
  clarify what is going on.  You don't have to include this print in
  yours, and in future output examples I don't include this print
  because it makes things noisy

* The line about the parent thinks it found a good process is printed
  by the *parent* not the child.  Note that the parent can get its
  child pid as the non-zero return value from the call to `fork()`.  **For
  credit, you must have your parent print this line.**

* You should not allow your unlucky children to become zombies.  If
  the child is unlucky, the parent process should wait (if it is lucky
  though, it should not wait because lucky processes run forever)
  
To verify everything is working as expected, when your lucky processes
are running open a second terminal and type "pstree -p -c
\<parent-pid\>".

What you should see is something that looks like this:

    $ pstree -p -c 103627
    lucky_nums.bin(103627)───lucky_nums.bin(103637)
    
If `pstree` is not found in your case, you can use 
```shell
sudo apt install -y psmisc
```
to install it. 

Note that the child should be lucky (in this case, my parent is lucky
too but that's just a coincidence).

If you see a lot of unlucky children too, it probably means they are
not exiting or you are not waiting and they are ending up zombies.

# Part 2: Second lucky child (20 points)

You can do Part 2 or Part 3 first.  You can do Part 4 too, but it
makes less sense without Part 2 being done.

We now want the first lucky child to spawn a lucky child of its own.
Note this a little different than our usual threading model, where
usually we want the parent to spawn all the children but in this case
a child will spawn a child.

This should work very similar to the last stage.

    $ ./lucky_nums.bin 
    lucky child pid 104207 created
    child running forever
    lucky child pid 104217 created
    child running forever
    parent 104197 thinks it found a good process 104207 
    parent running forever

Notes:

* Child 104207 spawns child 104217 before it runs forever, but because
  it takes a little while to start up the lucky child pid 104217 lines
  appears after the run forever line
* the parent's good process is its direct child, not the child's child
  (this will be changed in later steps)
* I recommended you don't try to do this as a loop at this point and
  just hard code a lucky child that produces a lucky child

You'll want to ensure everything looks correct with `pstree` before
continuing.
    
    $ pstree -p -c 104197
    lucky_nums.bin(104197)───lucky_nums.bin(104207)───lucky_nums.bin(104217)

Note in particular that 104217 is a child of 104207 and not the parent.

# Part 3: Automatic pstree (20 points)

It's annoying to constantly have to check our work with `pstree` - if
only that could be done for us.

Before the parent runs forever, have it fork/exec `pstree -p -c` on the
parent pid.

    $ ./lucky_nums.bin                  
    lucky child pid 104647 created
    child running forever
    lucky child pid 104657 created
    child running forever
    parent 104644 thinks it found a good process 104647 
    lucky_nums.bin(104644)─┬─lucky_nums.bin(104647)───lucky_nums.bin(104657)
                           └─pstree(104658)
    parent running forever

Notes:

* you might want to `sleep(1)` in the parent before you invoke `pstree:`
  That gives the child's child enough time to spawn.

* be sure to pass -p -c and process id as three separate parameters,
  in your exec call otherwise they won't be interpreted correctly

* You'll have to convert the parents process id to a string to pass it as a
  parameter to `pstree`.  Here's how I do it:
```C
char num[100];
snprintf(num, 100, "%d", parent_pid); //now num is a string I can pass to exec
```

* get that `parent_pid` before you call fork, not after (why?)

* **don't let pstree become a zombie**

* If you need the full path for `pstree`, you can run `which pstree` from your
  terminal window.

# Part 4: Getting the last child pid (20 points)

We'd like the parent "thinks it found a good process" message to print
the pid of the last (deepest) child, not the direct child.  But how
can the parent know what pid that is?

Use either shared memory or pipes to have the last child communicate
its pid back to the parent.  Again that "parent XXXX thinks it found a
good process YYYY" message must be printed by the first original parent, not a
child.

    $ ./lucky_nums.bin
    lucky child pid 104847 created
    child running forever
    lucky child pid 104857 created
    child running forever
    parent 104837 thinks it found a good process 104857 
    lucky_nums.bin(104837)─┬─lucky_nums.bin(104847)───lucky_nums.bin(104857)
                           └─pstree(104858)
    parent running forever

Once you do this, you should no longer need to rely on sleep to ensure
the children are created before calling `pstree`.

*Hint:* To write an integer into a pipe, you can use one of two approaches, you
can write as a string and then use the `atoi` function at the other end to
convert it back into an integer. Or, you can write directly as an integer and
read it at the other end directly as a 32-bits integer. 

*Hint:* If you'd like to do it with shared memory, you'll probably
need to have a while loop where the parent waits for the final process
pid to be set before continuing.

If you'd like to attempt this part without succeeding on part 2, you
can.  Use shared memory/pipes rather than the return value of fork()
to get your child's pid.


# Part 5: Long lucky chains (5 points)

This part might bend your brain just slightly with its tricky forking.
I encourage you to only attempt this part if all other parts are done.
I also recommend backing up your c file now so you can submit
something even if you can't get this part to work.

Rather than have just one or two lucky children, we want to have
process chains of arbitrary length using a loop.  Modify your code so
that we produce a chain length controlled by the MAX_CHAIN variable.
Here's my output with `MAX_CHAIN = 5`.

    $ ./lucky_nums.bin                  
    lucky child pid 105287 created
    child running forever
    lucky child pid 105297 created
    child running forever
    lucky child pid 105307 created
    child running forever
    lucky child pid 105317 created
    child running forever
    parent 105280 thinks it found a good process 105317 
    lucky_nums.bin(105280)─┬─lucky_nums.bin(105287)───lucky_nums.bin(105297)───lucky_nums.bin(105307)───lucky_nums.bin(105317)
                           └─pstree(105318)
    parent running forever

# Submission

Submit your solution via gradescope.  You should submit:

    * lucky_nums.c
    * Makefile (from the makefile directory)

You may see a bug where you get a bunch of duplicate messages when
running in gradescope that you didn't get when running locally.  This
can happen when a process prints something and then soon after forks.
On my solution this happens with "lucky child pid" messages.

If this happens to you, simply add this line:

   fflush( stdout );
   
After the offending printf and your problem should go away.


