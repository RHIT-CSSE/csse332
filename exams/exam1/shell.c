#include "shell.h"


int
main(int argc, char **argv)
{
	char *line_buff;

	while((line_buff = readline("CSSE332 >> "))) {
		if(strcmp(line_buff, "exit") == 0) {
			free(line_buff);
			cleanup();
			exit(EXIT_SUCCESS);
		} else if(!strcmp(line_buff, "hibernate")) {
			hibernate();
		} else if(!strcmp(line_buff, "sleep")) {
			csse332_sleep();
		} else if(!strcmp(line_buff, "status")) {
			status();
		} else if(!strcmp(line_buff, "cleanup")) {
			cleanup();
		} else if(!strcmp(line_buff, "pong")) {
			pong();
		} else if(!strcmp(line_buff, "fpoint")) {
			fpoint();
		} else if(!strcmp(line_buff, "primes")) {
			primes();
		}else {
			if(strlen(line_buff) > 0)
				printf("[ERROR] Unrecognized command \"%s\"\n",
				       line_buff);
		}

		free(line_buff);
	}
}
