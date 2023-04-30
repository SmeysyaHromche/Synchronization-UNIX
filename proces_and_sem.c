#include "proj2.h"

/* function for implement all activites in clerk proces */
void clerk_proces(Shared_memory_t *post, int id){
    srand(getpid()*time(NULL));  // start generic random value based on time and random pid
    int choose_service;  // var for choose service for serving
    write_output(post, 0, false, true, id, 0);  // start
    while(post->post_live || post->service_1 > 0 || post->service_2 > 0 || post->service_3 > 0){  // cycle for clearks working(work when post open and exist people in queue)
        if(post->service_1 > 0){  // choose service 1 if in queue be customer
            choose_service = 1;  // set var
            sem_wait(&post->service1_sem);  // wait mutex for count in queue
            post->service_1--;  // dec count in queue
            sem_post(&post->service1_sem);  //  post mutex for count in queue
            sem_post(&post->service1_queue_sem);  // invites the customer
            write_output(post, choose_service, false, true, id, 4);  // serving
        }
        else if(post->service_2 > 0){  // choose service 2 if in queue be customer
            choose_service = 2;  // set var
            sem_wait(&post->service2_sem);  // wait mutex for count in queue
            post->service_2--;  // dec count in queue
            sem_post(&post->service2_sem);  //  post mutex for count in queue
            sem_post(&post->service2_queue_sem);  // invites the customer
            write_output(post, choose_service, false, true, id, 4);  // serving
        }
        else if(post->service_3 > 0){  // choose service 3 if in queue be customer
            choose_service = 3;  // set var
            sem_wait(&post->service3_sem);  // wait mutex for count in queue
            post->service_3--;  // dec count in queue
            sem_post(&post->service3_sem);  //  post mutex for count in queue
            sem_post(&post->service3_queue_sem);  // invites the customer
            write_output(post, choose_service, false, true, id, 4);  // serving
        }
        else{  // if in post dont be customer
            sem_wait(&post->check_clos_sem);  // wait mutex for sinchronizate with closing post for going customers and breaaking work clerks
            if(!post->post_live){
                sem_post(&post->check_clos_sem);  // post mutex for sinchronizate with closing post for going customers and breaaking work clerks
                break;
            }
            write_output(post, 0, false, true, id, 6);  // going in break
            sem_post(&post->check_clos_sem);  // post mutex for sinchronizate with closing post for going customers and breaaking work clerks
            time_to_sleep(post->max_time_break, false);  // sleep
            write_output(post, 0, false, true, id, 7);  // back form break
            continue;
        }
        time_to_sleep(10, false);  // sleep
        write_output(post, choose_service, false, true, id, 5);  // end serving   
    }
    sem_wait(&post->clos_sem);  // wait sem for sinchronizate with going clerk home
    write_output(post, 0, false, true, id, 1);  // go to home
    sem_post(&post->clos_sem);  // post sem for sinchronizate with going clerk home
    exit(0);
}


/* function for implement all activites in customers proces */
void customer_proces(Shared_memory_t *post, int id){
    srand(getpid()*time(NULL));  // start generic random value based on time and random pid
    write_output(post, 0, true, false, id, 0);  // start
    time_to_sleep(post->max_time_wait, false);  // sleep
    sem_wait(&post->check_clos_sem);  // wait mutex for sinchronizate with closing post for going customers and breaaking work clerks
    if(!post->post_live){  // check if post close
        write_output(post, 0, true, false, id, 1);  // go to home
        sem_post(&post->check_clos_sem);  // post mutex for sinchronizate with closing post for going customers and breaaking work clerks
        exit(0);
    }
    int choose_service = rand() % 3 + 1;  // client choose servec
    switch (choose_service){
        case 1:  // if service 1
            sem_wait(&post->service1_sem);  // wait mutex for count in queue
            post->service_1++;  // inc count in queue
            sem_post(&post->service1_sem);  // post mutex for count in queue
            break;
        case 2:
            sem_wait(&post->service2_sem);  // wait mutex for count in queue
            post->service_2++;  // inc count in queue
            sem_post(&post->service2_sem);  // post mutex for count in queue
            break;
        case 3:
            sem_wait(&post->service3_sem);  // wait mutex for count in queue
            post->service_3++;  // inc count in queue
            sem_post(&post->service3_sem);  // post mutex for count in queue
            break;
        default:
            break;
    } 
    write_output(post, choose_service, true, false, id, 2);  // waiting clerk
    sem_post(&post->check_clos_sem);  // post mutex for sinchronizate with closing post for going customers and breaaking work clerks
    switch (choose_service){
        case 1:
            sem_wait(&post->service1_queue_sem);  // invited by a clerk
            break;
        case 2:
            sem_wait(&post->service2_queue_sem);  // invited by a clerk
            break;
        case 3:
            sem_wait(&post->service3_queue_sem);  // invited by a clerk
            break;
        default:
            break;
    }
    write_output(post, 0, true, false, id, 3);  // go to serving with clerk
    time_to_sleep(10, false); // sleep
    write_output(post, 0, true, false, id, 1);  // go home
    exit(0);
}

/* set all semaphors from shared memory in starting state */
int semaphors_init(Shared_memory_t *post){
    if(sem_init(&post->service1_sem, 1, 1) == -1){  // init mutex for 1 servis and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service2_sem, 1, 1) == -1 ){  // init mutex for 2 servis and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service3_sem, 1, 1) == -1 ){  // init mutex for 3 servis and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->output_sem, 1, 1) == -1 ){  // init mutex for output writting and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service1_queue_sem, 1, 0) == -1){  // init semaphor for 1 servis queue and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service2_queue_sem, 1, 0) == -1 ){  // init semaphor for 2 servis queue and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->service3_queue_sem, 1, 0) == -1 ){  // init semaphor for 3 servis queue and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->clos_sem, 1, 0) == -1 ){  // init semaphor for sinchronizate with going clerk home and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    if(sem_init(&post->check_clos_sem, 1, 1) == -1 ){  // init mutex for sinchronizate with closing post for going customers and breaaking work clerks and check error
        fprintf(stderr, "Error! Failed inittialization of semaphor.\n");
        return 1;
    }
    return 0;
}

/* cleaning semaphors from shared memory */
int semaphors_destroy(Shared_memory_t *post){
    if(sem_destroy(&post->service1_sem) == -1 ){  // destroy mutex for 1 servis and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service2_sem) == -1 ){  // destroy mutex for 2 servis and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service3_sem) == -1){  // destroy mutex for 3 servis and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    } 
    if(sem_destroy(&post->output_sem) == -1 ){  // destroy mutex for output writting and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service1_queue_sem)  == -1 ){  // destroy semaphor for 1 servis queue and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service2_queue_sem)  == -1 ){  // destroy semaphor for 2 servis queue and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->service3_queue_sem)  == -1 ){  // destroy semaphor for 3 servis queue and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->clos_sem)  == -1 ){  // destroy semaphor for sinchronizate with going clerk home and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    if(sem_destroy(&post->check_clos_sem)  == -1 ){  // destroy mutex for sinchronizate with closing post for going customers and breaaking work clerks and check error
        fprintf(stderr, "Error! Failed destruction of semaphor.\n");
        return 1;
    }
    return 0;
}
