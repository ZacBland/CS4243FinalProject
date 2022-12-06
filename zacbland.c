/*
Zac Bland
zac.bland@okstate.edu
Semaphore and Queue management
*/

// SERVER.C
#include "queue.h"
#include "semaphore.h"
 
char readArray[720][7 * 250];

int num_of_assistants;
int num_of_sofas;
int queue_max;
queue *q;
int * assist_sems;
int * couch_sems;
int queue_count = 0;

void manage_couches(){
    /**
     * Thread function to manage couch semaphores
     */

    //Loop indefinitely
    while(1){
        
        //wait for queue to fill
        while(isEmpty(q));

        //get next client in queue
        int next = pop(q);
        queue_count--;
        
        sleep(1);
        //send message to client
        if(send(next, "You are next in line for a sofa.\n", 50, 0) < 0){
            perror("send");
        }

        //wait for couch to become available
        int couch;
        while((couch = check_avaibility(couch_sems, num_of_sofas)) < 0);

        //put client on couch
        take(couch_sems, couch, next);

        //send message to client
        char message[50];
        sprintf(message, "Currently on Sofa #%d.\n", couch);
        if(send(next, message, 50, 0) < 0){
            perror("send");
        }
    }
}

void manage_assistants(){
    /**
     * Thread function to manage assistants.
     */
    while(1){
        //Loop through couches indefinitely
        for(int i = 0; i < num_of_sofas; i++){
            //check if couch has client
            if(couch_sems[i] != 0){
                int socket = couch_sems[i];

                sleep(1);
                //send message to client
                if(send(socket, "You are next in line for an assistant.\n", 50, 0) < 0){
                    perror("send");
                }

                //wait for next assistant to become available
                int assistant;
                while((assistant = check_avaibility(assist_sems, num_of_assistants)) < 0);
                printf("found assistant. #%d\n", assistant);

                //give back couch that client was occupying
                post(couch_sems, i);

                //send message to client
                char message[50];
                sprintf(message, "Currently with Assistant #%d.\n", assistant);
                if(send(socket, message, 50, 0) < 0){
                    perror("send");
                }

                sleep(1);
                if(send(socket, "assistant", 50, 0) < 0){
                    perror("send");
                }

                //take assistance semaphore
                take(assist_sems, assistant, socket);

            }
        }
    }
}

void assistant_thread(void* arg){
    /**
     * Assistant thread to ask user options
     */

    //get assistant index from arg
    int index = (int) arg;

    while(1){
        //wait for client socket in array
        while(assist_sems[index] == 0);
        int socket = assist_sems[index];

        //run menu options 

        //TODO : ADD MENU HERE
        while(1){
            char buffer[MAXCHAR];
            if(recv(socket, buffer, MAXCHAR, 0) < 0){
                perror("send");
            }   

            printf("%s\n", buffer);
            //exit condition
            if(atoi(buffer) == 4){
                break;
            }
        }

        //Once finished, give back assistant semaphore
        post(assist_sems, index);
    }
}

int main(){
    num_of_assistants = atoi(argv[1]);
    num_of_sofas = atoi(argv[2]);
    queue_max = atoi(argv[3]);
    
    //Create semaphore arrays
    assist_sems = create_semaphores(num_of_assistants);
    couch_sems = create_semaphores(num_of_sofas);
    
    //Initialize queue
    q = malloc(sizeof(queue));
    init(q, queue_max);

    //Create manager threads
    pthread_t couch_manager;
    pthread_t assistant_manager;
    pthread_create(&couch_manager, NULL, &manage_couches, NULL);
    pthread_create(&assistant_manager, NULL, &manage_assistants, NULL);
    
    //Create assistant threads
    pthread_t assistants[num_of_assistants];
    for(int i = 0; i < num_of_assistants; i++){
        pthread_create(&assistants[i], NULL, &assistant_thread, (void *) i);
    }

    for(;;){
        //Check if queue is full
        if(isFull(q)){
            close(client_sock);
            printf("Client rejected, queue is full!\n");
            continue;
        }

        //add client to queue
        add(q, client_sock);
        queue_count++;
        
        char msg[10];
        sprintf(msg, "%d", queue_count);

        if(send(client_sock, msg, 10, 0) < 0){
            perror("send");
        }
    }
}

// CLIENT.C
 printf("Connected to the server.\n\n");

printf("All the shop assistants are currently busy assisting the customers.\n");
printf("You are placed in the waiting room.\n");

bzero(buffer, MAXCHAR);
recv(sock, buffer, sizeof(buffer), 0);
printf("Your current number in the waiting room is %s.\n", buffer);

//SEMAPHORE.C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>


int * create_semaphores(int n){
    /*  create_semaphores
    *   Creates an array of semaphores based on the number inputted.
    *   
    *   Returns array of sem_ts
    */

    int* semaphores = (int *) malloc(sizeof(int) * n);

    for(int i = 0; i < n; i++){
        semaphores[i] = 0;
        //printf("Semaphore %d available\n", i);
    }
    
    return semaphores;
}

int check_avaibility(int * semaphores, int n){
    /**
     * @brief check if semaphore in array is available
     * 
     * @returns returns index of open semaphore
     *          returns -1 if none are available
     */
    for(int i = 0; i < n; i++){
        if(semaphores[i] == 0){
            return i;
        }
    }
    return -1;
}

void take(int * semaphores, int index, int socket){
    /**
     * @brief take control of semaphore
     * 
     */
    semaphores[index] = socket;
}
void post(int * semaphores, int index){
    /**
     * @brief give back semaphore
     * 
     */
    semaphores[index] = 0;
}


// QUEUE.C

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