#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
print_bad_outcome(void)
{
	printf("You are pwnd\n");
	exit(1);
}

void
print_good_outcome(void)
{
	printf("Have a nice day =)\n");
	exit(0);
}

void
vulnerable_fn(void)
{
	char input[4];
	gets(input);
}

void
_main(void)
{
	vulnerable_fn();
	print_good_outcome();
}

int
main(int argc, char **argv)
{
	_main();
}
