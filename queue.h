#ifndef QUEUE_H
#define QUEUE_H

struct node
{
    int socket;
    struct node *next;
};
typedef struct node node;

struct queue
{
    int num;
    node *head;
    node *tail;
};
typedef struct queue queue;

void init(queue *q, int max);
int isFull(queue *q);
int isEmpty(queue *q);
void add(queue *q, int socket);
int pop(queue *q);

#endif