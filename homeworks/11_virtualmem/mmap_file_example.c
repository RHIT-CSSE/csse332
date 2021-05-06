#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include "forth_embed.h"

#define STACKHEAP_MEM_START 0xf9f8c000

void main() {

    //create the file and use lseek to ensure that it has enough space
    //to contain the full page
    int fd = open("page_0.dat", O_RDWR | O_CREAT, S_IRWXU);
    if(fd < 0) {
        perror("error loading linked file");
        exit(25);
    }
    char data = '\0';
    lseek(fd, getpagesize() - 1, SEEK_SET);
    write(fd, &data, 1);
    lseek(fd, 0, SEEK_SET);

    char* result = mmap((void*) STACKHEAP_MEM_START,
                        getpagesize(),
                        PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_FIXED | MAP_SHARED,
                        fd, 0);


    //lets put some data in the file
    for(int i = 0; i < getpagesize(); i++) 
        result[i] = 'Q';

    // now lets unmap and close the file to ensure everything gets written
    int munmap_result = munmap((void*) STACKHEAP_MEM_START, getpagesize());
    if(munmap_result < 0) {
        perror("munmap failed");
        exit(6);
    }

    close(fd);
    printf("done\n");
    // take a look at the file to see if the write worked!
}
