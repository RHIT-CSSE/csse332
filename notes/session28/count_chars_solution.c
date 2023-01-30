#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

// constants
#define NUM_FILES 5
#define NUM_THREADS 5
#define FILE_SIZE 10 * 1024

// the sum of all e characters in the all files
volatile int sum = 0;

// locking mechanisms
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * Transform a file from hex strings into a set of ASCII characters.
 *
 * @tid     The id of the thread making this transformation.
 * @fname   The name of the file to read the hex characters from.
 * @ofname  The name of the output file to write to.
 *
 * This function will read the content of fname as a set hex strings and will
 * transform those into ASCII characters. It will then write those ASCII
 * characters into another file of name ofname.
 *
 * @warning Do not edit this function, all generated files will be cleaned up
 * when doing a `make clean`.
 */
void transform_file(int tid, const char *fname, const char *ofname) {
  FILE *fp, *wfp;
  char hexstr[5]; // each hex string is 0xab and null char
  char c;

  fp = fopen(fname, "r");
  if(!fp) {
    // kill everything upon failing to read the file
    fprintf(stderr, "Could not open file %s correcty\n", fname);
    exit(EXIT_FAILURE);
  }

  wfp = fopen(ofname, "w");
  if(!wfp) {
    // kill everything upon failing to open the output file
    fprintf(stderr, "Could not open file %s correcty\n", ofname);
    exit(EXIT_FAILURE);
  }

  while(fgets(hexstr, 5, fp)) {
    // skip over a new line if it's there
    if(!strcmp(hexstr, "\n"))
      continue;
    c = (char)strtoul(hexstr, 0, 16);
    fputc(c, wfp);
  }

  fclose(fp);
  fclose(wfp);
}

/**
 * Read a file into a character buffer.
 *
 * @buffer    The buffer to read into.
 * @fname     The name of the file to read from.
 *
 * This function will read the content of the file fname and then store them
 * into the buffer buffer. It will read at most FILE_SIZE - 1 bytes from the
 * file.
 */
void read_file_to_buffer(char *buffer, const char *fname) {
  FILE *fp;

  fp = fopen(fname, "r");
  if(!fp) {
    fprintf(stderr, "Cannot open file %s, serious problems have happened!",
            fname);
    exit(EXIT_FAILURE);
  }
  fgets(buffer, FILE_SIZE, fp);
  fclose(fp);
}

// TODO: ADD YOUR THREADING STRUCTURES AND THREADING FUNCTIONS HERE
struct thread_arguments {
  int tid;
  const char *fname;
  const char *ofname;
};

void *count_fn(void *arg) {
  struct thread_arguments *args = (struct thread_arguments *)arg;
  char buff[FILE_SIZE];
  int i, local_count = 0;

  printf("[Counting Thread %d] Counting 'e's in file %s\n", args->tid,
         args->ofname);
  read_file_to_buffer(buff, args->ofname);
  for(i = 0; i < FILE_SIZE; i++) {
    if(buff[i] == 'e') {
      local_count++;
    }
  }

  pthread_mutex_lock(&lock);
  sum += local_count;
  pthread_mutex_unlock(&lock);

  return 0;
}

void *thread_fn(void *arg) {
  struct thread_arguments *args = (struct thread_arguments *)arg;
  pthread_t th;

  printf("[Thread %d] Working on file %s\n", args->tid, args->fname);
  transform_file(args->tid, args->fname, args->ofname);

  // now create the next thread and wait for it
  pthread_create(&th, 0, count_fn, arg);
  pthread_join(th, 0);

  return 0;
}

int main(int argc, char **argv) {
  const char *file_names[] = {"files/file1.dat", "files/file2.dat",
                              "files/file3.dat", "files/file4.dat",
                              "files/file5.dat"};

  const char *ofnames[] = {"files/output_file1.txt", "files/output_file2.txt",
                           "files/output_file3.txt", "files/output_file4.txt",
                           "files/output_file5.txt"};
  pthread_t threads[NUM_THREADS];
  struct thread_arguments arguments[NUM_THREADS];
  int i, j, seq_sum = 0;
  char buff[FILE_SIZE];
  struct timeval startt, stopt;
  suseconds_t time_cost;

  // Step 1: Do the stuff sequentially
  gettimeofday(&startt, NULL);
  for(i = 0; i < NUM_FILES; i++) {
    transform_file(i + 1, file_names[i], ofnames[i]);
  }

  // Step 1: Count the stuff
  for(i = 0; i < NUM_FILES; i++) {
    read_file_to_buffer(buff, ofnames[i]);
    for(j = 0; j < FILE_SIZE; j++) {
      if(buff[j] == 'e') {
        seq_sum++;
      }
    }
  }
  printf("Sequential sum approach computed %d 'e' chars\n", seq_sum);
  gettimeofday(&stopt, NULL);
  time_cost = stopt.tv_usec - startt.tv_usec;
  printf("Sequential approach finished in %ld usecs.\n", time_cost);

  /////////////////////////////////////////////////////////////////////////////
  // DO NOT CHANGE THE CODE ABOVE THIS
  /////////////////////////////////////////////////////////////////////////////

  gettimeofday(&startt, NULL);

  // TODO: ADD YOUR PARALLEL CODE HERE
  for(i = 0; i < NUM_THREADS; i++) {
    arguments[i].tid = i + 1;
    arguments[i].fname = file_names[i];
    arguments[i].ofname = ofnames[i];
    pthread_create(&threads[i], 0, thread_fn, &arguments[i]);
  }

  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], 0);
  }
  // END OF YOUR ADDED CODE

  printf("Parallel sum approach computed %d 'e' chars\n", sum);
  gettimeofday(&stopt, NULL);
  time_cost = stopt.tv_usec - startt.tv_usec;
  printf("Parallel approach finished in %ld usecs.\n", time_cost);

  exit(EXIT_SUCCESS);
}
