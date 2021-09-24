#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "say_hello.h"


#define MAX_HEATING_DEMONS 2
#define MAX_REPAIR_DEMONS 4

int
main(int argc, char **argv)
{
	/* Let beezy say hello to you, he's a polite devil! */
	say_hello();

	/* you code goes here.... */
}
