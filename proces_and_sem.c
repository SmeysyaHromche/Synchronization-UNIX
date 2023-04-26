#include "proces_and_sem.h"
void file_creating(){  // definition func for open writting stream in output file
    file = fopen("proj2.out", "a");  // open stream in file
    if(file == NULL){
        fprintf(stderr, "Error! Error in openning file 'proj2.out' for output.\n");
        exit(1);
    }
}

void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act, Semaphores_t *sem){
    sem_wait(&sem->output_sem);
    file_creating();  // create file for output
    fprintf(file, "%d: ", post->line);
    if(customer){
        fprintf(file, "U ");
    }
    if(clerk){
        fprintf(file, "Z ");
    }
    if(!customer && !clerk){
        fprintf(file, "%s\n", act_text[act]);
        sem_post(&sem->output_sem);
        post->line++;
        return;
    }
    fprintf(file, "%d: %s  ", id, act_text[act]);
    if(act == 2 || act == 4){
        fprintf(file, "%d", service);
    }
    fprintf(file, "\n");
    fclose(file);  // closing output
    post->line++;
    sem_post(&sem->output_sem);
}


void clerk_proces(Shared_memory_t *post, int id, Semaphores_t *sem ){
    write_output(post, 0, false, true, id, 0, sem);
    while(post->post_live || post->serves_1 > 0 || post->serves_2 > 0 || post->serves_3 > 0){
        srand(getpid()*time(NULL));  // start generic random value based on time and random pid
        int choose_service = rand() % 3 + 1;  // client choose servec(or 1 or 2 or 3)
        if(choose_service == 1){
            if(post->serves_1 > 0){
                sem_wait(&sem->serves1_sem);
                post->serves_1--;
                sem_post(&sem->serves1_sem);
                write_output(post, choose_service, false, true, id, 4, sem);
            }
            else{
                write_output(post, choose_service, false, true, id, 6, sem);
                time_to_sleep(post->max_time_break, false);
                write_output(post, choose_service, false, true, id, 7, sem);
            }
        }
        else if(choose_service == 2){
            if(post->serves_2 > 0){
                sem_wait(&sem->serves2_sem);
                post->serves_2--;
                sem_post(&sem->serves2_sem);
                write_output(post, choose_service, false, true, id, 4, sem);
            }
            else{
                write_output(post, choose_service, false, true, id, 6, sem);
                time_to_sleep(post->max_time_break, false);
                write_output(post, choose_service, false, true, id, 7, sem);
                continue;
            }
        }
        else if(choose_service == 3){
            if(post->serves_3 > 0){
                sem_wait(&sem->serves3_sem);
                post->serves_3--;
                sem_post(&sem->serves3_sem);
                write_output(post, choose_service, false, true, id, 4, sem);
            }
            else{
                write_output(post, choose_service, false, true, id, 6, sem);
                time_to_sleep(post->max_time_break, false);
                write_output(post, choose_service, false, true, id, 7, sem);
            }
        }
        time_to_sleep(10, false);
        write_output(post, choose_service, false, true, id, 5, sem);   
    }
    write_output(post, 0, false, true, id, 1, sem);
    exit(0);
}



void customer_proces(Shared_memory_t *post, int id, Semaphores_t *sem){
    write_output(post, 0, true, false, id, 0, sem);
    time_to_sleep(post->max_time_wait, false);
    if(!post->post_live){
        write_output(post, 0, true, false, id, 1, sem);
        post->customer_in_post--;
        exit(0);
    }
    srand(getpid()*time(NULL));  // start generic random value based on time and random pid
    int choose_service = rand() % 3 + 1;  // client choose servec(or 1 or 2 or 3)
    switch (choose_service){
        case 1:
            sem_wait(&sem->serves1_sem);
            post->serves_1++;
            sem_post(&sem->serves1_sem);
            break;
        case 2:
            sem_wait(&sem->serves2_sem);
            post->serves_2++;
            sem_post(&sem->serves2_sem);
            break;
        case 3:
            sem_wait(&sem->serves3_sem);
            post->serves_3++;
            sem_post(&sem->serves3_sem);
            break;
        default:
            break;
    }

    write_output(post, choose_service, true, false, id, 2, sem);
    sem_post(&sem->customer);
    sem_wait(&sem->clerk);
    write_output(post, 0, true, false, id, 3, sem);
    sem_post(&sem->customer_done);
    sem_wait(&sem->clerk_done);
    time_to_sleep(10, false);
    write_output(post, 0, true, false, id, 1, sem);
    exit(0);
}

int semaphors_init(Semaphores_t *sem){
    if(sem_init(&sem->serves1_sem, 1, 1) == -1){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->serves2_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->serves3_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->output_sem, 1, 1) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->customer, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->clerk, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->customer_done, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&sem->clerk_done, 1, 0) == -1 ){
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    return 0;
}
int semaphors_destroy(Semaphores_t *sem){
    if(sem_destroy(&sem->serves1_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&sem->serves2_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&sem->serves3_sem) == -1){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    } 
    if(sem_destroy(&sem->output_sem) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&sem->customer)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&sem->clerk) == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&sem->customer_done)  == -1 ){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&sem->clerk_done) == -1){
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    return 0;
}