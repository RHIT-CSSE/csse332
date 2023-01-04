#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8021
#define HOST "localhost"
#define MAX_CMD_LEN 128

extern void red(void);
extern void green(void);
extern void purple(void);
extern void reset(void);

int main(int argc, char **argv)
{
  struct hostent *hostnm;
  struct sockaddr_in server_addr;
  int s;

  /* grab the hostname */
  if((hostnm = gethostbyname(HOST)) == 0) {
    fprintf(stderr, "%s: gethostbyname failed\n", __func__);
    exit(100);
  }

  /* set the server address */
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = *((unsigned long*)hostnm->h_addr_list[0]);

  /* grab a stream socket */
  if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "%s: Failed to create stream socket...\n", __func__);
    exit(100);
  }

  /* connect to the server */
  if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    fprintf(stderr, "%s: Failed to connect to server socket...\n", __func__);
    exit(100);
  }

  printf("\033[0;32mClient connected to server at port %d\033[0m\n", PORT);

  /* start the mini shell */
  char command[MAX_CMD_LEN];
  while(1) {
    purple();
    printf("CLIENT (%d) %%%% ", getpid());
    reset();

    fgets(command, MAX_CMD_LEN, stdin);
    command[strlen(command)-1] = '\0';
    if(!strcmp(command, "quit()") || !strcmp(command, "exit()"))
      break;

    if(send(s, command, strlen(command), 0) < 0) {
      fprintf(stderr, "Failed to send command to server...\n");
      break;
    }

    /* leave the loop on close */
    if(!strcmp(command, "close")) {
      break;
    }
  }
  printf("\033[0;32mClient closing connection to server at port %d\033[0m\n",
         PORT);
  close(s);
  exit(0);
}
