#ifndef SETSIGHANDLER_H
#define SETSIGHANDLER_H

#include <signal.h>

void old_setsighandler(int signum, void (*handler)(int))
{
	signal(signum, handler);
}

void setsighandler(int signum, void (*handler)(int))
{
	struct sigaction act;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(signum, &act, NULL);
}

void mask_signal(int signum)
{
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, signum);
	if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
		perror("sigprocmask");
}

void unmask_signal(int signum)
{
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, signum);
	if(sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0)
		perror("sigprocmask");
}

#endif
