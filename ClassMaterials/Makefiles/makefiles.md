---
layout: slides
---

# Makefiles

A Basic Introduction

---

# make is generic build tool

+ It works off the idea that a particular build target (e.g. an executable program) is built from particular dependent files
+ These sources might be stuff that's edited directly, or might be themselves build targets that have their own dependencies
+ We detect changes based on the modification dates of files
+ We try to rebuild only what is needed based on changes

---

# Basic Syntax

Imagine that we have a file called myprog.c that compiles to executable myprog.

Makefile:

    myprog: myprog.c
        gcc myprog.c
        mv a.out myprog

In english, "the executable myprog depends on myprog.c".  To build myprog, execute the given commands (note these are tabbed in).

Build myprog like this:

    $ make myprog



---
Let's say there's also a zipfile that should also be kept up to date

    myprog: myprog.c
        gcc myprog.c
        mv a.out myprog
    
    myprog_stuff.zip: myprog.c myprog
        zip myprog_stuff.zip myprog.c myprog
        
If we run this

    $ make myprog_stuff.zip

It will rezip the files, but only if something has changed.

---
To get even slighty fancier:

    all: myprog myprog_stuff.zip

    myprog: myprog.c
        gcc myprog.c
        mv a.out myprog
    
    myprog_stuff.zip: myprog.c myprog
        zip myprog_stuff.zip myprog.c myprog

We put the all rule first because if you say "make" with no parameters it defaults to the first rule.

---

# Building c incrementally

If we want to build a big c program, we can always pass all the c files to gcc like this:

    $ gcc someMain.c bigLibraryA.c bigLibraryB.c -o someProg
    
The problem is that this builds everything from scratch even if only a little has changed.  So if you change someMain.c, it recompiles bigLibraryA even if that isn't needed.

---

# .o files

.o files are individually compiled c files.  It let's us do the build in 2 stages: compilation and linking.  The -c gcc flag means compile but don't link

    $ gcc -c someMain.c -o someMain.o
    $ gcc -c bigLibraryA.c -o bigLibraryA.o
    $ gcc -c bigLibraryB.c -o bigLibraryB.o
    $ gcc someMain.o bigLibraryA.o bigLibraryB.o -o someProg
    
Why is this better?

---
# Makefiles with .o files

    someMain.o: someMain.c bigLibraryA.h bigLibraryB.h
        gcc -c someMain.c -o someMain.o
    
    bigLibraryA.o: bigLibraryA.c bigLibraryA.h
        gcc -c bigLibraryA.c -o bigLibraryA.o
        
    bigLibraryB.o: bigLibraryB.c bigLibraryB.h
        gcc -c bigLibraryB.c -o bigLibraryB.o
        
    someProg: someMain.o bigLibraryA.o bigLibraryB.o
        gcc someMain.o bigLibraryA.o bigLibraryB.o -o someProg

Note the .h files are how someMain.c references functions in bigLibraryA that won't be actually provided until the linking step.  It's crucial that the dependencies be specified correctly (even though, as you can see, the .h files are not needed in the compilation commands themselves). 

---
# Activity

In the make_tutorial directory there is an almain which also relies on arraylist library in the same directory.

You can compile it like this:

    $ gcc almain.c arraylist.c -o almain
    
But that will rebuild everything.  Instead make a Makefile the constructs .o files for almain and arraylist and then links them.

---
# Activity Goals

If you build your almain Makefile correctly:

+ typing "make almain" should build everything, if nothing exists
+ typing "make almain" after editing only editing almain.c should only rebuild almain.o and the executable
+ typing "make almain" after editing only editing arraylist.c should only rebuild arraylist.o and the executable
+ typing "make almain" after editing only editing arraylist.h rebuild everything
+ typing "make all" or "make" should act as if you had typed "make almain"

Hint: for your testing, typing "touch <filename>" will make a file appear to be edited

---
# Activity Solution

	all: almain
	
	almain.o: almain.c arraylist.h
		gcc -c almain.c -o almain.o
	
	arraylist.o: arraylist.c arraylist.h
		gcc -c arraylist.c -o arraylist.o
	
	almain: almain.o arraylist.o
		gcc almain.o arraylist.o -o almain
	
