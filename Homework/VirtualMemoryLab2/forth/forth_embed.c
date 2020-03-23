#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "forth_embed.h"

// these externs are in the assembly code but aren't intended to be
// used by anything directly
extern int64_t fcontinue(struct forth_data *data);

// the is last assembly defined forth function.  it is the initial
// value of LATEST, which is set during C struct initialization
extern void* name_SYSCALL0;

static int32_t process_counter;

void cfoo() {
    printf("cFOO\n");
}

void initialize_forth_data(struct forth_data *data,
                           void* rstack_bottom,
                           void* data_top,
                           void* stack_bottom) {
    data->rstack_top = data->rstack_bot = rstack_bottom;
    data->stack_bot = stack_bottom;
    data->stack_top = data->stack_bot - sizeof(void*);
    *((void**)data->stack_top) = fstart;

    data->state = 0;
    data->latest = name_SYSCALL0;
    data->here = data_top;
    data->base = 10;
    data->process_id = ++process_counter;
}


void initialize_forth_data_expanded(struct forth_data_expanded *data) {
    
    initialize_forth_data(&data->f,
                          data->return_stack + sizeof data->return_stack / sizeof *(data->return_stack),
                          data->data_area,
                          data->stack + sizeof data->stack / sizeof *(data->stack));
}

int64_t f_run(struct forth_data *data, char *input, char *output, int max_output_len) {

    if(output != NULL) {

        data->output_current = output;
        data->output_max = output + max_output_len - 1;
    }
    if(input != NULL) {
        data->input_current = input;
    }
    int64_t result = fcontinue(data);
    *(data->output_current) = '\0';
    return result;
}

void load_starter_forth_at_path(struct forth_data *mem, char* path) {

    char input_buffer[200];
    char output_buffer[200];
    char* orig_output = mem->output_current;
    char* orig_output_max = mem->output_max;
    mem->output_current = output_buffer;
    mem->output_max = output_buffer + sizeof output_buffer;
    
    FILE* file = fopen(path,"r");
    if(file == NULL) {
        printf("error loading %s\n", path);
        exit(1);
    }

    int fresult = FCONTINUE_INPUT_DONE;
    char* file_result = NULL;
    
    while(1) {
        
        switch(fresult) {
            
        case FCONTINUE_INPUT_DONE:
            file_result = fgets(input_buffer, 200, file);
            if(file_result == NULL) {
                mem->output_current = orig_output;
                mem->output_max = orig_output_max;
                return;
            }
            mem->input_current = input_buffer;
            break;
        case FCONTINUE_OUTPUT_FLUSH:
            //we ignore output, so just reset the output buffer
            mem->output_current = output_buffer;
            break;
        case FCONTINUE_ERROR:
            printf("error %s load response.  (Maybe while handling %s?)\n", path, mem->wordbuf);
            exit(2);
            
        default:
            printf("unexpected %s load response\n", path);
            exit(2);
        }
        fresult = fcontinue(mem);
    }
    // code never arrives here
}
    
void load_starter_forth(struct forth_data *mem) {
    load_starter_forth_at_path(mem, "jonesforth.f");
}
