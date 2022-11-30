#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAXCHAR 1024
#define PORT 8080
#define IP "127.0.0.1"

//#include "../shared/utils.h"

#include "queue.h"
#include "semaphore.h"
 
char readArray[720][7 * 250];

int num_of_assistants = 2;
int num_of_sofas = 1;
queue *q;
int * assist_sems;
int * couch_sems;


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

int main() {
 
    int server_sock, client_sock;
    struct sockaddr_in serveraddr = {0}, client_addr = {0};
    socklen_t addr_size;
    int n;
    char buffer[MAXCHAR];
    pid_t clientPID;
 
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
 
    if (server_sock < 0) {
        perror("[-] Socket error");
        exit(1);
    }
 
    printf("[+] TCP server socket created.\n");
 
    memset(&serveraddr, '\0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = PORT;
    serveraddr.sin_addr.s_addr = inet_addr(IP);
 
    n  = bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (n < 0) {
        perror("[-] Bind error");
        exit(1);
    }
 
    printf("[+} Bind to the port number: %d\n", PORT);
 
    if (listen(server_sock, 5) ==0) {
        printf("[+] Listening...\n");
    } else {
        printf("[-] Bind error...\n");
    }
 
    // TODO: Read and store options.txt
    //struct Option* options = readOptions("../options.txt", 2);

    //Create semaphore arrays
    assist_sems = create_semaphores(num_of_assistants);
    couch_sems = create_semaphores(num_of_sofas);
    
    //Initialize queue
    q = malloc(sizeof(queue));
    init(q);

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

    for (;;) {

        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr_in*)&client_addr, &addr_size);
        if (client_sock < 0) {
            exit(1);
        }

        printf("[+] Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        //add client to queue
        add(q, client_sock);
    }

    //Join all threads
    pthread_join(&couch_manager, NULL);
    pthread_join(&assistant_manager, NULL);
    for(int i = 0; i < num_of_assistants; i++){
        pthread_join(&assistants[i], NULL);
    }
 
    close(client_sock);
 
    return 0;
}
 
 
