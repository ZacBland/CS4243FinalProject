#include <stdio.h>
#include <stdlib.h>

struct node
{
    int socket;
    struct node *next;
};
typedef struct node node;

struct queue
{
    int max;
    int num;
    node *head;
    node *tail;
};
typedef struct queue queue;

void init(queue *q, int max){
    /* init
    Initializes a queue struct

    Args:
        q (queue): Queue struct
    */
    q->num = 0;
    q->max = max;
    q->head = NULL;
    q->tail = NULL;
}

int isEmpty(queue *q){
    /* isEmpty
    Checks if queue is empty

    Args:
        q (queue): Queue struct to check if empty.

    Returns:
        (int): 1 or 0 based if the queue is empty.
    */
    return (q->tail == NULL);
}

int isFull(queue *q){
    /**
     * Checks if queue is full
     * 
     * Args:
     *      q (queue): Queue struct to check if full.
     * 
     * Returns: 
     *      (int): returns 1 if full, 0 if not
     */
    return (q->num >= q->max);
}

void add(queue *q, int socket){
    /* add
    Adds node to end of queue.

    Args:
        q (queue): Queue struct to add to.
        socket (int): Socket id number.
    */

    node *temp;
    temp = malloc(sizeof(node));

    temp->socket = socket;
    temp->next = NULL;

    if(!isEmpty(q)){
        q->tail->next = temp;
        q->tail = temp;
    }
    else{
        q->head = q->tail = temp;
    }
    q->num++;
}

int pop(queue *q){
    /* pop
    Pops the node on the top of the queue.

    Args:
        q (queue): Queue struct to pop from.

    Returns:
        socket (int): data from popped node.
    */

    node *temp;
    
    int socket = q->head->socket;
    temp = q->head;

    q->head = q->head->next;
    q->num--;

    free(temp);

    if(q->num == 0){
        q->tail = NULL;
    }

    return socket;
}