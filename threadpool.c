/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"
#include "Queue.h"

#define QUEUE_SIZE 100
#define NUMBER_OF_THREADS 5

#define TRUE 1

// # of spots taken in the queue
int  queueSpace = 0;

//The work queue
queue_t worktodo;
queue_t *ptrworktodo = &worktodo;


// the worker bees
pthread_t bee[NUMBER_OF_THREADS];


// insert a task into the queue
int enqueue(task t) 
{
    if (queueSpace < QUEUE_SIZE)
    {
        // Add the task t to the heap memory 
        task *ptrtot = malloc(sizeof(t));
        *ptrtot = t;

        // Create the Node to pass 
        node_t *ptrnewNode = malloc(sizeof(node_t));
        ptrnewNode -> next = NULL;
        ptrnewNode -> data = ptrtot;

        // Add the node into the queue
        add(ptrworktodo,ptrnewNode);
        queueSpace++;
        return 0;
    }

    return 1;
}
// remove a task from the queue
task dequeue() 
{
    if (queueSpace != 0){
        // Pointer to the head data before it is pop'd 
        task *ptrHeadData = ptrworktodo -> head -> data;
        task headData = *ptrHeadData;

        pop(ptrworktodo);

        // Freeing the OLD head data
        free(ptrHeadData);

        queueSpace--;
        return headData;
    }
    
    printf("Queue Space is empty!");
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
