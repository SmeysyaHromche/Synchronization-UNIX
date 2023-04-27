#include "kernel.h"

int main(int argc, char *argv[]){
    int arg_value[SIZE_ARG_LINE];  // array with int value of arg line
    analys_arg_line(argc, argv, arg_value);  // parsing and analysing of command line
    Shared_memory_t *post = NULL;
    post = sh_m_create();
    sh_m_new_value(arg_value, post);
    if(semaphors_init(post)){
        sh_m_clean(post);
        exit (1);
    }
    file_creating();  // create file for output
    setbuf(file, 0);  // cancel buffering for output file (writting maximum speed)
    pid_t id_customer[post->num_customer];
    pid_t id_clerk[post->num_clerk];

    for(int i = 0; i < post->num_clerk; i++){
        id_clerk[i] = fork();
        if(id_clerk[i] == 0){
            clerk_proces(post,(i+1));
        }
        else if(id_clerk[i] == 0){
            fprintf(stderr, "Error! Fail of clerks fork\n");
            fclose(file);  // closing output
            semaphors_destroy(post);
            sh_m_clean(post);
            exit(1);
        }
    }
    for(int i = 0; i < post->num_customer; i++){
        id_customer[i] = fork();
        if(id_customer[i] == 0){
            customer_proces(post,(i+1));
        }
        else if(id_customer[i] == 0){
            fprintf(stderr, "Error! Fail of clerks fork\n");
            fclose(file);  // closing output
            semaphors_destroy(post);
            sh_m_clean(post);
            exit(1);
        }
    }
    time_to_sleep(post->max_time_close_post, true);
    post->post_live = false;
    write_output(post, 0, false, false, 0, 8);
    //while(wait(NULL)>0);
    if(semaphors_destroy(post)){
        fclose(file);  // closing output
        sh_m_clean(post);
        exit(1);
    }
    fclose(file);  // closing output
    sh_m_clean(post);
    exit(0);
    return 0;
}
