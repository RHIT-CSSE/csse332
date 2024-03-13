#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000

void asserthandshake(char* readbuffer, int* readpipe);
char* printbuffalo(char* msg);

int main(int argc, char** argv) {
  if(argc < 3) {
    printf("You do not know how to call buffalosay.bin.\n");
    exit(1);
  }
  int readpipe[] = {atoi(argv[1]), atoi(argv[2])};
  close(readpipe[1]);

  char readbuffer[BUFSIZE];

  asserthandshake(readbuffer, readpipe);

  int b = read(readpipe[0], readbuffer, BUFSIZE);
  char* result = printbuffalo(readbuffer);
  printf("%s", result);
  
  close(readpipe[0]);

  srand(time(NULL));
  return rand() % 100;
}


void asserthandshake(char* readbuffer, int* readpipe){
  int b = read(readpipe[0], readbuffer, 1000);
  if(b == -1){
    perror("Error reading message to print from pipe");
    exit(1);
  }
  
  if(strcmp("secret handshake", readbuffer) != 0){
    printf("You do not know how to call buffalosay.bin.\n");
    exit(2);
  }

  memset(readbuffer, 0, 1000);
}

// credit to the classic unix program cowsay
// https://en.wikipedia.org/wiki/Cowsay
//
// and also to mic & dwb who I guess created this ascii art to begin
// with (https://asciiart.website/index.php?art=animals/bison)

char* printbuffalo(char* msg){

  char* ret = malloc(sizeof(char)*1000);
    snprintf(ret, sizeof(char)*1000,
"             _.-````'-,_\n"
"   _,.,_ ,-'`           `'-.,_\n"
" /)     (\\                   '``-.\n"
"((      ) )                      `\\\n"
" \\)    (_/                        )\\\n"
"  |       /)           '    ,'    / \\\n"
"  `\\    ^'            '     (    /  ))\n"
"    |      _/\\ ,     /    ,,`\\   (  \"`\n"
"     \\Y,   |  \\  \\  | ````| / \\_ \\\n"
"    /  `)_/    \\  \\  )    ( >  ( >\n"
"   |            \\( \\(     |/   |/\n"
"   |           /_(/_(    /_(  /_(\n"
"   | \n"
"   < %s > - Buffalo, probably \n\n", msg);
  
  return ret;
}

