#include "proj2.h"
// author: xkukht01

/* main function */
int main(int argc, char *argv[]){
    
    /* parsing argument from command line */
    int arg_value[SIZE_ARG_LINE];  // array with int value of arg line
    analys_arg_line(argc, argv, arg_value);  // parsing and analysing of command line
    
    /* init shared memory */
    Shared_memory_t *post = NULL;  // abstraction "Post" (shared memory)
    post = sh_m_create();  // allocate shared memory
    sh_m_new_value(arg_value, post);  // specification shared memory
    
    /* creating file for output */
    file_creating();
    
    /* creating pid_t array array for identification customers and clerks process */
    pid_t id_customer[post->num_customer];
    pid_t id_clerk[post->num_clerk];

    /* starts clerks process */
    for(int i = 0; i < post->num_clerk; i++){
        
        id_clerk[i] = fork();  // making new process

        if(id_clerk[i] == 0){  // child proces
            clerk_proces(post,(i+1));
        }
        else if(id_clerk[i] == -1){  // error state after forking
            fprintf(stderr, "Error! Fail of clerks fork.\n");
            fclose(file);  // closing output
            semaphors_destroy(post);  // clean semaphors
            sh_m_clean(post);  // clean shared memory
            exit(1);
        }
    }

    /* starts customers process */
    for(int i = 0; i < post->num_customer; i++){  // start clients proces
        
        id_customer[i] = fork();  // making new process
        
        if(id_customer[i] == 0){  // child proces
            customer_proces(post,(i+1));
        }
        else if(id_customer[i] == -1){  // error state after forking
            fprintf(stderr, "Error! Fail of clerks fork\n");
            fclose(file);  // closing output
            semaphors_destroy(post);  // clean semaphors
            sh_m_clean(post);  // clean shared memory
            exit(1);
        }
    }
    
    /*  main proces */
    time_to_sleep(post->max_time_close_post, true);  // sleeping befor closing posts
    post->post_live = false;  // close post
    write_output(post, 0, false, false, 0, 8);
    
    /* wait when all childs process will finish */
    while(wait(NULL)>0);

    /* ending program, cleaning resurs */
    if(semaphors_destroy(post)){  // error state if failed sem_destroy()
        fclose(file);  // closing output
        sh_m_clean(post);  // clean shared memory
        exit(1);
    }
    fclose(file);  // closing output
    sh_m_clean(post);  // clean shared memory
    exit(0);
}
