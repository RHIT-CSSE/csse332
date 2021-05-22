#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
say_hello_using_echo(void)
{
	system("echo hello world");
}

void
vulnerable_fn(char *argument)
{
	char buff[32];
	strcpy(buff, argument);
}

int
_main(int argc, char **argv)
{
	printf("argc=%d\n", argc);
	if (argc < 2) {
		fprintf(stderr, "ERROR: Need an input argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc > 2) {
		/* printf("argv[1] = %s\n", argv[1]); */
		/* printf("len = %d\n", strlen(argv[1])); */
		/* printf("len = %d\n", strlen(argv[2])); */
		/* printf("argv[2] = %s\n", argv[2]); */

		int llen = strlen(argv[1]) + strlen(argv[2]);
		char *x = malloc(llen + 1);
		for (int i = 0; i < strlen(argv[1]); i++)
			x[i] = argv[1][i];
		for (int i = 0; i < strlen(argv[2]); i++)
			x[strlen(argv[1]) + i] = argv[2][i];
		x[llen]='\0';
		vulnerable_fn(x);
		return 0;
	} else {
		say_hello_using_echo();
		vulnerable_fn(argv[1]);
		exit(0);
	}

}
