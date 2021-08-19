/*This file contains the includes and function prototypes for the validation.c file. */

#ifndef RECOGNIZE_HELPER_H
#define RECOGNIZE_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_line.h"
#include "consts.h"
#include "structs.h"
#include "tables.h"
/*#include "errors.h"*/

/* checks if the word is a valid label and put it in symbol table */
int is_label(char *, int *, int *, int *, int *, int *);

/* checks if the word is a directive and returns it's number */
int is_dir(char *, int *);

/* checks if the word is a instruction and returns it's number */
int is_inst(char *);

/* checks the instruction type, and returns the index */
int check_inst_type(int);

/* checks the validation of the directive sentence */
int check_dir(char *, int,  int *, int *);

/* checks the validation of the instruction sentence */
int check_inst(char *, int *, int *);

/* checks operand addressing */
int check_addressing(char *, int *);

/*validate that using R copy instruction is int the correct syntax, for example add $1, $2*/
int validate_inst_r_copy(char*, int*);

/*validate that using R arithmetic instruction is int the correct syntax, for example add $1, $2, $3*/
int validate_inst_r_arithmetic(char *, int *);

/*validate that using I conditional branching or storage instruction is int the correct syntax, for example add $1, $2, label*/
int validate_inst_i_conditional_branching_or_storage(char *, int *);

/*validate that using j jmp instruction is in the correct syntax, for example jmp label OR jmp $reg*/
int validate_inst_j_jmp(char *, int *);

/*validate that label doesn't contain invalid characters*/
int validate_label_operand(char *,char * );

/*validate that using I arithmetic instruction is int the correct syntax, for example add $1, -35, $2*/
int validate_inst_i_arithmetic_or_storage(char *, int *);

/*validate that immed doesn't contain invalid characters*/
int validate_immed_operand(char *,char * );

/*validate that using j call or la instructions is in the correct syntax, for example call label || la label*/
int validate_inst_j_call_or_la(char *, int *);

/*validate that register operand doesn't contain invalid characters*/
int validate_register_operand(char *,char * );

/*validate that using j stop instructions is in the correct syntax, for example stop    */
int validate_inst_j_stop(char *, int *);
#endif
