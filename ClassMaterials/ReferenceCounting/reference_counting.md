---
layout: slides
---

# Garbage Collection: Reference Counting

The way you probably think automatic memory management works

---

In languages like C, we have to explicitly allocate and deallocate
memory.  In other languages, usually have a way to explicitly allocate
memory but there is not always a deallocate (e.g. java):

    // this is similar to a C malloc
    int[] myData = new int[10000];
    
    // some code that uses myData
    
    // this is not anything like C free
    myData = null;
    
Presumably it's not leaked (e.g. kept forever, with no way to free
it), right?

---

This is complicated by the fact that different objects might point to
the same region of memory:

    int[] myData = new int[10000];
    int[] myData2 = myData; // not a copy, just a pointer assignment
    
    myData = null;
    System.out.println(myData2[5]); // 100% safe (in java)
    
In C we have to be super careful to ensure we don't attempt to use
some memory after it is freed.  How could this be safe in Java?

---

# Garbage Collection

Goal: When a region of memory is no longer accessible through any
existing pointer, automatically free it.

---

# Reference Counting

A simple way to solve this problem seems to be "reference counting"
i.e. keeping track of how many variables are referencing a particular
region of memory.

    char* foo = malloc(1000);
    // one thing pointing to the malloc
    char* bar = foo;
    // now 2
    foo = NULL;
    // don't free malloc yet
    bar = NULL;
    // now free

---

For reference counting to work we must:

1.  Be able to execute code when variables go out of scope or are
    assigned:
    
        void parameterPointer(void* mem) {
            // mem's count must be incremented for the duration of this function
            
            void* otherMem = mem; // incremented again
            
            otherMem = null; //decremented 
        }

2.  Allocate some extra space somewhere to hold the counts
3.  Not use trixy pointer arithmetic

        ptr = ptr + 10000;
        // what should the count be in this region?
        ptr = ptr - 10000;
        
---

# Reference Counting in C++

These requirements make C not a good language for reference counting,
but C++ has what we need:

    shared_ptr<ExampleObj> var1 = make_shared<ExampleObj>();
    shared_ptr<ExampleObj> var2 = var1;

    printf("Setting var1 to null\n");
    var1 = nullptr;
    //ExampleObj should not be deleted yet because var2 still exists
    var2->data[17] = 5;
    printf("Setting var2 to null\n");
    var2 = nullptr;

---
# Activity 1

One important part of ref counting schemes is that the deletion of one
object triggers the deletion of another.

In the given code:

    ExampleObj* holder = new ExampleObj();
    holder->next = new ExampleObj();
    holder = nullptr;

Both holder and holder->next are raw pointers and therefore both
objects are leaked.  Switch them both to shared\_ptrs and you should
see both are properly disposed of.  Correct output looks like:

    Activity 1
    ExampleObj memory freed
    ExampleObj memory freed

---

# Is Reference Counting Good?

It has a major problem we'll talk about in a minute.  But that aside:

1. It's pretty cheap to do, resource-wise
2. It reliably prevents use-after-free issues
3. It can be mixed with regular pointers
4. Memory is freed at the moment a variable goes out of scope, meaning
   you can reliably do stuff on object free

It's frequently used in C++, was the main method in Perl 5 (but not
Perl 6).  So it's popular, but isn't most common.

---

# The Problem

Imagine you have a system like this:


![Circular Refs](circ_ref.png)

---

# The Problem with Reference Counting: Circular References

Circular references can cause a reference counting scheme to leak
structures of memory, because internal references prevent the count
from being decremented to 0.

There is no simple solution to this problem.

---

# Activity 2: Make some circular references

Use the ExampleObjHolder class to create a leaked data structure using
only shared_ptrs.  If you've done it correctly you should construct
shared pointers but they should never be freed.

---

# What to remember about reference counting

Reference counting can an effective memory management scheme in some
circumstances but you must always be extremely careful not to leak
memory by introducing circular structures.

However, because of the issue with circular references this is *not*
what most modern languages use to automatically manage memory
