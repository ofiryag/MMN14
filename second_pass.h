/* This file contains the includes and function prototypes for the second_pass.c file */

#ifndef SECOND_PASS_H_
#define SECOND_PASS_H_

#include "structs.h"
#include "consts.h"
#include "errors.h"
#include "read_line.h"
#include "tables.h"
#include "validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the second pass of the project */
void second_pass(FILE *, char *, int* ,int* ,int* , int* , int*);

/* the function reads each line, checks its validation and incoding it */
void read_line2(char *,FILE *,FILE *,FILE *, int*, int*);

#endif
