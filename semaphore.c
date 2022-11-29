#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>


sem_t * create_semaphores(int n){
    /*  create_semaphores
    *   Creates an array of semaphores based on the number inputted.
    *   
    *   Returns array of sem_ts
    */

    sem_t* semaphores = (sem_t *) malloc(sizeof(sem_t) * n);

    for(int i = 0; i < n; i++){
        if(sem_init(&semaphores[i], 0, 1) < 0){
            perror("Semaphore init");
            exit(1);
        }
        //printf("Semaphore %d available\n", i);
    }
    
    return semaphores;
}

void destroy_semaphores(sem_t * semaphores, int n){
    /*  destroy_semaphores
    *   Destroys an array of semaphores
    */

    for(int i = 0; i < n; i++){
        if(sem_destroy(&semaphores[i]) < 0){
            perror("Semaphore destroy");
            exit(1);
        }
        //printf("Destroyed Semaphore %d\n", i);
    }
}
/*
sem_t check_avaibility(sem_t * semaphores, int n){
    
}
*/

int request(char* msg, int pipe){
    if(write(pipe, msg, sizeof(msg)) < 0){
        return -1;
    }
    else{
        return 0;
    }
}

char wait_msg[5] = "wait";

int main(){

    int num_of_employees = 2;
    int num_of_couches = 2;

    int p1[2];
    int p2[2];
    if(pipe(p1) < 0){
        perror("pipe!\n");
    }
    if(pipe(p2) < 0){
        perror("pipe!\n");
    }

    if(fork() == 0){
        close(p1[0]);
        close(p2[1]);

        int send = 0;
        if(fork() == 0){
            while(send != 0);
            send += 1;
            if(request(wait_msg, p1[1]) < 0){
                exit(1);
            }
            send -= 1;
        }
        else{
            while(send != 0);
            send += 1;
            if(request(wait_msg, p1[1]) < 0){
                exit(1);
            }
            send -= 1;
        }
    }
    else{
        close(p1[1]);
        close(p2[0]);

        char buffer[50];
        if(read(p1[0], buffer, 50) < 0){
            perror("read");
            exit(1);
        }
        puts(buffer);

        bzero(buffer, 50);
        if(read(p1[0], buffer, 50) < 0){
            perror("read");
            exit(1);
        }
        puts(buffer);

        

        wait(NULL);

        sem_t* employees = create_semaphores(num_of_employees);
        sem_t* couches = create_semaphores(num_of_couches);

        printf("Hello from parent!\n");

        destroy_semaphores(employees, num_of_employees);
        destroy_semaphores(couches, num_of_couches);
    }
}