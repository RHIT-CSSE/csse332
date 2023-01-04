#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

/* The port number to listen on. */
#define PORT 8021

/**
 * Needed functions, implemented elsewhere.
 *
 * @warning You do not need to worry about any of these functions. Nor will you
 * need to call any of them.
 */
extern int create_socket(int);
extern void red(void);
extern void green(void);
extern void purple(void);
extern void reset(void);
extern void yellow(void);

#define MAX_RECV_LEN 128
#define MAX_CONNECTIONS 10
#define MAX_BACKGROUND_CMDS 5

/**
 * Helper functions.
 * 
 * @warning You do not need to edit any of these functions.
*/
void parse_client_command(char*, char*[2]);
void read_from_client(int, char*, int);

void service_request(int ns, int client, int fd)
{
  char buff[MAX_RECV_LEN];
  char *parsed_command[2];
  int rlen;
  pid_t pid;
  int status;
  int num_background_children = 0, i;
  pid_t children[MAX_BACKGROUND_CMDS];
  char log[512];
  int llen;

  printf("\033[0;32m%s(%d): \tAccepted connection from client port "
         "%d\033[0m\n",
         __func__, getpid(), client);

  while(1) {
    /* read the command from the client */
    read_from_client(ns, buff, client);

    purple();
    llen = sprintf(log, "Worker %d received %s from client\n", getpid(), buff);
    log[llen] = '\0';
    printf("%s", log);

    /* TODO: STEP 6 : Send this log back to main parent for logging */

    /* parse the client command */
    parse_client_command(buff, parsed_command);

    /* check if it is the close command */
    if(!strcmp(buff, "close")) {
      /* TODO: STEP 4: Close all children processes */

      /* Do not change the code below, use it for debugging */
      printf("\033[0;32m%s(%d): \tClosing connection from client port "
             "%d\033[0m\n",
             __func__, getpid(), client);
      if(fd) close(fd);
      close(ns);
      exit(1);
    }

    /* TODO: STEP 2-3 : Handle the command sent by the client */
  }
}


/**
 * Serve and listen for incoming connections from the clients.
 *
 * @warning If all goes well, this function never returns.
 */
void server(void)
{
  int sock;
  struct sockaddr_in client_addr;
  int addrlen = sizeof client_addr;
  int ns;
  pid_t pid;
  int client;
  int fd[2];

  sock = create_socket(PORT);
  printf("\033[0;31mServer listening on port %d\033[0m\n", PORT);
  while(1) {
    /* accept new connection from client */
    ns = accept(sock, (struct sockaddr *)&client_addr, &addrlen);
    if(ns == -1) {
      fprintf(stderr, "%s: Failed to accept incoming connection..\n", __func__);
      exit(1);
    }
    client = client_addr.sin_port;
    
    /* TODO: STEP 1: Handle a new connection */
  }
}

/**
 * The main function.
 *
 * This only makes a call to the serve function and nothing more.
 */
int main(int argc, char **argv)
{
  server();
  exit(0);
}

void parse_client_command(char *command, char *parsed_command[2])
{
  int len_1;

  parsed_command[1] = 0;
  for(len_1 = 0; command[len_1] != '\0'; len_1++) {
    if(command[len_1] == ' ')
      break;
  }
  parsed_command[0] = command;
  if(len_1 == strlen(command)) {
    parsed_command[1] = 0;
  } else {
    command[len_1] = '\0';
    parsed_command[1] = command + len_1 + 1;
  }
}

void read_from_client(int ns, char *buff, int client)
{
  int rlen;
  if((rlen = recv(ns, buff, MAX_RECV_LEN - 1, 0)) <= 0) {
    printf("\033[0;32m%s(%d): \tClosing connection from client port "
           "%d\033[0m\n",
           __func__, getpid(), client);
    close(ns);
    exit(1);
  }
  buff[rlen] = '\0';
}
