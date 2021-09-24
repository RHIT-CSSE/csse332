#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <time.h>

static inline void
fail_and_exit(void) {
	printf("Repair demon failed the repair operation. Damn them to hell!\n");
	exit(2);
}

static inline void
pass_and_exit(void) {
	printf("Repair demon succeeded in the repair operation. Promote to heaven?\n");
	exit(0);
}

static inline int
get_rand(int n)
{
	if (n == RAND_MAX) {
		return rand();
	} else {
		long limit = (RAND_MAX / n)*n;
		int r;
		while (1) {
			r = rand();
			if(r < limit) break;
		}
		return r % n;
	}
}


int
main(int argc, char **argv)
{
	int r;
	long state = 0;

	/* seed the pseudo random number generator */
	srand(getpid());

	printf("Repair demon starting damned repair operations...\n");

	if (argc > 1) {
		state = strtol(argv[1], NULL, 0);
		if (state > 2 || state < 0) {
			fprintf(stderr, "Incorrect argument passed to repair demon! FAILING\n");
			exit(3);
		}
		printf("\t Repair demon received state: %s\n",
		       (state == 1)? "always succeed!" : "always fail!");
	}

	/* sleep for 10 seconds */
	sleep(5);

	if (state == 0) {
		/* get a random integer between 0 and 100 */
		r = get_rand(100);
		/* if number is < 50, exit with failure. Otherwise exit with success */
		if (r < 50) {
			fail_and_exit();
		}
		pass_and_exit();
	} else if (state == 1) {
		pass_and_exit();
	} else {
		fail_and_exit();
	}
}
