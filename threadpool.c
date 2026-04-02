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

//Synchronization
pthread_mutex_t mutex_queue;
sem_t sem_workers;


// # of spots taken in the queue
int  queueSpace = 0;

//The work queue
queue_t worktodo;
queue_t *ptrworktodo = &worktodo;

//shutdown flag
int shutdown_flag = 0;

// the worker bees
pthread_t bee[NUMBER_OF_THREADS];


// insert a task into the queue
int enqueue(task t) 
{
    //Lock queue
    pthread_mutex_lock(&mutex_queue);

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

        //Unlock and signal
        pthread_mutex_unlock(&mutex_queue);
        sem_post(&sem_workers);
        return 0;
    }

    pthread_mutex_unlock(&mutex_queue);
    return 1;
}
// remove a task from the queue
task dequeue() 
{
    //lock for safety
    pthread_mutex_lock(&mutex_queue);

    //Create an empty task so we can return in case of empty queue
    task emptyTask;
    emptyTask.function = NULL;
    emptyTask.data = NULL;

    // check if queue is empty
    if (ptrworktodo->head == NULL){
        pthread_mutex_unlock(&mutex_queue);
        return emptyTask;
    }

    // Pointer to the head data before it is pop'd
    task *ptrHeadData = ptrworktodo -> head -> data;
    task headData = *ptrHeadData;

    pop(ptrworktodo);

    // Freeing the OLD head data
    free(ptrHeadData);

    queueSpace--;

    // unlock and return
    pthread_mutex_unlock(&mutex_queue);
    return headData;

}

// the worker thread in the thread pool
void *worker(void *param)
{
    while(1) {
        //wait for sem_post
        sem_wait(&sem_workers);

        // if shutdown is called, exit loop
        if (shutdown_flag)
            break;

        //grab task
        task worktodo = dequeue();

        // execute the task
        execute(worktodo.function, worktodo.data);
    }

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
    task worktodo;
    worktodo.function = somefunction;
    worktodo.data = p;

    // add task to queue
    return enqueue(worktodo);
}

// initialize the thread pool
void pool_init(void)
{
    pthread_mutex_init(&mutex_queue, 0);
    sem_init(&sem_workers, 0 ,0);

    // initialize queue
    worktodo.head = NULL;
    worktodo.back = NULL;

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

    // set shutdown flag
    shutdown_flag = 1;

    // wake up all threads so they can exit
    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        sem_post(&sem_workers);
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++){
        if (pthread_join(bee[i],NULL) != 0){
            perror("Failed to shut down thread");
        }
    }

    //clean up sync
    pthread_mutex_destroy(&mutex_queue);
    sem_destroy(&sem_workers);
}
