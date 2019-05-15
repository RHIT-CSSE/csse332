---
layout: slides
---

# Garbage Collection 2

### Tracing Garbage Collection

---

# The story thus far

+ Our programming languages want to automatically detect when memory
  is inaccessible and automatically free it
+ Reference counting garbage collection has a major issue with
  circular references
  
---

# Simple Tracing Garbage Collection

Periodically or when memory is low...

1. Pause the execution of our language
2. Start with a set of pointers we call the "roots"
   + These are variables that are definitely accessible
   + Includes the stacks of all threads
   + Includes globals
3. For each accessible object
   + Find all objects that are accessible through the object's fields
   + Mark these objects as accessible
   + Add these objects to the list of objects to be searched
4. When this is done, free any unmarked objects

---

## Pros

+ Never frees something accessible
+ Always frees everything unused
+ No trouble with circular references

## Cons

+ Basically have to visit entire memory space - slow
+ Program is paused while this is happening
+ Memory is fragmented

---

# Compacting GC

Initially it might seem inefficient, but it can actually be better to:

+ Copy known accessible objects into a new memory space, updating
  references
+ Free the old memory space

---

Pros:

+ We naturally de-fragment the memory as we move it, meaning that we
  can allocate memory simply by moving a pointer in the large
  contiguous free space region created (otherwise we need to mess
  around with linked lists of free space)
+ We can deallocate the entire old memory region at once.  Usually
  deallocate is expensive-ish and has the same cost regardless of size
+ If we really get fancy, we can move related objects close to each
  other and improve our caching performance

Cons:

+ Time spent copying
+ Costs double the memory
+ Can break pointer arithmetic or non gc-managed pointers

---

# Generational GC

This is based on the observation that most objects tend live for a
very short amount of time BUT some objects live for a very long amount
of time.

1.  Have a separate memory space for long lived objects
2.  Take special note of when a long-lived object starts pointing to a
    short-lived object (this tends easy to tell because the spaces are
    distinct).  Put such objects in a "remembered set".

---

When your garbage collection runs
1.  Start from the roots as usual, but ignore any references from the
    roots to long lived objects.  We assume all long lived objects are
    going to be kept and references to the short-lived objects are
    handled by the remembered set.
2.  Also consider the remembered set as roots

Any short lived objects not marked by #1 or #2 are inaccessible
and freed.

3.  After some time, promote short lived objects to long lived objects

Note that this is a heuristic so occasionally we probably need to
garbage collect the whole memory space

---

# Incremental/Concurrent GC

We really want to avoid the long pause of "stop the world" garbage
collection.

We can:

+ Do some of the work at regular intervals (e.g. python combines
  reference counting with tracing gc so many objects are deallocated
  as variables go out of scope etc.) but we can try to incrementally
  update sets of referenced/maybe referenced too
+ Run garbage collection in its own separate thread with some fancy
  work to ensure it doesn't view the world in an inconsistent state

Most moderns GCs do this kind of thing

---

Pros:
+ No pausing (or at least shorter pauses)

Cons:
+ Complex algorithms (but workable! just more than this class)
+ Doesn't reduce the work required to be done, just spaces it out over
  time (i.e. you're still in-aggregate paying for that full memory
  space traversal)

---

# What to remember about garbage collection

Cons:

1. Reliable garbage collection generally requires some pretty
   expensive operations.  We can amortize or use heuristics to improve
   the cost but not eliminate it.
2. Modern GC algorithms are complex so GC can produce pauses at
   unexpected times in your code

Pros:

1. Reliably eliminates a whole class of bugs and super doesn't matter
   for small/medium programs
   
---
# Good advice

Allocating and freeing efficiently for programs that use a large
amount of memory is important, regardless of memory model.  C makes it
explicit and that encourages you to think about how memory can be
reused etc.  But similar techniques can work in GCed languages too.

Leaks can exist in GC languages too, if you needlessly hold on to
references you no longer care about.
