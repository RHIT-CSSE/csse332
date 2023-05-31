#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_SIZE 2048

double **alloc_mat(unsigned long n)
{
  int i;
  double **mat = malloc(n*sizeof(double*));

  for(i = 0; i < n; i++) {
    mat[i] = malloc(n*sizeof(double));
  }

  return mat;
}

double **rand_mat(unsigned long n)
{
  int i,j;
  double **mat = alloc_mat(n);

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      mat[i][j] = drand48();
    }
  }

  return mat;
}

void free_mat(double **mat, unsigned long n)
{
  int i;

  for(i = 0; i < n; i++) {
    free(mat[i]);
  }
  free(mat);
}

void naive_mult(double **a, double **b, double **c, unsigned long n)
{
  struct timespec start, finish;
  int i, j, k;
  double elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++)
      for(k = 0; k < n; k++)
        c[i][j] = c[i][j] + a[i][k]*b[k][j];


  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = finish.tv_sec - start.tv_sec;
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  printf("Time elapsed for naive matrix multiplication is %.12f seconds\n",
         elapsed);
}

void naive_mult2(double **a, double **b, double **c, unsigned long n)
{
  struct timespec start, finish;
  int i, j, k;
  double elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i = 0; i < n; i++)
    for(k = 0; k < n; k++)
      for(j = 0; j < n; j++)
        c[i][j] = c[i][j] + a[i][k]*b[k][j];


  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = finish.tv_sec - start.tv_sec;
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  printf("Time elapsed for weird matrix multiplication is %.12f seconds\n",
         elapsed);
}


int main(int argc, char **argv)
{
  unsigned long n = DEFAULT_SIZE;
  double **a, **b, **c;

  srand48(time(NULL));
  if(argc > 1) {
    n = atol(argv[1]);
  }

  // grab a few matrices
  a = rand_mat(n);
  b = rand_mat(n);
  c = alloc_mat(n);

  naive_mult(a, b, c, n);

  // free and realloc so that not to use same cache lines
  free_mat(a, n);
  free_mat(b, n);
  free_mat(c, n);

  a = rand_mat(n);
  b = rand_mat(n);
  c = alloc_mat(n);


  naive_mult2(a, b, c, n);

  // done
  free_mat(a, n);
  free_mat(b, n);
  free_mat(c, n);
}
