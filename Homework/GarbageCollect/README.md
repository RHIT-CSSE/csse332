In this assignment, we build a simple garbage collector for the forth
programming language.

This assignment assumes you are familiar with the forth virtual
machine from the virtual memory assignment.  You shouldn't use your
code from that assignment (while it's not incompatible with what we're
doing here, it would just complicate things) but you should review the
introduction about forth and virtual machines in that assignment.

# Forth and Memory

Forth has a similar memory layout with 2 regions (actually 3, but
we're going to ignore the return stack for the purposes of this
assignment).  

## The forth stack

The first region is the stack.  Code like this

    1 2 + .
    
corresponds to

1. Push the number 1 onto the stack
2. Push the number 2 onto the stack
3. Pop the top two numbers off the stack, add them, and push the
   result on the stack
4. Pop the top number off the stack and display it

Unlike most modern languages which have individual stack data for each
function call, in forth all code uses the shared stack.  Functions
pull from the stack for their parameters, and push onto the stack for
their return values.  As they run, they also are likely to push and
pop temporary values on the stack but hopefully these temporary values
are cleaned up before functions return.

The fact that all local memory is stored in a single space will be
very handy to us as we try and compute what parts of memory are still
accessible.

## The heap

The second region is the heap.  A variable called HERE keeps track of
the next available space on the heap.  Adding new functions, values,
or calling ALLOT (forth's malloc equivalent) increments HERE.

However, forth does not have an equivalent of free.  If recently used
memory needs to be reclaimed it's possible to decrement HERE.  But as
multiple sections of code use the heap, certain parts of the heap may
be no longer in use yet there is no good way to reclaim that memory.

In this assignment, we intend to use garbage collection to identify
parts of the heap that are no longer accessible and then move memory
regions (and references to those regions) to compact the unused space.

# How Our Garbage Collector Will Operate

This section will explain the general approach and then Steps 1 - 3 below
will walk you through the process.

## Tracking allocated regions

To garbage collect, first we must have a idea of regions of memory.
In some languages "regions" might correspond to individual objects or
arrays.  Forth does not have such things so instead we will consider
each allocation (be it for function, variable, or just a raw call to
ALLOT) to refer to unique region.

We will annotate core forth functions to track regions and call
callbacks in C.  You will build an internal data structure that tracks
all currently allocated regions and where they start and end.

## Finding accessible regions

At some point, forth's execution will be paused and the garbage
collector will be explicitly invoked (this is in contrast to modern
garbage collectors which attempt to run in the background when
possible).  The first step will be to find all the regions that are
accessible (i.e. that are not garbage).  Initially there will be two
regions considered to be always accessible:

1. The forth stack
2. The variable forth.latest which is the head of a linked list of
   forth globals.  Whatever region it is currently pointing to is the
   mostly recently declared function/global.

The basic algorithm will be:

1. Intially mark all regions but the two above as inaccessible
2. If we have existing unsearched accessible regions, pick one
3. Search that region for pointers to other regions
4. For each pointer to another region found, mark that region as
   accessible and add it to the list of regions to be searched
5. Go back to step 2
6. If every accessible region has been searched all accessible
   regions are accounted for
    
Forth data is not typed, so looking at a particular chunk of data we
won't know if it refers to an integer, character, or pointer.  So
we're going to treat all data as potential pointers and if it points
to a valid region we'll consider that region accessible.  This may
mean occasionally we will treat an actually inaccessible region as
accessible because we misinterpret some random data as a pointer when
it's not.  Our algorithm has the potential for false positives.

Our algorithm also has the potential for false negatives when pointer
data is munged (either encoded in some non-obvious form, or perhaps
pointing just slightly outside the region but the code still uses it
correctly).  Because of these difficulties, most garbage collected
languages prohibit pointer arithmetic so pointers are always explicit.
Forth routinely uses pointer arithmetic so we're just going to have be
aware of the way garbage collection works in our test code or we can
cause memory corruption bugs.

## Collecting the Garbage

Once regions have been marked as accessible and inaccessible, we will
compact memory.  The basic algorithm will be:

1.  We are going to have a variable which represents the next unused
part of memory to write to.  Initially this will be initialized to the
start of forth's memory (this is represented by the variable stackheap
BTW).  Then, as we copy used regions into this space, we'll increment
this variable so it's always pointing to space that is safe for
writing.


2. For each accessible memory region, starting from low memory regions
   and going to high
   
   a. Copy the region into the next available space in writable memory
   
   b. Increment writable memory corresponding to the size of the
      region you're copying
   
This process will naturally overwrite all inaccessible regions

Once you finish moving all the memory, we must now go through all 
accessible memory and rewrite all pointers to refer to their new 
location.

# Step 1: Record all regions

Before we can begin to do garbage collection, we need keep track of
memory regions.  We can't mark regions as accessible/inaccessible
unless we know where they start and where they end.  

To do this, we'll have two callbacks: handle\_alloc\_begin() and
handle\_alloc\_end().  Begin will be called at the start of an
allocation and end will be called at the end of an allocation.

## How to store region information

You need to store information for each allocated region - at minimum
where it begins and ends.  How you store it is up to you, but I do
have some suggestions:

1.  I built a struct that I use as a region record called mem_rec.  It
    has 2 void pointers - a start and a end, plus some additional
    variables that I used for later parts of the project.  I malloced
    these structs whenever new regions were declared, and then I had
    to be careful to free them as they were no longer needed.
2.  I've included a basic C arraylist in the assignment directory
    which I used to keep track of the list of regions and easily
    add/remove regions as I needed to.  You might find it handy to use
    that.  Note that if you do use this, you have to allocate memory
    using its functions.  Take a look at [this chunk of example code I
    provided for you](al_example.c).
    
    On the other hand, if you prefer another structure (either
    one you write yourself or one your found on the internet) feel
    free.

## How the callbacks are called

The callbacks are configured in the function initialize\_forth\_for\_test().  
This function is called before each test case.  

Briefly, the way the system works is that it overrides several core
forth functions like ":" (function declaration) "VARIABLE" and
"ALLOT".  These functions work as before, but trigger a return to C
before they begin (calling handle\_alloc\_begin()) and when they end
(calling handle\_alloc\_end()).

## What do I need to do?

All of the tricky forth code that this section needs is implemented
for you.  What you need to do is:

1. Make some sort of global memory structure where you will record
   regions
2. Modify the beginning of initialize\_forth\_for\_test() and main to
   initialize that structure as you like
3. Add code in handle\_alloc\_begin() and handle\_alloc\_end() that
   adds to your region structure as you need.  To figure out the size
   of the regions, use the global variable forth.here.  When begin is
   called forth.here should refer to the beginning of the region, when
   end is called it refers to the end. (note: as is often the case, the
   beginning should be considered inclusive but the end is not
   inclusive)
4. Write the functions compute\_alloced\_size() and
   compute\_num\_regions().  These functions aren't really needed by
   our garbage collection algorithm, but they are used by the first
   test case.
   
## Testing

There is one testcase for this step: test\_region\_records.  Running

    make test
    
should run it.

# Step 2: Identify all inaccessible regions

In this step we'll be writing code to identify which regions are
accessible.  The basic approach is the one noted in "Finding
accessible regions" above but here are a few hints.

1. You'll probably want to add something to your memory rec data
   structure to note if a particular region has been marked as
   referenced.  Be sure you initialize all of those values to false as
   your start your algorithm.
2. The algorithm I describe above is a bit similar to shortest path
   algorithm for walking a graph breadth first.  If you like, you
   could also use recursion to solve this problem - just be aware that
   circular references are possible.
3. The 2 regions that start your algorithm are the stack and latest.
   To determine the currently accessible stack, it's the region
   between forth.stack\_top and forth.stack\_bot (note the stack
   starts high and grows to low so stack_top is the lower memory
   address).  Latest is forth.latest.
4. You'll almost certainly want to write a function that given a
   pointer, returns the region that it points to or null if it does
   not point to any real region.  You'll use it in several places.
5. When you're walking through memory looking for pointers, you should
   only increment your variable one byte at a time (because a pointer
   could start at any 8 byte block in the region).
   
   To increment a pointer by one by you can:
   
   a. Rely on the fact that gcc lets you increment void
   pointers by bytes using pointer math.  So my\_void\_pointer++
   works.
   b. You can also cast the void pointer to a char
   
   But then you'll want to treat that particular position as a 8 byte
   void pointer.  Here's the cast you'll need (where current is a
   pointer into the region you're walking across):
   
       void* potential_pointer = *((void**) current);

I reccommend you write a seperate function to identify inaccessable regions.
You can call it whatever you like but you'll need to invoke it from
compute\_unrefed\_size() which is what the test cases use (see below).

## Testing

All the tests in this section use the function
compute\_unrefed\_size() which returns the number of unreferenced
*bytes* (not the number of unrefed regions).  You'll also have to
write a function compute\_num\_regions() which is mostly just to
ensure your implementation is sane.

So you'll need to write that and make sure it calls your code that
marks unreferenced regions (but doesn't garbage collect).  Then
compute the total size in bytes of all unreferenced regions and return
it.

There are several tests for this part - you'll have to uncomment them
in main.  Once you do, make test should run everything. The tests
themselves have comments that indicate what they are doing.


# Step 3: Garbage collect

I've tried to simplify things by making a set of gradually more
difficult tests.  You'll need to uncomment them one at a time from
main.

## 3a: gc\_no\_pointer\_rewrite

So there are basically 2 things we need to do:

1. move everything to a new location
2. update all pointers to be point to their new locations

The first test only deals with #1.  Look at the algorithm described
in "Collecting the Garbage" to get started.  The function you'll be
editing is called gc_collect().

Here's a few hints:

1. You have to call your region marking code first to identify garbage
   regions.
2. Don't forget to remove unreferenced regions from your region list
3. The initial forth code allocates a ton of function regions but none
   of these regions need to move because no garbage will ever be
   allocated before them.  You probably want to detect this and not
   copy regions that aren't really moving. Also, as you get started, it can
   be good to print every time you move a region (and start and
   destination) and if you see a ton of moving you've probably messed
   things up.
4. Unless you do weird things to the ordering of your region list, the
   list should naturally be in the order of low to high.  It's
   important that you do the move in that order, otherwise moving
   later things will corrupt the memory-space of yet-unmoved earlier
   things.
5. Update forth.here when you're all done or your newly reclaimed
   memory won't be usable.

## 3b: gc\_stack\_update

The next test requires you to rewrite pointers but only pointers that
are on forth's stack.

A few hints:

1. You'll want to store both the old and the new positions of each
   region as you move.  The old positions will let you detect pointers
   that need rewriting - the new positions tell you how to rewrite.  I
   added fields to my mem_rec to do this.
2. Each element of the stack is 8 bytes long - so you can assume
   pointers on the stack always start on 8 byte boundaries.
3. Once everything's finished, be sure your regions fields fully
   reflect their new positions - garbage collection can happen
   multiple times
   
## 3c: gc\_internal\_data\_update

For this test, you must also update pointers within the memory
regions.

1. Note that you want to figure out the new location of each region
   THEN rewrite all the pointers.  Because circular references can
   exist, there's no way to rewrite as you copy (unless you initially
   compute all destinations, then actually begin copying with all
   destinations known).


## 3d: gc\_unaligned\_data\_update

If you did 3c right, this might immediately work.  If you assumed that
pointers occur on 8-byte boundaries like the do on the forth stack,
you need to take into account that in memory regions a pointer might
be anywhere.  Also be careful about where you end your memory search,
as it's easy to get off by one errors where you don't check the last 8
bytes of memory as a pointer.

Note that if you're copying data to the new region with memcpy, you're
probably going to discover a problem here.  Take a look at memmove
instead.


## 3e: gc\_code\_relocation

So this is where things get weird.  In forth, the heap contains not
only data but actual compiled code...meaning we might need to relocate
compiled functions.

But there's nothing special we need to do here - forth compiled code
contains pointers just like data, we relocate the pointers and the
code works.

Except the variable forth.latest refers to the latest allocated code
function.  If that latest function is moved (and it will be in these
tests), the forth.latest pointer must be pointed at the updated code
region.

# Conclusion

That's it.  Submit the assignment as usual!

# Rubric

| Part                                      | Points | Comments                                                                                         |
| Step 1: Record All Regions                | 15     |                                                                                                  |
| Step 2: Identify all inaccessible regions | 30     | I know this part has a lot of tests, but if you get the idea right they should all start working |
| Step 3: test\_gc\_no\_pointer\_rewrite    | 20     |                                                                                                  |
| Step 3: test\_gc\_stack\_update           | 10     |                                                                                                  |
| Step 3: test\_gc\_internal\_data\_update  | 10     |                                                                                                  |
| Step 3: test\_gc\_unaligned\_data\_update | 10     |                                                                                                  |
| Step 4: test\_gc\_code\_relocation        | 5      |                                                                                                  |

