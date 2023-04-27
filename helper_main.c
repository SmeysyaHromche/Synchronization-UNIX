#include "helper_main.h"
void analys_arg_line(int argc, char *argv[], int *arg_value){  // definition of func func for parsing and analysing command line
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

void time_to_sleep(int intervale, bool main_process){
    int interval_sleep;
    if(main_process){
        double random = (double) rand() / RAND_MAX;
        interval_sleep = (int)((intervale/2) + random*(intervale/2)*1000);
    }
    else{
        interval_sleep = (rand()%(intervale+1))*1000;
    }
    usleep(interval_sleep);
}
