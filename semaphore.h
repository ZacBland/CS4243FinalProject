
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

int * create_semaphores(int n);
int check_avaibility(int * semaphores, int n);
void take(int * semaphores, int index, int socket);
void post(int * semaphores, int index);

#endif