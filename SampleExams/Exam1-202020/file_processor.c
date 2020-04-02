#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "file.h"

void setup_for_processing() {
    printf("file processor starting up\n");
    sleep(3);
    printf("file processor ready\n");
}

void process_file(char* filename) {
    printf("file processor processing %s\n", filename);

    // this sets up our random generator
    // time_t t;
    // srand(getpid());
    //
    // // currently this is set to error half the time
    // // feel free to edit that for your testing
    //
    // if(rand() % 2 == 0) {
    //    printf("error discovered processing %s\n", filename);
    //    // here's where you should add your code to do the "right thing" on an error
    //    // I think exiting with a particular code is easiest
    //
    //}

    sleep(3);
    printf("file processor done processing %s\n", filename);
}

int main(int argc, char** argv) {
    char* example_names[] = {"data1.dat", "data2.dat"};
    printf("master processor started\n");

    
    const int num_files_to_create = 2;
    // make your life easier by keeping this string in sync
    // with the variable above
    const char* num_files_to_create_str = "2";
    
    int pipes_out[num_files_to_create];

    int fork_result;
    
    
    char filename[FILENAME_LEN + 1];
    filename[FILENAME_LEN] = 0; //null at end so its a vaid string
                                //after reading


    int fd = open("files.dat", O_RDONLY);
    if(fd < 0) {
        perror("could not open files.dat");
        exit(2);
    }

    
    for(int i = 0; i < num_files_to_create; i++) {
        int read_result = read(fd, filename, FILENAME_LEN);
        if(read_result != FILENAME_LEN) {
            perror("error reading file");
            exit(3);
        }
        setup_for_processing();
        process_file(filename);
    }
    
    printf("master processor finished. %d files processed successfully.\n", num_files_to_create);
}
