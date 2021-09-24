# Table of contents 
<!-- vim-markdown-toc GFM -->

* [Introduction](#introduction)
    * [Heating demons](#heating-demons)
    * [Cleanup demons](#cleanup-demons)
    * [Repair demons](#repair-demons)
    * [Helpful hint](#helpful-hint)
* [Part 0: Makefile (15 points)](#part-0-makefile-15-points)
* [Part 1: Summon repair operations (25 points)](#part-1-summon-repair-operations-25-points)
* [Part 2: Summon heating demons (15 points)](#part-2-summon-heating-demons-15-points)
* [Part 3: Summon cleanup demons (25 points)](#part-3-summon-cleanup-demons-25-points)
* [Part 4: Communicate repair status to Beezy (10 points)](#part-4-communicate-repair-status-to-beezy-10-points)
* [Part 5: Terminate heating demons to stop waste (10 points)](#part-5-terminate-heating-demons-to-stop-waste-10-points)

<!-- vim-markdown-toc -->

# Introduction
In this exam, we will be helping out Beelzebub, the Lord of The Flies, and the second in command to
non another than the red devil himself. Beelzebub (also known as Beezy) is hell's plant manager.
His role is the supervision and maintenance of hell's operation, especially the heating system (You
know, hell is all about heat).

One hot day, Beezy discovered that hell's heating system has malfunctioned. Hell was quickly loosing
temperature and the residents of hell were starting to complain (i.e., are becoming happy and cool).
We would like to help Beezy fix hell's HVAC by summoning and coordinating worker demons that will fix
the HVAC system, if at all possible.

Your job is to implement `beezy.c` that will fork demon children to act as repair-demons,
cleanup-demons, or heating-demons. Eventually, we would like our code to do the following:
1. Summon _repair demans_ as background processes that each will execute the file
   `repair_demon` (built from `repair_demon.c`). Repair demons (as they are low-level demons) can
   succeed or fail, so you will need to find a way to check whether they fixed their part of the
   HVAC or failed at doing that, and then report that to Beezy. The main way of communication in
   hell is __pipes__ (well, Beezy was on a budget and he wasn't able to get more modern
   communication devices).
    * If a repair demon fails, then we must summon a _cleanup_ demon to make sure that nothing gets
    left over uncleaned. In this case, we must wait for the cleaning demons to finish before reporting
    back to Beezy (Beezy is pretty particular about cleanup so he will be upset if we reported to him
    before cleanup).
1. Summon _heating demons_ that will try to blow heat into hell's vents to make sure the temperature
   stays as high as possible.
1. Beezy will then wait to read the status of repair demons from corresponding pipes. If more
   than half of the demons communicate the message `"SUCCESS"`, then Beezy will be happy and will kill
   all the heating demons by sending them the `TERMINATE` signal. The heating demons must catch Beezy's
   signal, print a message that they have been terminated, and disappear into non-existence. If less
   than half of them succeed, then Beezy will resign and quit as hell's plant manager. He will leave
   the heating demons to wreak havoc and won't care if they're orphaned.
 1. **Note that among all demons, only repair demons are external programs (implemented in
    `repair_demon.c`) while the heating and cleanup demons are function runs or simple code
    executions.**

Here is the breakdown of each job's description:

### Heating demons
In this exam, heating demons will do nothing except loop forever, i.e., something like `while(1);`.
However, we might need them to catch some signals, so be aware of that. DO NOT use `exec` for
implementing heating demons. You are to implement them using a couple lines of code.

### Cleanup demons
In this exam, cleanup demons do nothing except sleep for 3 seconds (`sleep(3)`) and then exit. You
are to implement them using a couple lines of code. 

### Repair demons
Repair demons must execute an external program located in `repair_demon.c`, which is independently compiled
into the `repair_demon` binary. By default, repair demons have a 50% chance of succeeding or
failing. However, you can override this behavior to test your code later on if you'd like. If you
pass the argument 1 to the repair demons, they will always succeed. If you pass the argument 2 to
the repair demons, they will always fail. For example,
```shell
$ ./repair_demon
Repair demon starting damned repair operations...
Repair demon failed the repair operation. Damn them to hell!
```
However, running it again might generate
```shell
$ ./repair_demon
Repair demon starting damned repair operations...
Repair demon succeeded in the repair operation. Promote to heaven?
```
If you'd like to guarantee success, then use
```shell
$ ./repair_demon 1
Repair demon starting damned repair operations...
	 Repair demon received state: always succeed!
Repair demon succeeded in the repair operation. Promote to heaven?
```
If you'd like to guarantee failure, then use
```shell
$ ./repair demon 2
Repair demon starting damned repair operations...
	 Repair demon received state: always fail!
Repair demon failed the repair operation. Damn them to hell!
```

### Helpful hint
If at any point in time you have orphaned children, you can use `ps -a` to check if there are any.
For example, here is the output from my shell (you might see other stuff as well)
```shell
$ ps -a
  PID TTY           TIME CMD
63527 ttys002    0:10.64 ./beezy
63530 ttys002    0:10.63 ./beezy
63552 ttys002    0:00.00 ps -a
```
To get rid of those demons, you can perform an exorcism by using the `pkill` command. `pkill` will
terminate any process that matches the name you pass to it, for example, here what I did
```shell
$ pkill beezy
$ ps -a
  PID TTY           TIME CMD
63740 ttys002    0:00.01 ps -a
```


---

**Let's get started. You can do parts 1 and 2 in any order you like. They are independent of each
other and it doesn't matter which one you do first.**

# Part 0: Makefile (15 points)
We have already provided you with a makefile that includes only an `all` rule that builds everything
and a `clean` rule that removes all `.o` files and binaries.

In this part, you need to complete the `Makefile` to generate all the executables (`beezy` and
`repair_demon`). Your `Makefile` must satisfy the following requirements:
* Compile everything correctly.
  * Please note that `beezy.c` depends on functions defined in `say_hello.h` and implemented in `say_hello.c`.
* All of the `.c` files should be compiled individually to generate object files (i.e., `.o` files)
using the `-c` flag in `gcc`.
* You will need to correctly link the object files (not `.c` files) to generate the corresponding
executables.
* You should correctly specify dependencies, such that no unneeded files are ever rebuilt.
* Your `Makefile` should trigger the correct operations when a `.h` file is changed.

# Part 1: Summon repair operations (25 points)
In this first part, we would like Beezy to summon repair operations. But recall that repair demons
might succeed or fail, so we will need to capture their exit state. To match my output and make sure
everything is running correctly, in your code, print `"JOB COMPLETED"` when the repair
demon succeeds, and `"JOB FAILED"` when the repair demon fails.

**NOTE that Beezy should be free to do other things after starting the repair operations, i.e.,
Beezy should not be waiting for anyone and is left to do any other business, or do nothing at all.**

> Note that in `beezy.c`, `MAX_REPAIR_DEMONS` represents the number of repair demons we would like
to summon.

> Hint: You might need to `fork` multiple times and then use any variant of the `exec` family.

> If you like to guarantee success or failures, then pass the argument **1** or **2** to your summoned
repair demon, respectively. To get an integer into a string, you can use the following
  ```c
  char buffer[10];
  int len = sprintf(buff, "%d", YOUR_ARGUMENT);
  buffer[len] = '\0';
  ```

When I implemented this part, here's what my output was (you might see other things, but those are
for your entertainment):
```shell
$ ./beezy
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
$ Repair demon failed the repair operation. Damn them to hell! <-- Beezy process died here the dollar sign is my shell
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
JOB FAILED
JOB FAILED
JOB FAILED
JOB FAILED
```

The above showed the output when all jobs fail, here is the output when all of them succeed.
```shell
$ ./beezy
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
$ Repair demon succeeded in the repair operation. Promote to heaven? <-- again the $ is my shell
Repair demon succeeded in the repair operation. Promote to heaven?
Repair demon succeeded in the repair operation. Promote to heaven?
Repair demon succeeded in the repair operation. Promote to heaven?
JOB COMPLETED
JOB COMPLETED
JOB COMPLETED
JOB COMPLETED
```

# Part 2: Summon heating demons (15 points)
In this second part, we will help Beezy summon heating demons. Note that heating demons do nothing
except loop forever (we will add only one other functionality in the last part). You can use
something like
```c
while(1);
```
to achieve an unending loop. The heating demons should run in the background and should not
interfere with Beezy's business at all. **Note** that it might be helpful for Beezy to remember the
process ids for each of the heating demons.

> Note that in `beezy.c`, `MAX_HEATING_DEMONS` represents the number of heating demons we would like
to summon.

To match my output, print the following statement before
you loop forever: `"Heating demon looping forever"`.

If you started with Part 2 before Part 1, here's what I had:
```shell
$ ./beezy
Heating demon looping forever
Heating demon looping forever
```

If you started Part 1 before Part 2, here's what I had:
```shell
$ ./beezy
Heating demon looping forever
Heating demon looping forever
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
$ Repair demon succeeded in the repair operation. Promote to heaven? <-- Again, the shell here!
Repair demon succeeded in the repair operation. Promote to heaven?
JOB COMPLETED
JOB COMPLETED
Repair demon succeeded in the repair operation. Promote to heaven?
JOB COMPLETED
Repair demon succeeded in the repair operation. Promote to heaven?
JOB COMPLETED
```
At this time, you will have orphaned processes that are simply looping forever. See the note in the
introduction to see how to detect and then terminate these orphan demons.

# Part 3: Summon cleanup demons (25 points)
Depending on whether repair demons succeed or fail, we must perform cleanup
operations. Modify your code from Part 1 to summon cleanup demons whenever a repair demon fails.
Your code should check the status of a repair demon after it finishes, if it was a failure (check
`repair_demon.c` for appropriate values), then it must summon a cleanup demon and wait for it to
finish.

We will simulate a cleanup demon by sleeping for 3 seconds. To do so and match my expected output,
you can use the following code for the cleanup demon
```c
printf("Triggering a cleanup demon\n");
sleep(3);
exit(0);
```

When I implemented this part (after part 2), here's the output I got:
```shell
$ ./beezy
Heating demon looping forever
Heating demon looping forever
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
$ Repair demon failed the repair operation. Damn them to hell! <-- again, beezy exited here
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
JOB FAILED
JOB FAILED
JOB FAILED
JOB FAILED
Triggering a cleanup demon
Triggering a cleanup demon
Triggering a cleanup demon
Triggering a cleanup demon
```

# Part 4: Communicate repair status to Beezy (10 points)
In this part, we will need to communicate the status of repair demons to Beezy (after cleanup if we
needed to perform cleanup operations).  Use pipes to tell Beezy whether the repair task succeeded or
failed. You **MUST** write `FAILURE` if the repair demon fails (after summoning a
cleaning demon and waiting for it), and `SUCCESS` if the repair demon succeeds in its operation. Beezy
will count the number of successful operations and print that number out.

Note that Beezy must read the status from **EVERY** repair operation **after** the repair operations
have completed all their tasks. Be careful to only read from the pipe that corresponds to each
repair operation you are waiting for.

> Note that you can implement this using a single pipe, but it will be a pain, so I recommend
against it. But feel free to do that if you are comfortable with it.

In my implementation, I used `Beezy recorded %d succesful operations` as my string message to print,
and here is my output:
```shell
$ ./beezy
Heating demon looping forever
Heating demon looping forever
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
JOB FAILED
JOB FAILED
JOB FAILED
JOB FAILED
Triggering a cleanup demon
Triggering a cleanup demon
Triggering a cleanup demon
Triggering a cleanup demon
Beezy recorded 0 successful operations
```

In the case where some or all repairs are successful, you should see
```
$ ./beezy
Heating demon looping forever
Heating demon looping forever
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon succeeded in the repair operation. Promote to heaven?
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
JOB COMPLETED
JOB FAILED
JOB FAILED
JOB FAILED
Triggering a cleanup demon
Triggering a cleanup demon
Triggering a cleanup demon
Beezy recorded 1 successful operations
```

# Part 5: Terminate heating demons to stop waste (10 points)
In this last part, Beezy will try to be energy efficient whenever possible. If the number of
successful repair operations is greater than or equal to half of the total number of repair operations, then
Beezy will be happy. He will use the `SIGTERM` signal to kill off all of the heating demons.
Beezy will also print `Beezy is happy, everything went well, hell is back in business` to the
console.

You heating demons **MUST** catch beezy's signal and print `Heating demon killed.... OUCH!` on a line
before completely exiting.

To send a signal to another process, use the `kill` system call as follows: To terminate heating
demon with `pid` of `111`, use
```c
kill(111, SIGTERM);
waitpid(111, NULL, 0);
```

If less than half of the repair operations succeed, then Beezy will be upset and will quit his job.
In that case, he will leave the heating demons behind (and they will become orphans). In this case,
ask Beezy to print `Beezy is giving up, here is my resignation letter` before completely exiting
(regardless of what happens to heating demons). You can check if your heating demons have been
orphaned using `ps -a` as we mentioned in the helpful hints.

Here's an example of what you should see if at least 2 of the repair jobs are successful
```shell
$ ./beezy
Heating demon looping forever
Heating demon looping forever
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon starting damned repair operations...
Repair demon failed the repair operation. Damn them to hell!
Repair demon failed the repair operation. Damn them to hell!
Repair demon succeeded in the repair operation. Promote to heaven?
JOB FAILED
JOB FAILED
JOB COMPLETED
Triggering a cleanup demon
Triggering a cleanup demon
Repair demon succeeded in the repair operation. Promote to heaven?
JOB COMPLETED
Beezy recorded 2 successful operations
Beezy is happy, everything went well, hell is back in business
Heating demon killed.... OUCH!
Heating demon killed.... OUCH!
```
Check `ps -a` and you should see no orphan processes. If you see any and there were 2 or more
successful repairs, then your code must be incorrect.
