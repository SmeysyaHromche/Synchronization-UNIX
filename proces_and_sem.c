#include "helper_main.h"

const char act_text[9][30] = {"started", "going home", "entering office for service", "called by office worker", "serving a service of type", "service finished", "taking break", "break finished", "closing"};


void file_creating(){  // definition func for open writting stream in output file
    file = fopen("proj2.out", "w");  // open stream in file
    if(file == NULL){
        fprintf(stderr, "Error! Error in openning file 'proj2.out' for output.\n");
        exit(1);
    }
}

void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act){
    sem_wait(&post->output_sem);
    fprintf(file, "%d: ", post->line);
    if(customer){
        fprintf(file, "Z ");
    }
    if(clerk){
        fprintf(file, "U ");
    }
    if(!customer && !clerk){
        fprintf(file, "%s\n", act_text[act]);
        post->line++;
        sem_post(&post->output_sem);
        return;
    }
    fprintf(file, "%d: %s  ", id, act_text[act]);
    if(act == 2 || act == 4){
        fprintf(file, "%d", service);
    }
    fprintf(file, "\n");
    post->line++;
    sem_post(&post->output_sem);
}


void clerk_proces(Shared_memory_t *post, int id){
    int choose_service;
    write_output(post, 0, false, true, id, 0);
    while(post->post_live || post->serves_1 > 0 || post->serves_2 > 0 || post->serves_3 > 0){
        if(post->serves_1 > 0){
            choose_service = 1;
            sem_wait(&post->serves1_sem);
            post->serves_1--;
            sem_post(&post->serves1_sem);
            sem_post(&post->serves1_queue_sem);
            write_output(post, choose_service, false, true, id, 4);
        }
        else if(post->serves_2 > 0){
            choose_service = 2;
            sem_wait(&post->serves2_sem);
            post->serves_2--;
            sem_post(&post->serves2_sem);
            sem_post(&post->serves2_queue_sem);
            write_output(post, choose_service, false, true, id, 4);
        }
        else if(post->serves_3 > 0){
            choose_service = 3;
            sem_wait(&post->serves3_sem);
            post->serves_3--;
            sem_post(&post->serves3_sem);
            sem_post(&post->serves3_queue_sem);
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
            sem_wait(&post->serves1_sem);
            post->serves_1++;
            sem_post(&post->serves1_sem);
            break;
        case 2:
            sem_wait(&post->serves2_sem);
            post->serves_2++;
            sem_post(&post->serves2_sem);
            break;
        case 3:
            sem_wait(&post->serves3_sem);
            post->serves_3++;
            sem_post(&post->serves3_sem);
            break;
        default:
            break;
    }
    write_output(post, choose_service, true, false, id, 2);
    switch (choose_service){
        case 1:
            sem_wait(&post->serves1_queue_sem);
            break;
        case 2:
            sem_wait(&post->serves2_queue_sem);
            break;
        case 3:
            sem_wait(&post->serves3_queue_sem);
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
    if(sem_init(&post->serves1_sem, 1, 1) == -1){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->serves2_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->serves3_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->output_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->serves1_queue_sem, 1, 0) == -1){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->serves2_queue_sem, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->serves3_queue_sem, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    return 0;
}
int semaphors_destroy(Shared_memory_t *post){
    if(sem_destroy(&post->serves1_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->serves2_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->serves3_sem) == -1){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    } 
    if(sem_destroy(&post->output_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->serves1_queue_sem)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->serves2_queue_sem)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->serves3_queue_sem)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    return 0;
}
