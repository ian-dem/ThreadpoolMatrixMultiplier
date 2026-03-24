#include <stdio.h>
#include <stdlib.h>

/*
This is a linked list implementation of a queue, behavior you would expect of a typical linked list is the same but you will also have a pointer to the last node.
*/


typedef struct node
{
    void *data;
    struct node *next;
}
node_t;

typedef struct Queue
{
    node_t *head;
    node_t *back;
}
Queue_t;

int enqueue(Queue_t *queue, node_t *newNode)
{
    
    // If the Queue is empty then the head and back are set to the node
    if (queue -> head == NULL){
        newNode -> next = NULL;
        queue -> head = newNode;
        queue -> back = newNode;
        return 0;
    }

    newNode -> next = NULL;
    queue -> back -> next = newNode;
    queue -> back = newNode;
    
    return 0;
}

// Will only delete first node
int dequeue(Queue_t *queue){

    node_t *nodeDel;
    nodeDel = queue -> head;
    
    if (nodeDel == NULL){
        printf("Queue is empty\n");
        return 1;
    }

    // Check if there is only one node in the queue
    if (nodeDel == queue -> back)
    {
        queue -> back = NULL;
    }

    queue -> head = nodeDel -> next;
    free(nodeDel);
    return 0;

}

int main() {
    Queue_t queue;
    queue.head = NULL;
    queue.back = NULL;

    node_t *n1 = malloc(sizeof(node_t));
    node_t *n2 = malloc(sizeof(node_t));

    if (n1 == NULL || n2 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    n1->data = NULL;
    n2->data = NULL;

    enqueue(&queue, n1);
    enqueue(&queue, n2);

    printf("After enqueue: head exists? %s, back exists? %s\n",
           queue.head != NULL ? "yes" : "no",
           queue.back != NULL ? "yes" : "no");

    dequeue(&queue);
    printf("After first dequeue: head exists? %s, back exists? %s\n",
           queue.head != NULL ? "yes" : "no",
           queue.back != NULL ? "yes" : "no");

    dequeue(&queue);
    printf("After second dequeue: head exists? %s, back exists? %s\n",
           queue.head != NULL ? "yes" : "no",
           queue.back != NULL ? "yes" : "no");

    dequeue(&queue);  // tests empty queue case

    return 0;
}