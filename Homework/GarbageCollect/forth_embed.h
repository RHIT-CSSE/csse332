#include <stdint.h>

#define STACK_SIZE 1000
#define DATA_AREA_SIZE 16384
#define BUFFER_SIZE 128

struct forth_data {

    /* these special variables must be kept in sync
       with the list of offsets in jonesforth.S
     */
    void* saved_registers[4];
    void* stack_top; //this is a register too but we access it
    void* rstack_top;
    void* rstack_bot;
    void* stack_bot;
    int32_t state;
    void* here;
    void* latest;
    char* output_max;
    char* output_current;
    int32_t base;

    int32_t emit_scratch;
    int32_t intepret_is_lit;

    char*  input_current;
    int32_t process_id;

    char wordbuf[32];
};

// an expanded struct with defaults for all the various data regions
// forth needs. If you want manage forths non-static memory regions
// yourself, use data rather than data expanded.
struct forth_data_expanded {

    struct forth_data f;
    
    int32_t return_stack[STACK_SIZE];
    int32_t data_area[16384];
    int32_t stack[STACK_SIZE];

    
};

void initialize_forth_data(struct forth_data *data,
                           void* rstack_bottom,
                           void* data_top,
                           void* stack_bottom);

void initialize_forth_data_expanded(struct forth_data_expanded *data);

// completely loads functions from jonesforth.f
// note: exits on error
void load_starter_forth(struct forth_data *mem);

void cfoo();

// these are defined in assembly

// this is the function that starts forth for the first time
extern void fstart();

// the is the function the continues forth after a pause
// its return value indicates why forth returned
#define FCONTINUE_YIELD 1
#define FCONTINUE_INPUT_DONE 2
#define FCONTINUE_ERROR 3
#define FCONTINUE_OUTPUT_FLUSH 4

// this runs the "input" forth code, using the given buffer for output
// if you set the input to NULL, it will leave any existing input
// if you set the output to NULL, it will use the existing output
int f_run(struct forth_data *data, char *input, char* output, int max_output_len);

