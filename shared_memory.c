#include "kernel.h"

void sh_m_new_value(int *arr, Shared_memory_t* post){
    
    post->num_customer = arr[0];
    post->num_clerk = arr[1];
    post->max_time_wait = arr[2];
    post->max_time_close_post = arr[3];
    post->max_time_break = arr[4];

    post->post_live = true;
    post->serves_1 = 0;
    post->serves_2 = 0;
    post->serves_3 = 0;
    post->customer_in_post = 0; 
    post->line = 1;
}

void sh_m_clean(Shared_memory_t *post){
    if(shmctl(post->id_sh_m, IPC_RMID, NULL) == -1){
        fprintf(stderr, "Error! Failed marking segment shared memory to destroy");
        exit(1);
    }
    if(shmdt(post) == -1){
        fprintf(stderr, "Error! Failed detaches from the calling process's address space the shared memory");
        exit(1);
    }
}

Shared_memory_t *sh_m_create(){
    Shared_memory_t *post;
    int id_shm;
    if((id_shm = shmget(IPC_PRIVATE, sizeof(Shared_memory_t), IPC_CREAT | 0666)) < 1){
        printf("bad");
        fprintf(stderr, "Error! Failed creating shared memory shmget().\n");
        exit(1);
    }
    if ((post = (Shared_memory_t *)shmat(id_shm, NULL, 0)) == (Shared_memory_t *) -1){
        fprintf(stderr, "Error! Failed creating shared memory shmat().\n");
        exit(1);
    }
    post->id_sh_m = id_shm;
    return post;
}
