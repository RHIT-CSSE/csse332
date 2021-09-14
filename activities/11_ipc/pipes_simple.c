/*****************************************************************************
  Excerpt from "Linux Programmer's Guide - Chapter 6"
  (C)opyright 1994-1995, Scott Burkett

  Modified by Buffalo, Rose-Hulman Institute of Technology
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(void)
{
	int     fd[2], nbytes;
	pid_t   childpid;
	char readbuffer[80];
	char data[] = "23 and 37\n";  
	pipe(fd);

	if((childpid = fork()) == -1) {
		perror("fork");
		return 1;
	}

	if(childpid == 0) {
		/* Child process closes up input side of pipe */
		close(fd[0]);
		write(fd[1], data, (strlen(data)+1));
		close(fd[1]);
		return 0;
	} else {
		/* Parent process closes up output side of pipe */
		close(fd[1]);
		nbytes = read(fd[0], readbuffer, 80);
		printf("Received string: %s", readbuffer);
		close(fd[0]);

	}

	return 0;
}
