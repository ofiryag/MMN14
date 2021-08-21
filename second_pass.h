/* This file contains the includes and function prototypes for the second_pass.c file */

#ifndef SECOND_PASS_H_
#define SECOND_PASS_H_

#include "structs.h"
#include "consts.h"
#include "errors.h"
#include "read_line.h"
#include "tables.h"
#include "ctype.h"
#include "recognize_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the second pass of the project */
void second_pass(FILE *, char *, int* ,int* ,int* , int* , int*);

/* the function reads each line, checks its validation and encoding it */
void read_line2(char *, FILE *, FILE *, FILE *, int *, int * ,int *,int* , int* );


/*this function will return decimal number from string to int*/
int get_number_from_string(char * ,int ,int* );
/*this function will return the label's name if it's the next word*/
char * get_label_name(char* );
#endif
