#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_line.h"

int main(int argc, char *argv[]){
    int arg_value[5];
    if(analys_arg_line(argc, argv, arg_value)){
        return 1;
    }
    for(int i = 0; i < 5; i++){
        printf("%d\n", arg_value[i]);
    }
    return 0;
}