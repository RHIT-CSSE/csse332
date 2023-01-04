#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int create_socket(int port)
{
  int sock;
  struct sockaddr_in server_address = {AF_INET, htons(port), INADDR_ANY};

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "%s: Cannot open server socket...\n", __func__);
    exit(1);
  }

  if(bind(sock, (struct sockaddr *)&server_address, sizeof server_address) <
     0) {
    fprintf(stderr, "%s: Cannot bind server socket to address...\n", __func__);
    perror("Bind failed:");
    exit(1);
  }

  listen(sock, 1024);
  return sock;
}

int accept_connection(int socket)
{
  int addrlen = 0;
  struct sockaddr_in client_addr;
  addrlen = sizeof client_addr;
  return accept(socket, (struct sockaddr*)&client_addr, &addrlen);
}
