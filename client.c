/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "threadpool.h"

int Total_no_of_partial_product = 5; //for make to work I have defined this, you should comupte this value from the dimension of the input matrices
pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done_cond = PTHREAD_COND_INITIALIZER;

int tasks_remaining = 0;

struct data
{
    //information needed for one thread to process 
    //contains dimensions and pointers to arrays (matrices)

    // in matrix multi :
    // matrix A: with m x n 
    // matrix B: with n x p
    // resulting matrix C: m x p
    
    // row and col here represent current cell in product matrix being calculated
    int row;
    int col;

    int n;


    // matrices
    double **A;
    double **B;
    double **C;
};

void compute_partial_product(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    // perform partial product
    double sum = 0.0;
    for (int i=0; i < temp->n; i++){
        sum += temp->A[temp->row][i] * temp->B[i][temp->col];
    }
    
    temp->C[temp->row][temp->col] = sum;

    // signal complete here
    pthread_mutex_lock(&done_mutex);
    tasks_remaining--;
    if (tasks_remaining == 0){
        pthread_cond_signal(&done_cond);
    }
    pthread_mutex_unlock(&done_mutex);

}

int main(void)
{
    // allocate A, B, C
    // fill A, B
    // allocate data array 
    // get start time
    // init threadpool 
    // assign work 
    // shutdown pool
    // get finish time
    // calculate elapsed time
    // output elapsed time and resulting matrix C to file


    // in matrix multi :
    // matrix A: with m x n 
    // matrix B: with n x p
    // resulting matrix C: m x p
    int m = 5;
    int n = 6;
    int p = 4;

    double **A = malloc(m * sizeof(double *));
    for (int i=0; i<m; i++){
        A[i] = malloc(n * sizeof(double));
    }
    double **B = malloc(n * sizeof(double *));
    for (int i=0; i<n; i++){
        B[i] = malloc(p * sizeof(double));
    }    
    double **C = malloc(m * sizeof(double *));
    for (int i=0; i<m; i++){
        C[i] = malloc(p * sizeof(double));
    }

    // assign data to A and B here
    // ----- TODO -----
    // filler for now
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = i + j;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            B[i][j] = i * j;

    

    // create data array 
    Total_no_of_partial_product = m * p;
    int i = 0;
    struct data *work = malloc(sizeof(struct data) * Total_no_of_partial_product);

    for (int row = 0; row < m; row++){
        for (int col = 0; col < p; col++){
            work[i].row = row;
            work[i].col = col;
            work[i].n = n;
            work[i].A = A;
            work[i].B = B;
            work[i].C = C;
            i++;
        }
    }




    // get start time
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    tasks_remaining = Total_no_of_partial_product;
    for(i=0;i<Total_no_of_partial_product;i++)
    	pool_submit(&compute_partial_product,&work[i]);

    // may be helpful 
    //sleep(3);

    pthread_mutex_lock(&done_mutex);
    while (tasks_remaining > 0){
        pthread_cond_wait(&done_cond, &done_mutex);
    }
    pthread_mutex_unlock(&done_mutex);
    

    pool_shutdown();

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    FILE *fp = fopen("output.txt", "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "Computation Time: %.4f seconds\n\n", elapsed);
    fprintf(fp, "resulting Matrix C: \n");
    printf("Computation Time: %.4f seconds\n\n", elapsed);
    printf("resulting Matrix C: \n");

    for (int j=0; j<m; j++){
        for (int k=0; k<p; k++){
            fprintf(fp, "%.2f ", C[j][k]);
            printf("%.2f ", C[j][k]);
        }
        fprintf(fp, "\n");
        printf("\n");
    }

    fclose(fp);


    // freeing memory
    for (int i=0; i<m; i++){
        free(A[i]);
    }
    free(A);

    for (int i=0; i<n; i++){
        free(B[i]);
    }
    free(B);
    
    for (int i=0; i<m; i++){
        free(C[i]);
    }
    free(C);

    free(work);
    
    
    return 0;
}
