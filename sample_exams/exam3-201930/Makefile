#
# Generic makefile
#

SHELL =  /bin/bash -f

MAKE = make
RM = rm -f

CODE = $(sort $(wildcard *.c))
EXEC = $(CODE:%.c=%)

all:	$(EXEC) Makefile
.PHONY:	all

%:	%.c
	gcc -pthread -std=gnu99 -Wall -ggdb $< -o $@

clean:	
	$(RM) $(EXEC)
.PHONY:	clean
