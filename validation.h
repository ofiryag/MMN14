/*This file contains the includes and function prototypes for the validation.c file. */

#ifndef VALIDATION_H_
#define VALIDATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tables.h"
#include "read_line.h"
#include "errors.h"
#include "consts.h"
#include "structs.h"

/* checks if the word is a valid label and put it in symbol table */
int is_label(char *, int *, int *, int *, int *, int *);

/* checks if the word is a directive and returns it's number */
int is_dir(char *, int *);

/* checks if the word is a instruction and returns it's number */
int is_inst(char *);

/* checks if the word is a valid macro and put it in symbol table */
int is_macro(char *, int *);

/* checks the validation of the directive sentence */
int check_dir(char *, int,  int *, int *);

/* checks the validation of the instruction sentence */
int check_inst(char *, int *, int *);

/* checks operand addressing */
int check_addressing(char *, int *);

/* checks if the word is a valid macro */
int check_macro(char *, int *);

#endif
