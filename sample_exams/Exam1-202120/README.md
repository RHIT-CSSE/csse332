
**Table of Contents**
- [Makefile  (15 points)](#makefile--15-points)
- [Part 1 (20 points)](#part-1-20-points)
- [Part 2 (15 points)](#part-2-15-points)
- [Part 3 (10 points)](#part-3-10-points)
- [Part 3 (20 points)](#part-3-20-points)
- [Part 4 (10 points)](#part-4-10-points)
- [Part 5 (10 points)](#part-5-10-points)

In this code, we will create a *shell* program to simulate the workflow of
a restaurant, including making order, cooking, and delivering.

Each of these steps (i.e., order, cook, and deliver) can be finished by executing a program with an order
number. For example, to make an order #1, you can first compile `order.c` by
running `gcc -ggdb order.c -o order.bin`. Then by running `./order.bin 1`, you can
make an order.

Overall, our goal is to make shell take command input from keyboard and then
run the external programs (i.e., order, cook, deliver) sequentially to finish
food orders.

# Makefile  (15 points)

I've already provided you a Makefile that includes an *all* rule that builds
everything as well as the *clean* rule to remove all `.o` and executables.

In this task, you need complete the `Makefile` to generate all executables
needed for this task, including `shell.bin`, `order.bin`, `cook.bin`, and
`deliver.bin`. In addition, your Makefile should satisfy the following
requirements:

* All the `.c` files should all be individually compiled into `.o` files by
using the `-c` gcc flag.

* Then you need to link `.o` files to generate corresponding executable files,
i.e., `.bin` files.

* The dependencies should be specified correctly so that no unneeded files are
rebuilt (HINT: `.bin` files should only depend on `.o`; The `.o` files should
depend on `.c` and `.h` files)

* Editing the `.h` file should cause the code that uses that `.h` to be rebuilt

In the following parts, you need to add code to `shell.c` to implement
various functionalities specified in each part.

# Part 1 (20 points)

**Order**: In this part, you need to add code in `shell.c` so that the shell
can take order *in a parallel manner*. When receiving a command **O**
(for ordering), the shell will create an order with an order number (starting
from **1**). Specifically, the shell can take multiple orders simultaneously.
Namely, you need to create *background* processes to handle each order.

> HINTS:

  * You need both `fork()` and `execlp` (or other `exec` functions) to
do this. When using `exec`, don't forget to include `./` in the file path when
you execute certain programs in this directory.
  * To feed the order number as the input argument to the programs, you need to
  convert an int number to string. You can use `sprintf` as the example shown
  below:
    ```
    char buff[10];
    sprintf(buff, "%d", order_num);
    ```

The expected output is displayed below if you quickly type *three* **O**
commands (i.e., **O** + enter three times):

```
$ ./shell.bin
O
Order 1 received
O
Order 2 received
O
Order 3 received
Order 1 processed
Order 2 processed
Order 3 processed
^C
$
```

The dollar sign is the command line prompt from the terminal. Yours may vary.
`^C` is the keyboard input `ctrl+c` that is used to quit the program.

# Part 2 (15 points)

**Cook**: Once an order is processed, it needs to be cooked immediately.
Namely, for a certain order, you need to call the `./cook.bin` program with
the order number after `./order.bin` program finishes.

The expected output is displayed below if you quickly type *three* **O**
commands (i.e., **O** + enter three times):

```txt
$ ./shell.bin
O
Order 1 received
O
Order 2 received
O
Order 3 received
Order 1 processed
Start COOKING order 1
Order 2 processed
Start COOKING order 2
Order 3 processed
Start COOKING order 3
Finish COOKING order 1
FAILED COOKING order 2
Finish COOKING order 3
^C
$
```

Note that the order 2 failed which is expected.

# Part 3 (10 points)

**Maximum Order Number**: It turned out this is one of the luxury "stuck-up"
restaurants (say Michelin N-star restaurants) that only takes **3** orders
each day. We can simulate this behavior in the shell by limiting the number of
command **O** you can type in. Namely, if you type more 3 command **O**, the
shell will automatically terminate.

When the system is terminated in this situation, you need to print out this
message `Accept No More Orders. Closing system now.` (end with `\n`).

The expected output is displayed below if you quickly type *four* **O**
commands (i.e., **O** + enter four times):

```
$ ./shell.bin
O
Order 1 received
O
Order 2 received
O
Order 3 received
O
Accept No More Orders. Closing system now.
$ Order 1 processed
Start COOKING order 1
Order 2 processed
Start COOKING order 2
Order 3 processed
Start COOKING order 3
Finish COOKING order 1
FAILED COOKING order 2
Finish COOKING order 3
```

Note the program quit right after the fourth *O* typed in. Notice the
terminal prompt (i.e., the dollar sign) is printed after the system closing
message. The rest of the lines are printed after the program is terminated.

# Part 3 (20 points)

**Abort Failed Orders**: If an order is failed to cook, you need to abort
the order (i.e., terminate the background process that handles this order)
and print out the error message - `Order N is aborted.\n` (`N` is the order
number).

Hint: the program `cook.bin` will exit with different number to
indicate whether or not an order is successfully cooked (check `cook.c` for
more details).

The expected output is displayed below if you quickly type *three* **O**
commands (i.e., **O** + enter three times):
```
$ ./shell.bin
O
Order 1 received
O
Order 2 received
O
Order 3 received
Order 1 processed
Start COOKING order 1
Order 2 processed
Start COOKING order 2
Order 3 processed
Start COOKING order 3
Finish COOKING order 1
FAILED COOKING order 2
Order 2 is aborted.
Finish COOKING order 3
^C
$
```

# Part 4 (10 points)

**Abort Slow Orders**: If an order takes too long to cook, you also need to
abort this order. If any order takes more than *four* seconds to cook, you
need to abort the order and print out the message: `Takes too long to cook. Order N is aborted.\n`
(N is the order number).

We need to use `signals` to implement this feature. Basically,
we need to time the runtime of a program. Using `signal` to timeout a function
run is easy but it is more tricky to do this with a program. Here are some
hints:

* You certainly need to `fork()` so that the child can be the program you want
to time.
* You can time the *wait* time of the parent in order to measure/timeout the
runtime of the child. If the child program takes too long to come back, the
alarm will be triggered. However, if the child doesn't run over the time
limit (4 seconds), you can call `signal(SIGALRM, SIG_IGN)` to cancel the
alarm.
* You only set up the alarm in one process so you won't print out the message
multiple times.
* To use the order number in the signal handler, you want to set the order
number variable as a global variable.

The expected output is displayed below if you quickly type *three* **O**
commands (i.e., **O** + enter three times):
```
$ ./shell.bin
O
Order 1 received
O
Order 2 received
O
Order 3 received
Order 1 processed
Start COOKING order 1
Order 2 processed
Start COOKING order 2
Order 3 processed
Start COOKING order 3
Finish COOKING order 1
FAILED COOKING order 2
Order 2 aborted.
Takes too long to cook. Order 3 is aborted.
Finish COOKING order 3
```

# Part 5 (10 points)

**Deliver**: To deliver an order, you need to type another command **D** with
an order number (e.g., `D 1` to deliver order 1). The system will call
`./deliver.bin N` program after that order is cooked. In other words,
`./deliver.bin N` must be called 1) when receiving the command `D N` AND 2)
after `./cook.bin N` finishes. To implement this feature, we need to use IPC
(pipe) to talk with the background process for that particular order. Here is
how:

* You need to set up one pipe for each order, i.e., 3 pipes in our case.
* For each process that runs an order, you want to `read` from its
corresponding pipe after finishing cooking (You don't want to `read` if the
order is aborted.). In that case, the process will be blocked and wait for
any message from the pipe after finishing cooking.
* When receiving command `D N`, you need to use the `N` here to locate that
pipe and send some message (e.g., "GO" or something else) to the pipe. Once the
process that is waiting for message received this message from its pipe, it
will proceed to run the program `./deliver N`. (Again `N` is the order number)
* If you send messages to the terminated processes (aborted orders), the system
will crash. So technically, you can only call `D 1` without crashing the
program, because order 2 and 3 have been (or will be) aborted.

The expected output is displayed below:

```
$ ./shell.bin
O
Order 1 received
O
Order 2 received
O
Order 3 received
Order 1 processed
Start COOKING order 1
Order 2 processed
Start COOKING order 2
Order 3 processed
Start COOKING order 3
Finish COOKING order 1
FAILED COOKING order 2
Order 2 aborted.
Takes too long to cook. Order 3 aborted.
Finish COOKING order 3
D 1
Start delivering 1
Order 1 delivered

```

If you type `D 2` or `D 3`, the program will crash and immediately quit.
