#define ITER 20
#define BUFSIZE 80
// Change VERB to 0 to reduce output
#define VERB 0

typedef enum {FAST, SLOW, UNRELIABLE} MODE;

int read_pipe(char *name, MODE mode, int fd);
int write_pipe(char *name, MODE mode, int fd);
