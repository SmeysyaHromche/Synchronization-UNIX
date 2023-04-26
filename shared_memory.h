#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <stdbool.h>

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
}Shared_memory_t;


void sh_m_new_value(int *arr,  Shared_memory_t* post);
void sh_m_clean(Shared_memory_t *post);
Shared_memory_t *sh_m_create();


#endif