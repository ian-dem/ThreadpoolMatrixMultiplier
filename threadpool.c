/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 100
#define NUMBER_OF_THREADS 5

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    // function should be replaced once client.c is worked on. 
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;

// the worker bees
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    return 0;
}

// initialize the thread pool
void pool_init(void)
{   
    int i; 
    for (i = 0; i < NUMBER_OF_THREADS; i++){
        if (pthread_create(&bee[i],NULL,worker,NULL) != 0){
            perror("Failed to create thread");
        }
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
     int i; 
    for (i = 0; i < NUMBER_OF_THREADS; i++){
        if (pthread_join(bee[i],NULL) != 0){
            perror("Failed to shut down thread");
        }
    }
}
