#include "proj2.h"

int main(int argc, char *argv[]){
    
    int arg_value[SIZE_ARG_LINE];  // array with int value of arg line
    analys_arg_line(argc, argv, arg_value);  // parsing and analysing of command line
    
    Shared_memory_t *post = NULL;  // abstraction "Post" (shared memory)
    post = sh_m_create();  // allocate shared memory
    sh_m_new_value(arg_value, post);  // specification shared memory
    if(semaphors_init(post)){  // init semaphors in shared memory
        sh_m_clean(post);
        exit (1);
    }
    
    file_creating();  // create file for output
    
    pid_t id_customer[post->num_customer];  // array for identification customers process with pid
    pid_t id_clerk[post->num_clerk];  // array for identification clerks proces with pid

    for(int i = 0; i < post->num_clerk; i++){  // start customers process
        id_clerk[i] = fork();  // making new process
        if(id_clerk[i] == 0){  // child proces
            clerk_proces(post,(i+1));
        }
        else if(id_clerk[i] == 0){  // error state after forking
            fprintf(stderr, "Error! Fail of clerks fork.\n");
            fclose(file);  // closing output
            semaphors_destroy(post);  // clean semaphors
            sh_m_clean(post);  // clean shared memory
            exit(1);
        }
    }
    
    for(int i = 0; i < post->num_customer; i++){  // start clients proces
        id_customer[i] = fork();  // making new process
        if(id_customer[i] == 0){  // child proces
            customer_proces(post,(i+1));
        }
        else if(id_customer[i] == 0){  // error state after forking
            fprintf(stderr, "Error! Fail of clerks fork\n");
            fclose(file);  // closing output
            semaphors_destroy(post);  // clean semaphors
            sh_m_clean(post);  // clean shared memory
            exit(1);
        }
    }
    
    time_to_sleep(post->max_time_close_post, true);  // sleeping befor closing posts
    post->post_live = false;  // close post
    write_output(post, 0, false, false, 0, 8);
    //while(wait(NULL)>0);
    
    if(semaphors_destroy(post)){  // error state if failed sem_destroy()
        fclose(file);  // closing output
        sh_m_clean(post);  // clean shared memory
        exit(1);
    }
    fclose(file);  // closing output
    sh_m_clean(post);  // clean shared memory
    exit(0);
}
