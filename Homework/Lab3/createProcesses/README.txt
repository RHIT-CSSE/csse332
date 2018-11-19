The programs in this folder demonstrate the use of the "fork", "wait", "waitpid", "getpid" and "exec" family of system calls.

Look at the programs in the following order:
a. createChild.c
b. createChildAndWait.c
c. createChildAndWait-v2.c

The "myCopy.c" file is used to create a stand-alone executable "myCopy" which is then used to demonstrate the "execv" and "execlp" systems calls. The program copies the contents of an input file to an output file, the names of which are specified at the command prompt. The "test.txt" is a sample input file.