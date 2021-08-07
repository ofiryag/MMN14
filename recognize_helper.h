/*This file contains the includes and function prototypes for the validation.c file. */

#ifndef RECOGNIZE_HELPER_H
#define RECOGNIZE_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_line.h"
#include "consts.h"
#include "structs.h"
/*
#include "tables.h"
#include "errors.h"
*/

/* checks if the word is a valid label and put it in symbol table */
int is_label(char *, int *, int *, int *, int *, int *);

/* checks if the word is a directive and returns it's number */
int is_dir(char *, int *);

/* checks if the word is a instruction and returns it's number */
int is_inst(char *);

/* checks the validation of the directive sentence */
int check_dir(char *, int,  int *, int *);

/* checks the validation of the instruction sentence */
int check_inst(char *, int *, int *);

/* checks operand addressing */
int check_addressing(char *, int *);

#endif
