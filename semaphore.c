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
