/*
Copyright (c) 2019 University of Oregon
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdio.h>
#include <stdlib.h>

#include "matmult_initialize.h"
#define TAU_USE_STUBS
#include "taustubs.h"

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#ifdef PTHREADS
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
/*** NOTE THE ATTR INITIALIZER HERE! ***/
pthread_mutex_t mutexsum;
#endif /* PTHREADS */

#ifndef MATRIX_SIZE
#define MATRIX_SIZE 512
#endif

#define NRA MATRIX_SIZE                 /* number of rows in matrix A */
#define NCA MATRIX_SIZE                 /* number of columns in matrix A */
#define NCB MATRIX_SIZE                 /* number of columns in matrix B */

double** allocateMatrix(int rows, int cols) {
  TAU_START_FUNC();
  int i;
  double **matrix = (double**)malloc((sizeof(double*)) * rows);
  for (i=0; i<rows; i++) {
    matrix[i] = (double*)malloc((sizeof(double)) * cols);
  }
  TAU_STOP_FUNC();
  return matrix;
}

void freeMatrix(double** matrix, int rows) {
  TAU_START_FUNC();
  int i;
  for (i=0; i<rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
  TAU_STOP_FUNC();
}

// cols_a and rows_b are the same value
void compute(double **a, double **b, double **c, int rows_a, int cols_a, int cols_b) {
  TAU_START_FUNC();
  int i,j,k;
  {
    /*** Do matrix multiply sharing iterations on outer loop ***/
    /*** Display who does which iterations for demonstration purposes ***/
    for (i=0; i<rows_a; i++) {
      for(j=0; j<cols_b; j++) {
        for (k=0; k<cols_a; k++) {
          c[i][j] += a[i][k] * b[k][j];
        }
      }
    }
  }   /*** End of parallel region ***/
  TAU_STOP_FUNC();
}

void compute_interchange(double **a, double **b, double **c, int rows_a, int cols_a, int cols_b) {
  TAU_START_FUNC();
  int i,j,k;
  {
    /*** Do matrix multiply sharing iterations on outer loop ***/
    /*** Display who does which iterations for demonstration purposes ***/
    for (i=0; i<rows_a; i++) {
      for (k=0; k<cols_a; k++) {
        for(j=0; j<cols_b; j++) {
          c[i][j] += a[i][k] * b[k][j];
        }
      }
    }
  }   /*** End of parallel region ***/
  TAU_STOP_FUNC();
}

double do_work(void) {
  TAU_START_FUNC();
  double **a,           /* matrix A to be multiplied */
  **b,           /* matrix B to be multiplied */
  **c;           /* result matrix C */
  a = allocateMatrix(NRA, NCA);
  b = allocateMatrix(NCA, NCB);
  c = allocateMatrix(NRA, NCB);  

/*** Spawn a parallel region explicitly scoping all variables ***/

  initialize(a, NRA, NCA);
  initialize(b, NCA, NCB);
  initialize(c, NRA, NCB);

  compute(a, b, c, NRA, NCA, NCB);
  compute_interchange(a, b, c, NRA, NCA, NCB);

  double result = c[0][1];

  freeMatrix(a, NRA);
  freeMatrix(b, NCA);
  freeMatrix(c, NCA);

  TAU_STOP_FUNC();
  return result;
}

#ifdef PTHREADS
int busy_sleep() {
  TAU_START_FUNC();
  int i, sum = 0;
  for (i = 0 ; i < 100000000 ; i++) {
    sum = sum+i;
  }
  return sum;
  TAU_STOP_FUNC();
}

void * threaded_func(void* UNUSED(data))
{
  TAU_REGISTER_THREAD();
  TAU_START_FUNC();
  // compute
  int i;
  for (i = 0 ; i < 1 ; i++) {
  do_work();
  }

#ifdef APP_DO_LOCK_TEST
  int rc;
  int sum = 0;
  // test locking - sampling should catch this
  if ((rc = pthread_mutex_lock(&mutexsum)) != 0)
  {
    errno = rc;
    perror("thread lock error");
    exit(1);
  }
  fprintf(stderr,"Thread 'sleeping'...\n"); fflush(stderr);
  sum += busy_sleep();
  fprintf(stderr,"Thread 'awake'...\n"); fflush(stderr);
  if ((rc = pthread_mutex_unlock(&mutexsum)) != 0)
  {
    errno = rc;
    perror("thread unlock error");
    exit(1);
  }
  pthread_exit((void*) 0);
#endif // APP_DO_LOCK_TEST
  TAU_STOP_FUNC();
  return NULL;
}
#endif // PTHREADS

int main (int UNUSED(argc), char* UNUSED(argv[])) 
{
  TAU_START_FUNC();
#ifdef PTHREADS
  int ret;
  pthread_t       tid1, tid2, tid3;
  pthread_mutexattr_t Attr;
  pthread_mutexattr_init(&Attr);
#ifndef TAU_CRAYCNL
  pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_ERRORCHECK);
#endif /* TAU_CRAYCNL */
  if (pthread_mutex_init(&mutexsum, &Attr)) {
   printf("Error while using pthread_mutex_init\n");
  }
#endif /* PTHREADS */

#ifdef PTHREADS
  ret = pthread_create(&tid1, NULL, threaded_func, NULL);
  if (ret) {
    printf("Error: pthread_create (1) fails ret = %d\n", ret);
    exit(1);
  }   

  ret = pthread_create(&tid2, NULL, threaded_func, NULL);
  if (ret) {
    printf("Error: pthread_create (2) fails ret = %d\n", ret);
    exit(1);
  }   

  ret = pthread_create(&tid3, NULL, threaded_func, NULL);
  if (ret) {
    printf("Error: pthread_create (3) fails ret = %d\n", ret);
    exit(1);
  }   

#endif /* PTHREADS */

/* On thread 0: */
  int i;
  for (i = 0 ; i < 1 ; i++) {
  do_work();
  }

#ifdef PTHREADS
  ret = pthread_join(tid1, NULL);
  if (ret) {
    printf("Error: pthread_join (1) fails ret = %d\n", ret);
    exit(1);
  }   

  ret = pthread_join(tid2, NULL);
  if (ret) {
    printf("Error: pthread_join (2) fails ret = %d\n", ret);
    exit(1);
  }   

  ret = pthread_join(tid3, NULL);
  if (ret) {
    printf("Error: pthread_join (3) fails ret = %d\n", ret);
    exit(1);
  }   

  pthread_mutex_destroy(&mutexsum);
#endif /* PTHREADS */

  printf ("Done.\n");

  TAU_STOP_FUNC();
  return 0;
}

