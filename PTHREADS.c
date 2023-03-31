#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define M 1700
#define N 800
#define P 1000
#define NUM_THREADS 4

double A[M][N], B[N][P], C[M][P];
pthread_t threads[NUM_THREADS];
int thread_args[NUM_THREADS];

void *matrix_mult(void *thread_arg) {
    int tid = *((int*) thread_arg);
    int i, j, k;
    double sum;
    int chunk_size = M / NUM_THREADS;
    int start = tid * chunk_size;
    int end = (tid == NUM_THREADS - 1) ? M : start + chunk_size;

    for (i = start; i < end; i++) {
        for (j = 0; j < P; j++) {
            sum = 0.0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    pthread_exit(NULL);
}

int main() {
    int i, j;
    void *status;

    // Initialize matrices A and B
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            B[i][j] = i + j;
        }
    }
    clock_t t;
    t = clock();
    // Create threads and perform matrix multiplication in parallel
    for (i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, matrix_mult, (void*) &thread_args[i]);
    }

    // Wait for all threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }
    t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC;
   printf("Matrix multiplication (PTHREADS) completed in %f seconds\n", time_taken);

    return 0;
}
