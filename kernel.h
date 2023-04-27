#ifndef KERNEL_H
#define KERNEL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SIZE_ARG_LINE 5  // const descrip count
void analys_arg_line(int argc, char *argv[], int *arg_value);  // declar func for parsing and analysing command line
void time_to_sleep(int intervale, bool main_process);

typedef struct {
    sem_t serves1_sem;
    sem_t serves2_sem;
    sem_t serves3_sem;
    sem_t output_sem;
    sem_t serves1_queue_sem;
    sem_t serves2_queue_sem;
    sem_t serves3_queue_sem;
} Semaphores_t;

typedef struct
{
    int num_customer;
    int num_clerk;
    int max_time_wait;
    int max_time_break;
    int max_time_close_post;
    
    bool post_live;
    int serves_1;
    int serves_2;
    int serves_3;
    int customer_in_post;
    int line; 
    
    int id_sh_m;

    sem_t serves1_sem;
    sem_t serves2_sem;
    sem_t serves3_sem;
    sem_t output_sem;
    sem_t serves1_queue_sem;
    sem_t serves2_queue_sem;
    sem_t serves3_queue_sem;
}Shared_memory_t;
FILE *file;  // global var for output file
void customer_proces(Shared_memory_t *post, int id);
void clerk_proces(Shared_memory_t *post, int id);
int semaphors_init(Shared_memory_t *post);
int semaphors_destroy(Shared_memory_t *post);
void file_creating();  // declar func for open writting stream in output file
void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act);


void sh_m_new_value(int *arr,  Shared_memory_t* post);
void sh_m_clean(Shared_memory_t *post);
Shared_memory_t *sh_m_create();
#endif
