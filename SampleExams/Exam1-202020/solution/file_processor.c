#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME_LEN 11

/**************************

This is the solution file, not where you should start coding.

 */

void process_file(char* filename) {
    printf("file processor processing %s\n", filename);

    // this sets up our random generator
    time_t t;
    srand(getpid());

    if(rand() % 2 == 0) {
        printf("error discovered processing %s\n", filename);
        exit(99);
    }


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
    
    for(int i = 0; i < num_files_to_create; i++) {
        int pipefds[2];
        if(pipe(pipefds) < 0) {
            perror("pipe failed");
        }
        fork_result = fork();
        if(fork_result < 0) {
            perror("fork error");
            exit(4);
        }
        if(fork_result == 0) {
            close(pipefds[1]);
            char filename[FILENAME_LEN + 1];
            filename[FILENAME_LEN] = 0;
            
            printf("file processor starting up\n");
            sleep(3);
            printf("file processor ready\n");
            
            int read_result = read(pipefds[0], filename, FILENAME_LEN);
            if(read_result != FILENAME_LEN) {
                perror("error reading file");
                exit(3);
            }
            process_file(filename);
            exit(0);
        }
        close(pipefds[0]);
        pipes_out[i] = pipefds[1];
    }
    
    fork_result = fork();
    if(fork_result == 0) {
        execl("./file_producer","./file_producer", num_files_to_create_str, NULL);
        perror("exec returned");
        exit(1);
    } else {
        int status;
        wait(&status);
        if(WEXITSTATUS(status) != 0) {
            printf("bad producer result means quit\n");
            exit(1);
        }
    }

    int fd = open("files.dat", O_RDONLY);
    if(fd < 0) {
        perror("could not open files.dat");
        exit(2);
    }

    char filename[FILENAME_LEN + 1];
    filename[FILENAME_LEN] = 0; //null at end so its a vaid string
                                //after reading

    for(int i = 0; i < num_files_to_create; i++) {
        int read_result = read(fd, filename, FILENAME_LEN);
        if(read_result != FILENAME_LEN) {
            perror("error reading file");
            exit(3);
        }
        if(write(pipes_out[i], filename, FILENAME_LEN) != FILENAME_LEN) {
            perror("error writing to pipe");
            exit(5);
        }
        
    }

    int fails = 0;
    for(int i = 0; i < num_files_to_create; i++) {
        int status;
        wait(&status);
        if(WEXITSTATUS(status) != 0) {
            fails++;
        }
    }
    printf("master processor finished. %d files processed successfully.\n", num_files_to_create - fails);
}
