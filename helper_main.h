#ifndef HELPER_MAIN_H
#define HELPER_MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define SIZE_ARG_LINE 5  // const descrip count
void analys_arg_line(int argc, char *argv[], int *arg_value);  // declar func for parsing and analysing command line
void time_to_sleep(int intervale, bool main_process);
#endif