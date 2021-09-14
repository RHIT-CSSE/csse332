# Your solution to the activity here!
# In this directory there is an almain which also relies on arraylist library in the same directory.
# You can compile it like this:
#
#     $ gcc almain.c arraylist.c -o almain.bin
#
# But that will rebuild everything.  Instead edit this Makefile so
# that it constructs .o files for almain and arraylist and then links
# them.
#
# Requirements:
#
# typing "make almain.bin" should build everything, if nothing exists
# typing "make almain.bin" after editing only editing almain.c should only rebuild almain.o and the executable
# typing "make almain.bin" after editing only editing arraylist.c should only rebuild arraylist.o and the executable
# typing "make almain.bin" after editing only editing arraylist.h rebuild everything
# typing "make all" or "make" should act as if you had typed "make almain.bin"
#
# Hint: for your testing, typing "touch <filename>" will make a file appear to be edited
#
# Bonus hint: TAB characters are required in Makefiles for indenting rules, spaces won't do the job

all: almain.bin

almain.bin: almain.o arraylist.o 
	gcc -o $@ $^

almain.o: almain.c arraylist.h
	gcc -c almain.c

arraylist.o: arraylist.c arraylist.h
	gcc -c $^

.PHONY: clean all

clean:
	rm -f makefiles.zip almain.bin *.o
