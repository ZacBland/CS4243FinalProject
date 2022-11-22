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
        printf("Semaphore %d available\n", i);
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
        printf("Destroyed Semaphore %d\n", i);
    }
}
/*
sem_t check_avaibility(sem_t * semaphores, int n){
    
}
*/
int main(){

    int num_of_employees = 2;
    int num_of_couches = 2;

    if(fork() == 0){
        if(fork() == 0){
            printf("Hello from child 2! >>\n");
            int x;
            scanf("%d", &x);
            printf("Child 2 output: %d\n", x);
        }
        else{
            printf("Hello from child 1!\n");
            int x;
            scanf("%d", &x);
            printf("Child 1 output: %d\n", x);
        }
    }
    else{
        wait(NULL);

        sem_t* employees = create_semaphores(num_of_employees);
        sem_t* couches = create_semaphores(num_of_couches);

        printf("Hello from parent!\n");

        destroy_semaphores(employees, num_of_employees);
        destroy_semaphores(couches, num_of_couches);
    }
}