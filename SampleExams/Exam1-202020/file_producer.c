#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file.h"


int main(int argc, char** argv) {

    printf("producer started\n");

    int num_files = 0;
    
    if(argc <= 1) {
        printf("no arguments passed. defaulting to 2.\n");
        num_files = 2;
    } else {
        num_files = atoi(argv[1]);
    }
    if(num_files == 0) {
        printf("bogus parameter passed! aborting\n");
        exit(3);
    }
    
    time_t t;
    srand((unsigned) time(&t));
    
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = open("files.dat", O_CREAT | O_WRONLY | O_TRUNC, mode);
    if(fd < 0) {
        perror("file write failed");
        exit(1);
    }
    for(int i = 0; i < num_files; i++) {
        int file_id = rand() % 1000;
        char printf_command[20], filename[FILENAME_LEN + 1]; //plus one for null
        snprintf(printf_command, 20, "data%%0%dd.dat", FILENAME_LEN - 8);
        snprintf(filename, FILENAME_LEN + 1, printf_command, file_id);
        printf("outputing file %s\n", filename);
        write(fd, filename, FILENAME_LEN);
        sleep(3);
    }
    close(fd);
    printf("producer finished\n");
}
