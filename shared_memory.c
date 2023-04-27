#include "proj2.h"


/* creating shared memory */
Shared_memory_t *sh_m_create(){
    Shared_memory_t *post;  // pointer on struct shared memory
    int id_shm;  // var for id shared memory
    if((id_shm = shmget(IPC_PRIVATE, sizeof(Shared_memory_t), IPC_CREAT | 0666)) < 1){  // inite segemnts of shared memory
        fprintf(stderr, "Error! Failed creating shared memory shmget().\n");
        exit(1);
    }
    if ((post = (Shared_memory_t *)shmat(id_shm, NULL, 0)) == (Shared_memory_t *) -1){  // attachment of shared memory to address space of process
        fprintf(stderr, "Error! Failed creating shared memory shmat().\n");
        exit(1);
    }
    post->id_sh_m = id_shm;  // save id of shared memory
    return post;  // return pointer on shared memory
}


/* cleaning shared memory */
void sh_m_clean(Shared_memory_t *post){
    if(shmctl(post->id_sh_m, IPC_RMID, NULL) == -1){  // delete shared memory
        fprintf(stderr, "Error! Failed marking segment shared memory to destroy");
        exit(1);
    }
    if(shmdt(post) == -1){  // detachment shared memory from address space of process
        fprintf(stderr, "Error! Failed detaches from the calling process's address space the shared memory");
        exit(1);
    }
}


/* set shared memory on init state */
void sh_m_new_value(int *arr, Shared_memory_t* post){
    
    post->num_customer = arr[0];  // save count of customers
    post->num_clerk = arr[1];  // save count of clerks
    post->max_time_wait = arr[2];  // save time for customer sleeping
    post->max_time_break = arr[3];  // save time for clerks break
    post->max_time_close_post = arr[4];  // save working time of post

    post->post_live = true;  // open post
    post->service_1 = 0;  // empty queue for 1 servis
    post->service_2 = 0;  // empty queue for 2 servis
    post->service_3 = 0;  // empty queue for 3 servis
    post->line = 1;  // first line

    if(semaphors_init(post)){  // init semaphors in shared memory
        sh_m_clean(post);
        exit (1);
    }
}
