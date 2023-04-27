#include "proj2.h"

void clerk_proces(Shared_memory_t *post, int id){
    int choose_service;
    write_output(post, 0, false, true, id, 0);
    while(post->post_live || post->service_1 > 0 || post->service_2 > 0 || post->service_3 > 0){
        if(post->service_1 > 0){
            choose_service = 1;
            sem_wait(&post->service1_sem);
            post->service_1--;
            sem_post(&post->service1_sem);
            sem_post(&post->service1_queue_sem);
            write_output(post, choose_service, false, true, id, 4);
        }
        else if(post->service_2 > 0){
            choose_service = 2;
            sem_wait(&post->service2_sem);
            post->service_2--;
            sem_post(&post->service2_sem);
            sem_post(&post->service2_queue_sem);
            write_output(post, choose_service, false, true, id, 4);
        }
        else if(post->service_3 > 0){
            choose_service = 3;
            sem_wait(&post->service3_sem);
            post->service_3--;
            sem_post(&post->service3_sem);
            sem_post(&post->service3_queue_sem);
            write_output(post, choose_service, false, true, id, 4);
        }
        else{
            write_output(post, 0, false, true, id, 6);
            time_to_sleep(post->max_time_break, false);
            write_output(post, 0, false, true, id, 7);
            continue;
        }
        time_to_sleep(10, false);
        write_output(post, choose_service, false, true, id, 5);   
    }
    write_output(post, 0, false, true, id, 1);
    exit(0);
}



void customer_proces(Shared_memory_t *post, int id){
    write_output(post, 0, true, false, id, 0);
    time_to_sleep(post->max_time_wait, false);
    if(!post->post_live){
        write_output(post, 0, true, false, id, 1);
        exit(0);
    }
    srand(getpid()*time(NULL));  // start generic random value based on time and random pid
    int choose_service = rand() % 3 + 1;  // client choose servec(or 1 or 2 or 3)
    switch (choose_service){
        case 1:
            sem_wait(&post->service1_sem);
            post->service_1++;
            sem_post(&post->service1_sem);
            break;
        case 2:
            sem_wait(&post->service2_sem);
            post->service_2++;
            sem_post(&post->service2_sem);
            break;
        case 3:
            sem_wait(&post->service3_sem);
            post->service_3++;
            sem_post(&post->service3_sem);
            break;
        default:
            break;
    }
    write_output(post, choose_service, true, false, id, 2);
    switch (choose_service){
        case 1:
            sem_wait(&post->service1_queue_sem);
            break;
        case 2:
            sem_wait(&post->service2_queue_sem);
            break;
        case 3:
            sem_wait(&post->service3_queue_sem);
            break;
        default:
            break;
    }
    write_output(post, 0, true, false, id, 3);
    time_to_sleep(10, false);
    write_output(post, 0, true, false, id, 1);
    exit(0);
}

int semaphors_init(Shared_memory_t *post){
    if(sem_init(&post->service1_sem, 1, 1) == -1){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service2_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service3_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->output_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service1_queue_sem, 1, 0) == -1){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service2_queue_sem, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service3_queue_sem, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    return 0;
}
int semaphors_destroy(Shared_memory_t *post){
    if(sem_destroy(&post->service1_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service2_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service3_sem) == -1){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    } 
    if(sem_destroy(&post->output_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service1_queue_sem)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service2_queue_sem)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service3_queue_sem)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    return 0;
}
