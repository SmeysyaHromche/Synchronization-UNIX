#ifndef PROCES_AND_SEM_H
#define PROCES_AND_SEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>


typedef struct {
    sem_t serves1_sem;
    sem_t serves2_sem;
    sem_t serves3_sem;
    sem_t output_sem;
    sem_t customer;
    sem_t clerk;
    sem_t customer_done;
    sem_t clerk_done;
} Semaphores_t;

FILE *file;  // global var for output file
const char act_text[9][30] = {"started", "going home", "entering office for service", "called by office worker", "serving a service of type", "service finished", "taking break", "break finished", "closing"};

void customer_proces(Shared_memory_t *post, int id, Semaphores_t *sem);
void clerk_proces(Shared_memory_t *post, int id, Semaphores_t *sem);
int semaphors_init(Semaphores_t *sem);
int semaphors_destroy(Semaphores_t *sem);
void file_creating();  // declar func for open writting stream in output file
void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act, Semaphores_t *sem);

#endif