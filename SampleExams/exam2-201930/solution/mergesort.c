
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/stat.h>

#define NUM_SORTERS 4
// we break our input file into chunks of CHUNK_SIZE or less
#define CHUNK_SIZE 10
// the max file we can sort is CHUNK_SIZE*MAX_CHUNKS, but depending on
// newlines it might stop for smaller files
#define MAX_CHUNKS 100

// this holds our input chunks
char *chunks[MAX_CHUNKS];

sem_t merge_sem;
sem_t count_sem;
int count_for_merging;

void sort_chunk(int* chunk_num_p) {    

    int chunk_num = *chunk_num_p;

    while(chunk_num <= MAX_CHUNKS && chunks[chunk_num] != NULL) {

        char* data_to_sort = chunks[chunk_num];
        printf("thread %d sorting '%s'\n", (*chunk_num_p) + 1, data_to_sort);
    
        // then sort the characters with a crude bubblesort
        int length = strlen(data_to_sort);
        for(int i = 0; i < length; i++) {
            for(int j = 0; j < length - 1; j++) {
                if(data_to_sort[j] > data_to_sort[j + 1]) {
                    char bigger = data_to_sort[j];
                data_to_sort[j] = data_to_sort[j + 1];
                data_to_sort[j + 1] = bigger;
                }
            }
        }
        //make this take a little longer so parallelism obvious
        sleep(1);
        chunk_num += NUM_SORTERS;
    }

}

// takes two malloced sorted strings, mallocs a output string
// merges the two inputs into output and frees the inputs
// returns output string
char* merge_strings(char* str1, char* str2) {
    int total_len = strlen(str1) + strlen(str2);
    char* output = malloc( total_len + 1);
    char* pos1 = str1;
    char* pos2 = str2;
    for(int i = 0; i < total_len; i++) {
        if(*pos2 == '\0') {
            output[i] = *pos1;
            pos1++;
            continue;
        }
        if(*pos1 == '\0') {
            output[i] = *pos2;
            pos2++;
            continue;
        }
        if(*pos1 < *pos2) {
            output[i] = *pos1;
            pos1++;
            continue;
        } else {
            output[i] = *pos2;
            pos2++;
            continue;
        }
    }
    output[total_len] = '\0';
    //make this take a little longer so parallelism obvious
    printf("merge of %s and %s produces %s\n", str1, str2, output);
    sleep(1);

    // note that if you get segfaults, it might have to do with
    // freeing the same string more than once.
    free(str1);
    free(str2);
    return output;
}

void merge_thread() {
    while(1) {
        int pos;
        char *merge1, *merge2;

        sem_wait(&count_sem);
        count_for_merging -= 2;
        sem_post(&count_sem);
    
        sem_wait(&merge_sem);

        for(pos = 0; pos < MAX_CHUNKS; pos++) {
            if(chunks[pos] != NULL) {
                merge1 = chunks[pos];
                usleep(100); // if you uncomment this line you will make the concurrency bug in part 3 obvious
                chunks[pos] = NULL; // to prevent re-merging the same data
                break;
            }
        }
    
        if(pos == MAX_CHUNKS) {
            printf("ERROR! A merger can't find a corresponding chunk to merge\n");
            exit(4);
        }

        int pos2 = 0;
        for(; pos2 < MAX_CHUNKS; pos2++) {
            if(chunks[pos2] != NULL) {
                merge2 = chunks[pos2];
                //usleep(100); // if you uncomment this line you will make the concurrency bug in part 3 obvious
                chunks[pos2] = NULL; // to prevent re-sorting the same data_to_sort
                break;
            }
        }

        if(pos2 == MAX_CHUNKS) {
            printf("ERROR! A merger can't find a corresponding chunk to merge\n");
            exit(5);
        }
        sem_post(&merge_sem);
    
        printf("merging '%s' and '%s'\n", merge1, merge2);
        char* output = merge_strings(merge1, merge2);

        // find an open position, ideally at the end of the chunk list for output
        sem_wait(&merge_sem);
        for(pos = MAX_CHUNKS - 1; pos >= 0; pos--) {
            if(chunks[pos] == NULL) {
                chunks[pos] = output;
                break;
            }
        }
        sem_post(&merge_sem);

        sem_wait(&count_sem);
        count_for_merging++;
        if(count_for_merging == 1) {
            sem_post(&count_sem);
            return;
        } else {
            sem_post(&count_sem);
        }
    }

}

int main(int argc, char** argv) {

    if(argc < 2) {
        printf("you must provide a filename to sort\n");
        exit(2);
    }

    char *filename = argv[1];

    for(int i = 0; i < MAX_CHUNKS; i++) {
        // initialize our string array to null
        chunks[i] = NULL;
    }

    FILE *stream = fopen(filename, "r");
    if(stream == NULL) {
        perror("error opening file");
        exit(1);
    }


    int chunks_read = 0;
    for(; chunks_read < MAX_CHUNKS; chunks_read++) {
        char *chunk = malloc(CHUNK_SIZE + 1);
        char *result = fgets(chunk, CHUNK_SIZE + 1, stream);
        if(result == NULL) {
            // we are at end of file
            free(chunk);
            break;
        }
        // we want to ignore newline characters
        int last_index = strlen(chunk) - 1;
        if(chunk[last_index] == '\n') {
            chunk[last_index] = '\0';
        }
        chunks[chunks_read] = chunk;
    }


    int input[chunks_read];
    pthread_t threads[chunks_read];
    
    for(int i = 0; i < NUM_SORTERS; i++) {
        input[i] = i;
        pthread_create(&threads[i], NULL, (void * (*)(void *)) sort_chunk, &input[i]); 
    }

    for(int i = 0; i < NUM_SORTERS; i++) {
        pthread_join(threads[i], NULL);
    }

    
    count_for_merging = chunks_read;
    sem_init(&merge_sem, 0, 1);
    sem_init(&count_sem, 0, 1);

    for(int i = 0; i < chunks_read/2; i++) {
        pthread_create(&threads[i], NULL, (void * (*)(void *)) merge_thread, NULL); 
    }

    for(int i = 0; i < chunks_read/2; i++) {
        pthread_join(threads[i], NULL);
    }

    // print everything in the output array
    for(int i = 0; i < MAX_CHUNKS; i++) {
        if(chunks[i] != NULL) {
            printf("output chunk: %s\n", chunks[i]);
        }
    }

    
    printf("done\n");
    // we should free the memory we malloced, but the program is
    // exiting anyway and I don't want to introduce unneeded
    // complications into your exam code
    
}
