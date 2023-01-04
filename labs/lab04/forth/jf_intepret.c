#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include "forth_embed.h"

void executeForth(struct forth_data_expanded *mem, int argc, char** argv) {
    int currFile = 0;
    int fresult = FCONTINUE_INPUT_DONE;
    char* file_result = NULL;
    char input_buffer[200], output_buffer[200];
    FILE* file = NULL;
    bool buffer_flushed = true;

    // TODO: needs to force a flush as we enter interative mode
    // which is not happening bcause the fgets occurs before the flush
    while(1) {
        switch(fresult) {
        case FCONTINUE_YIELD:
            //do nothing;
            printf("Forth yielded?\n");
            fresult = f_run(&mem->f, NULL, NULL, 0);
            break;
        case FCONTINUE_INPUT_DONE:
            while(1) {

                if(file == stdin) {
                    //always flush if we're interactive
                    printf("%s", output_buffer);
                    fflush(stdout);
                    buffer_flushed = true;
                }
                
                if(file != NULL) {
                    // fgets will also put a 0 at the end of the input
                    file_result = fgets(input_buffer, sizeof input_buffer, file);
                }
                
                if(file_result != NULL) {
                    //line read success!  Stop looking for input

                    // useful if we crash on load
                    // printf("EVAL: %s", input_buffer);
                    if(buffer_flushed) {
                        // if we're interactive don't buffer ouput between lines
                        fresult = f_run(&mem->f, input_buffer, output_buffer, sizeof output_buffer);
                        buffer_flushed = false;
                    } else {
                        fresult = f_run(&mem->f, input_buffer, NULL, 0);
                    }
                    break;
                } else {

                    currFile++;

                    if(currFile < argc) {
                        file = fopen(argv[currFile],"r");
                    }
                    if(currFile == argc) {
                        file = stdin;
                    }
                    if(currFile > argc) {
                        //we have no more ways to get input
                        return;
                    }

                }

            }
            break;
        case FCONTINUE_ERROR:
            printf("Error while processing command: %s\n", (char*) &mem->f.wordbuf);
            return;
        case FCONTINUE_OUTPUT_FLUSH:
            printf("%s", output_buffer);
            fflush(stdout);
            fresult = f_run(&mem->f, NULL, output_buffer, sizeof output_buffer);
            break;
        default:
            printf("Unknown forth result\n");
            return;
        }

    }

}

int main(int argc, char** argv) {

    printf("Welcome to forth! Press ^D to quit. \n");
    
    struct forth_data_expanded *mem = mmap(NULL, sizeof(struct forth_data_expanded), PROT_WRITE | PROT_EXEC,
                   MAP_ANON | MAP_PRIVATE, -1, 0);
    
    initialize_forth_data_expanded(mem);
    executeForth(mem, argc, argv);

    return 0;
}
