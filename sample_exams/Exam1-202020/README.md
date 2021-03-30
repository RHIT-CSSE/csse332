# Exam 1 202020

# Introduction

So in this system, we have two processes file\_producer and
file\_processor.  The basic idea is that file\_producer creates a
bunch of files called data###.dat, and stores their filenames in a
list called files.dat.  Each time file\_producer runs it produces a
new batch of files.  file\_processor reads the filenames out of
files.dat, and then processes each file.

This system already works as described.  Except, its fake - no actual
data###.dat files are produced, and "processing" the files does
nothing.  The creation and reading of the files.dat file really
happens though, in the code you've been given.

In this exam we'll use fork and related functions to improve the
functionality of file\_processor.  We will use file\_producer but we
won't change the code.

# Makefile (15 points)

So you can build the given files like this:

    gcc file_processor.c -ggdb -o file_processor
    gcc file_producer.c -ggdb -o file_producer

Build a Makefile with rules for file\_processor and file\_producer.
Both of them depend on file.h, so make sure to ensure that if file.h
changes both binaries are rebuilt.

For full credit, use the -c option to build individual .os and the
link the .os in a second link step.  This doesn't save any time
because no executable depends on two .os but I want to see that you
can do it.  If your Makefile is correct but doesn't use .os, you'll
get 10/15.

#  Step 1: Processing files in parallel (25 points)

So right now file\_processor deals with each of the files serially.
Use fork make each of the files be processed in parallel instead.

The processor should wait till all files are finished processing
before exiting.

Example output:

	master processor started
	file processor starting up
    file processor starting up
	file processor ready
	file processor processing data001.dat
	file processor ready
	file processor processing data002.dat
    file processor done processing data001.dat
	file processor done processing data002.dat
	master processor finished. 2 files processed successfully.

Don't worry if your filenames don't match mine.

# Step 2: Running the file\_producer (25 points)

Rather than having to run the producer manually, we would like the
file producer to run each time you run file\_processor.  Use fork/exec
to run the "./file\_producer" before the processor processes are
started.  You should pass the producer one parameter, which is the
number of files to produce (hint look at num\_files\_to\_create\_str
in your codebase).

This means that each time your processor runs, it should run with
different file names.

You code should wait for the producer to complete before it attempts
to read files.dat and process the files.

Example output:

	master processor started
	producer started
	outputing file data138.dat
	outputing file data951.dat
	producer finished
	file processor starting up
	file processor starting up
	file processor ready
    file processor processing data138.dat
	file processor ready
	file processor processing data951.dat
	file processor done processing data138.dat
	file processor done processing data951.dat
	master processor finished. 2 files processed successfully.

# Step 3: Handling processor errors (15 points)

Sometimes our file processing may have errors, in case we would like
to let the master processor know.  To add error simulation, uncomment
the relevant lines in process\_file.  If an error occurs, processing
can abort.  However, we'd like the final count of processing files to
reflect the fact that a file was not processed successfully.  You're
welcome to communicate this back to the master process any way you
wish, but I recommend exit status.  Edit the process\_file function to
handle the error in the way your master process expects.

Example output:

	master processor started
	producer started
	outputing file data648.dat
	outputing file data155.dat
	producer finished
	file processor starting up
	file processor starting up
	file processor ready
	file processor ready
    file processor processing data155.dat
	file processor processing data648.dat
	error discovered processing data648.dat
	file processor done processing data155.dat
	master processor finished. 1 files processed successfully.

# Step 4: Starting up the processors early (20 points)

So our processors have a startup time before they can begin processing
their files in setup\_for\_processing.  We're going to assume for this
problem that this is NOT something we can simply place before the
fork.  Instead, we'll imagine that each processing thread must run it
separately.  But we would like to do this setup in parallel with
running the file\_producer.  But if we do that, how can we communicate
the filenames?  They don't get read till after the producer finishes.

The solution we'll use is that we'll communicate the filenames to the
processing process via pipe.  The master process will startup the
child processors, then run the producer, then once the producer
finishes read the files.dat file, then pipe one file name to each
processor.

Example output:

	master processor started
	file processor starting up
	file processor starting up
	producer started
	outputing file data590.dat
	file processor ready
	file processor ready
	outputing file data449.dat
	producer finished
	file processor processing data590.dat
	file processor processing data449.dat
	error discovered processing data590.dat
	file processor done processing data449.dat
	master processor finished. 1 files processed successfully.

When you're piping data to the processes, you can assume each filename
is exactly FILENAME\_LEN characters long.

Getting this right is trickier than the previous steps, and requires
reorganizing things.  I encourage you to back up your .c file before
starting it.

# Submitting

You're done!  Do it via Moodle like usual.
