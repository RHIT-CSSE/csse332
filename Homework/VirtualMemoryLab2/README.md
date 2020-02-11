# VM2: Implementing fork

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [VM2: Implementing fork](#vm2-implementing-fork)
- [Starting point](#starting-point)
- [Step 0: Getting the given code to work](#step-0-getting-the-given-code-to-work)
- [Step 1: A Universal Memory Region](#step-1-a-universal-memory-region)
    - [The Theory](#the-theory)
    - [How can we do this?](#how-can-we-do-this)
    - [Step 1 Implementation](#step-1-implementation)
        - [Allocate a giant region of memory](#allocate-a-giant-region-of-memory)
        - [Switch between the memory regions](#switch-between-the-memory-regions)
- [Step 2: Saving memory again, but better](#step-2-saving-memory-again-but-better)
- [Step 3: Basic forking](#step-3-basic-forking)
- [Step 4: Fork Output](#step-4-fork-output)
- [Step 5: Copy on Write](#step-5-copy-on-write)
- [Submitting](#submitting)
- [Rubric](#rubric)

<!-- markdown-toc end -->


# Starting point

This assignment begins with code that's relatively similar to the
starting point of VM1.  The only difference is that this version
allows running multiple forth instances.  Only one forth can run at
once, but similar to the userspace threading lab we could easily
simulate parallelism by swapping between the various forths.  For
testing convenience though, we'll individually execute the various
forth instances manually.  Here's an example of how the API works (and
note that this NEARLY works correctly in the given code):

    initialize_forths();
    int pid = create_forth(".\" hello\" ");
    int pid2 = create_forth(".\" hello2\" ");
    struct run_output result = run_forth_until_event(pid);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "hello", result.output);
    result = run_forth_until_event(pid2);
    CuAssertIntEquals(tc, FCONTINUE_INPUT_DONE, result.result_code);
    CuAssertStrEquals(tc, "hello2", result.output);

When you call "create\_forth" it creates a new forth virtual machine
and returns a pid which is the id you'll use to access that machine.
You pass create\_forth the forth code you want it to run (to
understand the above forth code, realize that ." Hello World" is
forth's equivalent of hello world).  However, creating an forth
instance doesn't actually run the code.

When you call "run\_forth\_until\_event", it actually executes the
forth instance with the given pid.  The code runs until an "event"
happens: completion (FORTH\_INPUT\_DONE), error, or one of a few
special commands pause the running (initially just YIELD, but
eventually FORK with your help).  If the forth YIELDs, you can run the
instance again by calling "run\_forth\_until\_event".

Look at the unit test carefully and make sure it makes sense before
you continue.

# Step 0: Getting the given code to work

The code you will be editing is in [forking_forth.c](forking_forth.c).
The test code is in [fork_tests.c](fork_tests.c).

So the given code doesn't work at all because it does not allocate
memory for the forths.  Where this should happen is in the function
create\_forth, towards the bottom of forking\_forth.c.

If you look at this function, you'll see that this my code assumes
that data for forth pages with be allocated at one fixed location in
memory called UNIVERSAL\_PAGE\_START.  This is necessary for forking
to work (as I'll explain below).

Use mmap as you did in the previous lab to allocate memory at that
pre-defined fixed location.  Pass flags MAP\_FIXED, MAP\_SHARED, and
MAP\_ANONYMOUS to mmap (as in the last lab, we'll start with anonymous
mmaps and then transition to using files a little later).

If you do so, you should find that the first unit test should run.
Edit forth\_tests.c so that only test0\_oneforth is included and then:

    make
    ./fork_tests

# Step 1: A Universal Memory Region

## The Theory

Our goal in this lab is to give our forth instances the ability to
fork - the first thing to understand is that forking can't exist
efficiently without virtual memory.

When we fork we create two programs from one - but a running program
has numerous pointers to its own memory space.  If we copy the program
to a different memory region, all those pointers are pointing into the
original memory region.  Without virtual memory we have 2 pretty bad
options:

1.  Leave the programs in two regions but then copy them back into the
    original memory region when its time to run.  Extremely expensive
    because it means a massive copy every time we have to swap
    processes.
2.  Iterate across the child's memory space and re-write all pointers
    to point to the new location.  This is expensive and also error
    prone (i.e. if you miss something or accidentally rewrite
    something that's not really a pointer value after all).

With virtual memory though, we have a much better solution:

Copy the physical memory to from old location to a new location, but
ensure that when the program runs its virtual addresses (which have
been left unchanged by the copy) now map to the new region in physical
memory rather than the old.

This is the approach we're going to take with our forth
implementation.  We're going to have what I'll call the "universal"
memory region.  Each forth will be initialized to operate in the same
12 pages of our programs address space.  However, that universal
region will share physical memory with some other part of our
program's memory space (i.e. we will have two parts of our virtual
memory space, both of which correspond to the same physical memory and
therefore contain the same data).  Before we switch forth instances,
we will unmap the universal region and remap it to some other region.
Each forth when it executes will use pointers in the universal region,
but what other region is updated at the same time will vary depending
on which forth we are running.

## How can we do this?

So if you have access to the page table, it's trivial to have 2 pages
that point to the same frame which will produce the behavior we want.
But as an ordinary program we can't edit the page table and
malloc/mmap do not seem to have this feature.

Turns out there's an undocumented feature of mmap we can use.  If the
same file is mmaped twice with overlapping offsets, the overlapping
region will be setup to correspond to the same physical memory pages.
For example:

    mmap(reg1ptr, 
         getpagesize(), 
         PROT_READ | PROT_WRITE | PROT_EXEC,
         MAP_FIXED | MAP_SHARED, 
         fd, 
         myoffset);

    mmap(reg2ptr, 
         getpagesize(), 
         PROT_READ | PROT_WRITE | PROT_EXEC,
         MAP_FIXED | MAP_SHARED, 
         fd, 
         myoffset);

Will make 2 regions reg1ptr and reg2ptr that both correspond to the
same frame in physical memory.  This will also correspond to offset
myoffset on whatever file fd points to, but that is not as important.

To see a more detailed (runnable) example of this principle, check out
[mmap_twice_example.c](mmap_twice_example.c).  I strongly recommend
you fully understand that code before moving on.

This page https://nullprogram.com/blog/2016/04/10/ has some more
details on this trick, including a way to do it without an actual file
being created, but that's beyond what we need.

## Step 1 Implementation

The code we wrote for step 0 works but it only works with 1 forth at a
time.  test1\_twoforths and test2\_yields are the tests you'll want to use - right now test1 passes (though actually there a big problems there, the test doesn't detect them) but test2 SEGVs.

It should be obvious to you why: both forth instances are using the same universal memory region so they interfere with each other.  Obviously, we need each forth to have a separate NUM\_PAGES pages in memory.

### Allocate a giant region of memory

So to start with, let's allocate that separate memory.  Do this the first\_time part of initialize_forths.

Open a file (I named mine bigmem.dat) and use lseek/write to set it to
getpagesize()\*NUM_PAGES\*MAX\_FORTHS in size.  That's enough memory
for all the forths we might need to run.  Then mmap that whole file
into memory (doesn't matter where, except do NOT allocate it in
UNIVERSAL\_PAGE\_START region...practically just omit MAP\_FIXED and
pass NULL as the first parameter and mmap will choose for you).

BTW, for reasons that will become more clear in the next step, I call
this giant region of memory "frames".

### Switch between the memory regions

Now when we want to run some particular forth, we want to map some
part of our frames region into the universal region.

I wrote a function called switch\_current\_to(int forthnum) that does
this because we need it in a couple places.

The idea is exactly the same as the code above in the section "How can we do this?".  Our first mmap is already done in initialize\_forths.  Our second mmap is going to use the same file descriptor but it is going to be different in a couple of key ways:

1. Its going to map into a fixed position UNIVERSA\_PAGE\_START.  Don't forget MAP\_FIXED!
    
2. Its only going map a length for a single forth, not all the forths.

3. It is going to map at an offset position in the file (this is the last parameter of mmap we haven't used till now).  Forth 0 will have its memory at offset 0 in the file.  Forth 1 will have its memory at offset NUM\_PAGES*getpagesize(), etc.

4. You might think you need to can munmap before you call map, if something has already been mapped.  However it my testing it looks like a new mmap replaces the old.

Call this function two places:

1.  In create\_forth, just where your old create\_forth mmap was.  Create\_forth actually executes a ton of forth code to initialize the forth environment.

2. run\_forth\_until\_event, before the call to f\_run.


If you do this correctly, test1\_twoforths and test2\_yields should
pass.  BUT even more exciting, all your forths should think they are
operating in the same memory region.  If you want to prove to yourself
that this is really happening, you can print out some of the pointers
in the forth\_data struct (e.g. stack_top, here).  Even with different
forths, all these pointers should be in the single universal region.

# Step 2: Saving memory again, but better

In this step, we're going to implement the optimization that we did in
the first stage of VirtualMemoryLab1.  That is, we are only going to
give a particular forth a page of memory when it asks for it, by
generating a segfault.  You should go ahead and grab the code to catch
segmentation faults that you used in VM1.

Having multiple different forths causes a complication though.  How
should we store the various pages for the various forths?  We could
have a particular region for a particular forth instance (i.e. forth 0
stores its loaded pages in mem region a-b, forth 1 b-c, etc).  But
much easier is to have a single store for every forth page, in no
particular order.  We'll use the frames array for this.

Let me be more explicit:

Right now we sort of using frames as a region of NUM\_FORTHS chunks, each chunk is getpagesize()\*NUM\_PAGES long.  Forth 0 gets the first chunk, forth 1 gets the second, etc.  But this no longer makes sense because we don't actually want to allocate all that memory upfront.

Henceforth, we will use frames as a region of NUM\_FORTHS\*NUM\_PAGES
chunks, each getpagesize() long.  These individual page-size chunks
will very naturally correspond to how frames work in a regular virtual
memory system.  Forth 0 might use Frames 0 1 5 to correspond to its
pages 9 0 1.  Forth 1 might use frames 2 3 6 for its pages 9 0 1.
Note that the frames a particular forth uses aren't even contiguous.
When a forth request a new page (with a seg-fault) we will pick the
next unused frame (whatever number it is), and map that into the
universal memory region as whatever page is needed.

The first time a forth instance runs, we'll have no pages allocated in the universal region, and we'll map pages as we go.  When a forth
instance is swapped out and then back in, its memory space (the
universal region) needs to be recreated.  For that, we need to store
which pages are in which region for each forth instance.  We'll store
it in the forth\_extra\_data struct.

Instead, it will need to keep track of which frames in the giant chunk
correspond the 12 pages of the forth process.  So I made a integer
page\_table array.  Initially all entries are initialized to -1
meaning PAGE\_UNCREATED.  Then as pages are mapped from segfaults, I
insert their frame number at the corresponding slot in the current
forth's page\_table.  switch\_current\_to iterates across the
page\_table array, and maps the corresponding frame in the
giant chuck to the right region of the universal memory.  switch\_current\_to should also start with a big munmap for the entire universal region (otherwise, the we still have maped from the previous process won't generate segfaults and will instead be accessed/overwritten by the new process, causing all sorts of problems).

If you've coded this correctly, the test cases 0-2 will pass.  The
third test case uses a function called get\_used\_pages\_count to test
how many frames in the giant chunk are being used.  If you implement
that function, test case 3 should pass as well.

# Step 3: Basic forking

Now that we've got the basics down we can implement fork.  The basic
idea of fork is quite simple:

1.  When a process intends to fork, find a new fork slot to
    accommodate the child process and mark it as valid.
2.  Copy the contents of the forth\_data object from the parent into
    the child (of course this only works because of the universal
    memory region).  Hint: use memcpy so you don't have copy each
    field by hand.
3.  For each page mapped in the parent's page\_table, find a new
    frame, copy the data into it (again memcpy), and map that new
    frame in the child's page table
4.  Return the process id of the child in the forked\_child\_id field
    of the run\_object (take a look at run\_forth\_until\_event to
    see what I mean)
    
If you implement this correctly unit test 4 should pass.

# Step 4: Fork Output

As it currently stands, it's impossible for a forked process to know
if it's a parent or a child.  That's silly.  In our system, fork
should put 1 on the forth stack for a parent, and 0 on the stack for
the child.  To make this possible, there's a function called
push\_onto\_forth\_stack that adds an integer to a particular forth
stack.  Note: this function uses the stack_top variable in the forth
data object - for it to work properly you'll need to have that
particular forth mapped in the universal region.

If you do this correctly unit tests 5 and 6 should pass - 6 just
involves more forking.

# Step 5: Copy on Write

The final stage is to implement copy on write.  "Copy on write"
eliminates the need to do a copy the entire process's memory space
when a fork occurs.  Instead, both the parent and the child share the
same frame in their page tables.  They can share the memory forever as
long as neither parent nor child writes to it.  To ensure this
happens, the shared frame must be marked as read only.  When writing
is attempted into the region, it causes a segmentation fault (because
read only).  That segmentation fault is caught, the memory is copied
to the new region, that region is marked as writable, and the process
is allowed to continue.  See
[wikipedia](https://en.wikipedia.org/wiki/Copy-on-write) for some more
details if you're interested.

Note that in this system an arbitrary number of processes might share
a frame of memory (e.g. a process forks, and forks again while some of
its memory is still shared with its parent - now those frames are
shared with 3 processes).  We'll need to keep track of how many
processes are using each frame (I made a big array num\_shared for the
purpose).  When a page is initially allocated it should have a count
of 1.  When a process forks all its frame counts should be incremented.
When a write causes a frame to be copied the original's count should
be decremented.

When we map a process into memory during the switch, we'll use the
frame count.  If the frame's share count > 1, that means its a shared
page and should be mapped readonly (i.e. omit PROT\_WRITE from the
mmap).  If the frame's share count = 1, its unique to that process and
can be mapped as usual.

In our segmentation fault handler, if a segmentation fault occurs for
a page that we mapped but mapped readonly, it's causes by a write
attempt.  So we should unmap that particular section of the shared
memory region, copy the data in the readonly page to a new frame, put
that frame in the processes page table, map it in the shared region as
writeable, and continue the process.

Note that we won't worry about how to deallocate allocated pages in
this lab.  Our forth instances can always run more code, so its never
safe to deallocate their pages.

If you complete this step correctly, tests 7-9 should pass.

# Submitting

You're done.  Submit your forking_forth.c file.

# Rubric

|                                 | Points | Notes                                                                            |
|:--------------------------------|--------|----------------------------------------------------------------------------------|
| S1: Universal region            | 20     | we'll look at your code and make sure you really are using a single memory space |
| S2: Frames and a signal handler | 20     | again, use the approach described in the lab                                     |
| S3: Basic forking               | 20     |                                                                                  |
| S4: Output values               | 15     | should be pretty straightforward if you did the last step right                  |
| S5: Copy of write               | 25     |                                                                                  |

