
void initialize_forths();
int create_forth(char* code);

struct run_output {
    char output[200];
    int result_code;
    int forked_child_id;
};

struct run_output run_forth_until_event(int forth_to_run);

int get_used_pages_count();
