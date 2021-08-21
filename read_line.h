/* This file contains the includes and function prototypes for the read_line.c file. */

#ifndef READ_LINE_H_
#define READ_LINE_H_

#include <stdio.h>
#include <string.h>
#include "consts.h"
#include "recognize_helper.h"
#include "errors.h"
#include "structs.h"
#include "tables.h"
#include "ctype.h"


/* reads each line and check its validation */
void read_line(char *, int *, int *, int *, int *, int *);

/* returns true if the line is empty or a comment */
int ignore_line(char *);

/* skipping the space chars before the word */
char *skip_space(char *);

/* jumps to the next word */
char *next_word(char *);

/* jumps to the word after the comma */
char *to_comma(char *);

/* jumps to $ */
char *to_dollar(char *);


#endif
