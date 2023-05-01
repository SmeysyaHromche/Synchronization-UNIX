#ifndef PROJ2_H
#define PROJ2_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/wait.h>


//----------------------------------------------   SHARED MEMORY   ------------------------------------------------------

/*
Struct Shared_memory_t implememnts shared memory for all process at time running program. Describe starts counditions value (count clerk, customer...),
and actual state of post(open/close ...), id of shared memery and semaphores. 
*/
typedef struct
{
    /*starts conditions*/
    int num_customer;  // count of all customers
    int num_clerk;  // count of all clerks
    int max_time_wait;  // max time for waiting clerks after starts
    int max_time_break;  // max time for clerks break
    int max_time_close_post;  // max time for opening post
    
    /*actual state of post*/
    bool post_live;  // open post(True) or close post(False)
    int service_1;  // count customers in queue for 1 service
    int service_2;  // count customers in queue for 2 service
    int service_3;  // count customers in queue for 3 service
    int line;   // count acting was in post
    
    int id_sh_m;  // id shared memery

    sem_t service1_sem;  // mutex for serving 1 service
    sem_t service2_sem;  // mutex for serving 2 service
    sem_t service3_sem;  // mutex for serving 3 service
    sem_t output_sem;  // mutex for writting in output
    sem_t service1_queue_sem;  // semaphor for serving 1 service
    sem_t service2_queue_sem;  // semaphor for serving 2 service
    sem_t service3_queue_sem;  // semaphor for serving 3 service
    sem_t clos_sem;  // semaphor for sinchronizate with going clerk home
    sem_t check_clos_sem;  // mutex for sinchronizate with closing post for going customers and breaaking work clerks
}Shared_memory_t;


/*
Shared_memory_t *sh_m_create()  -- function for creating segments of shared memory and return pointer
on this segment. Shared memory implemented by struct Shared_memory_t.
*/
Shared_memory_t *sh_m_create();


/*
void sh_m_new_value(int *arr, Shared_memory_t* post)  -- function set shared memory on init state.
 Saved count of customer and clerks, set count in line on zero value, open post. Descripe "post"
 in morning. Return value miss.
 
 Param:
    + int *arr -- pointer on array with value from command line
    + Shared_memory_t* post  -- pointer on struct shared memory what will be set on start state
*/
void sh_m_new_value(int *arr,  Shared_memory_t* post);


/*
void sh_m_clean(Shared_memory_t *post)  -- function cleaning memory of shared memory.
Param:
    + Shared_memory_t* post  -- pointer on struct shared memory what will be clean
*/
void sh_m_clean(Shared_memory_t *post);




//---------------------------------------------   UTILS  -------------------------------------------

#define SIZE_ARG_LINE 5  // const descrip count

/*
void analys_arg_line(int argc, char *argv[], int *arg_value)  -- function for parsing of command line, checking 
correctness of arguments and save arguments.
Param:
    + int argc, char *argv[]
    + int *arg_value -- pointer on array where saved value of command lines arguments
*/
void analys_arg_line(int argc, char *argv[], int *arg_value);

/*
void time_to_sleep(int intervale, bool main_process)  -- function for sleeping process on rundom time based on 
the specified interval.
Param:
    + int intervale  -- maximum value for interval
    + bool main_process  -- if True is a Main proces, else is clerks/customers process
*/
void time_to_sleep(int intervale, bool main_process);

/* void file_creating()  -- function for opening outputs file and cancel buffering for increase the recording speed*/
void file_creating();

/*void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act)  -- function 
for writting output text in file.
Param:
    + Shared_memory_t *post  -- shared memeory for increment count of acting in post and using semaphors
    + int service  -- number of service
    + bool customer -- if True acting make customer  
    + bool clerk  -- if True acting make clerk
    + int id -- id of customer or clerk
    + int act -- index in array what save output pattern
*/
void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act);




//---------------------------------------------   PROCESS AND SEMAPHORS  -------------------------------------------

/*
void customer_proces(Shared_memory_t *post, int id) -- function implemante all activites in customer process.(start,
sleeping, choose service and stay in services queue, wait serving from clerk, and going out from post)
Param:
    + Shared_memory_t *post -- pointer on shared memory, for using data.
    + int id -- id this process
*/
void customer_proces(Shared_memory_t *post, int id);

/*
void clerk_proces(Shared_memory_t *post, int id) -- function implemante all activites in clerk process.(start,
sleeping, choose services queue for serving, customers invitation to service, serving, breaking and going
out from post)
Param:
    + Shared_memory_t *post -- pointer on shared memory, for using data.
    + int id -- id this process
*/
void clerk_proces(Shared_memory_t *post, int id);

/*
int semaphors_init(Shared_memory_t *post)  - function set all semaphors from shared memory in starting state.
If an error occurs return 1, if all ok return 0.
Param:
    + Shared_memory_t *post -- pointer on shared memory whete locate semaphors
*/
int semaphors_init(Shared_memory_t *post);

/*
int semaphors_destroy(Shared_memory_t *post)  - function cleaning semaphors from shared memory.
If an error occurs return 1, if all ok return 0.
Param:
    + Shared_memory_t *post -- pointer on shared memory whete locate semaphors
*/
int semaphors_destroy(Shared_memory_t *post);
#endif
