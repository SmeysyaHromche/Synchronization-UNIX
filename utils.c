#include "proj2.h"

/* const chars array with text output pattern */
const char act_text[9][30] = {"started", "going home", "entering office for service", "called by office worker", "serving a service of type", "service finished", "taking break", "break finished", "closing"};

/* parsing of command line, checking correctness of arguments and save arguments */
void analys_arg_line(int argc, char *argv[], int *arg_value){
    int num;  // var for individual value of command line 
    if(argc-1 != SIZE_ARG_LINE){  // must argc = 5 (without name 'proj2')
        fprintf(stderr, "Error! The user must specify 5 specifications of the program. But you have only %d.\n", argc);
        exit(1);   
    }
    for(int i = 1; i <= SIZE_ARG_LINE; i++){  // read command line
        for(int j = 0; argv[i][j] != '\0'; j++){
            if(argv[i][j] == '-' || argv[i][j] == '+'){  // without '+' and '-'
                fprintf(stderr, "Error! The entered argument '%s' on position '%i' must have type positive int without symb '+' or '-' .\n", argv[i], i);
                exit(1);
            }
            else if(!isdigit(argv[i][j])){  //  only num
                fprintf(stderr, "Error! The entered argument '%s' on position '%i' must have type 'int'.\n", argv[i], i);
                exit(1);
            }
        }
        num = atoi(argv[i]);  // from string to int
        if((i == 1 || i == 2) && num == 0){  // NZ > 0 and NU > 0 
            fprintf(stderr, "Error! Must be: NZ > 0 and NU > 0.\n");
            exit(1);
        }

        if((i == 3 || i == 5) && num > 1000){  // 0 <= TZ <= 1000 and 0 <= F <= 1000
            fprintf(stderr, "Error! Must be 0 <= TZ <= 1000 and 0 <= F <= 1000.\n");
            exit(1);
        }
        if (i == 4 && num > 100){  // 0 <= TU <= 100
            fprintf(stderr, "Error! Must be 0 <= TU <= 100.\n");
            exit(1);
        }

        arg_value[i-1] = num;  // saved value of command line
    }
}

/* sleeping process */
void time_to_sleep(int intervale, bool main_process){
    int interval_sleep;  // var for sleeping time
    if(main_process){  // if sleeping for main proces
        double random = (double) rand() / RAND_MAX;  // random 
        interval_sleep = (int)((intervale/2) + random*(intervale/2)*1000);
    }
    else{
        interval_sleep = (rand()%(intervale+1))*1000;
    }
    usleep(interval_sleep);
}

/* creating file for output */
void file_creating(){
    file = fopen("proj2.out", "w");  // open stream in file, with the possibility of writting
    if(file == NULL){  // error opening file
        fprintf(stderr, "Error! Error in openning file 'proj2.out' for output.\n");
        exit(1);
    }
    
    setbuf(file, 0);  // cancel buffering for output file (writting maximum speed)
}

/* writting output */
void write_output(Shared_memory_t *post, int service, bool customer, bool clerk, int id, int act){
    sem_wait(&post->output_sem);  // wait mutex for writting in file
    fprintf(file, "%d: ", post->line);  // act line
    if(customer){  // action or customer(Z) or clerk(U)
        fprintf(file, "Z ");
    }
    if(clerk){
        fprintf(file, "U ");
    }
    if(!customer && !clerk){  // action from main proces
        fprintf(file, "%s\n", act_text[act]);  // write action
        post->line++;  // inc line
        sem_post(&post->output_sem);  // post mutex for writting in file
        return;
    }
    fprintf(file, "%d: %s  ", id, act_text[act]);  // write action
    if(act == 2 || act == 4){  // if must be in line num of service
        fprintf(file, "%d", service);
    }
    fprintf(file, "\n");
    post->line++;  // inc line 
    sem_post(&post->output_sem);  // post mutex for writting in file
}
