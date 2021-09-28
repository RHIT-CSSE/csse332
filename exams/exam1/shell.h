#ifndef __SHELL_H
#define __SHELL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

void hibernate(void);
void csse332_sleep(void);
void status(void);
void cleanup(void);
void pong(void);
void fpoint(void);
void primes(void);
void do_fp_work(void);

#endif
