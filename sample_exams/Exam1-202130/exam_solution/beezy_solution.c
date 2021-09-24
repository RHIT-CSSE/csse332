#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "say_hello.h"


#define MAX_HEATING_DEMONS 2
#define MAX_REPAIR_DEMONS 4

void
catch_term_signal(int signal)
{
	printf("Heating demon killed.... OUCH!\n");
	exit(0);
}

int
main(int argc, char **argv)
{
	/* Remove this before giving to students */
	int heating_demon_ids[MAX_HEATING_DEMONS];
	int repair_demon_pipes[MAX_REPAIR_DEMONS];
	int repair_demon_ids[MAX_REPAIR_DEMONS];
	int i;
	int fork_result;
	int err;
	int wstatus;
	size_t len;
	int num_success_repairs = 0;


	/* Let beezy say hello to you, he's a polite devil! */
	say_hello();

	/* you code goes here.... */
	for(i = 0; i < MAX_REPAIR_DEMONS; ++i) {
		int pipe_result[2];
		pipe(pipe_result);

		fork_result = fork();
		if (fork_result == 0) {
			/* this is repair demon manager */

			/* no need to read */
			close(pipe_result[0]);
			fork_result = fork();
			if (fork_result == 0) {
				/* actual repair demon, execute repair */
				err = execlp("./repair_demon", "repair_demon",
					     NULL);
				/* should never get here! */
				fprintf(stderr,
					"Something is really wrong, couldn't execlp\n");
				exit(3);
			} else {
				/* manager code: wait for repair demon and then
				 * decide what to do back to the parent!
				 */
				wait(&wstatus);
				if (WEXITSTATUS(wstatus) == 2) {
					// failed, fork cleanup demon
					fork_result = fork();
					if (fork_result == 0) {
						printf("Triggering a cleanup demon\n");
						sleep(3);
						exit(0); // put this in rubric
					} else {
						wait(NULL);
						write(pipe_result[1], "FAILURE", 7);
						close(pipe_result[1]);
					}
				} else {
					// succeeded
					write(pipe_result[1], "SUCCESS", 7);
					close(pipe_result[1]);
				}
				exit(0);
			}
		} else {
			/* no need for the parent to write */
			repair_demon_ids[i] = fork_result;
			repair_demon_pipes[i] = pipe_result[0];
			close(pipe_result[1]);
		}
	}

	/* summon heating demons */
	for (i = 0; i < MAX_HEATING_DEMONS; i++) {
		fork_result = fork();
		if (fork_result == 0) {
			signal(SIGTERM, catch_term_signal);
			printf("Heating demon looping forever\n");
			while (1);
		} else {
			heating_demon_ids[i] = fork_result;
		}
	}

	/* check status of repair demons */
	for (i = 0; i < MAX_REPAIR_DEMONS; i++) {
		char message[101];
		waitpid(repair_demon_ids[i], NULL, 0);
		len = read(repair_demon_pipes[i], message, 101);
		close(repair_demon_pipes[i]);
		if (len < 0) {
			fprintf(stderr,
				"Failed to read from pipe, beezy kills everything!\n");
			exit(EXIT_FAILURE);
		} else if (len == 0) {
			fprintf(stderr,
				"Couldn't read anything from repair demon %d\n",
				i);
			exit(EXIT_FAILURE);
		} else {
			message[len] = '\0';
			if (!strncmp(message, "SUCCESS", len)) {
				num_success_repairs++;
			}
		}
	}

	if (num_success_repairs >= MAX_REPAIR_DEMONS / 2) {
		printf("Beezy is happy, everything went well, hell is back in business\n");
		for (i = 0; i < MAX_HEATING_DEMONS; ++i) {
			kill(heating_demon_ids[i], SIGTERM);
			waitpid(heating_demon_ids[i], NULL, 0);
		}
		exit(0);
	} else {
		printf("Beezy is giving up, here is my resignation letter\n");
		exit(1);
	}
}
