#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arg_line.h"

int analys_arg_line(int argc, char *argv[], int *arg_value){
    int num;
    for(int i = 1; i < argc; i++){
        for(int j = 0; argv[i][j] != '\0'; j++){
            if(argv[i][j] == '-' || argv[i][j] == '+'){
                fprintf(stderr, "Error! The entered argument '%s' on position '%i' must have type positive int without symb '+' or '-' .\n", argv[i], i);
                return 1;
            }
            else if(!isdigit(argv[i][j])){
                fprintf(stderr, "Error! The entered argument '%s' on position '%i' must have type 'int'.\n", argv[i], i);
                return 1;
            }
        }
        num = atoi(argv[i]);
        if((i == 1 || i == 2) && num <= 0){
            fprintf(stderr, "Error! Must be: NZ > 0 and NU > 0.\n");
            return 1;
        }

        if((i == 3 || i == 5) && (num < 0 || num > 1000)){
            fprintf(stderr, "Error! Must be 0 <= TZ <= 1000 and 0 <= F <= 1000.\n");
            return 1;
        }
        if (i == 4 && (num < 0 || num > 100)){
            fprintf(stderr, "Error! Must be 0 <= TU <= 100.\n");
            return 1;
        }

        arg_value[i] = num;
    }
    return 0;
}