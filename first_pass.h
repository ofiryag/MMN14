/* This file contains the includes and prototype for the first_pass file. */

#ifndef FIRST_PASS_H_
#define FIRST_PASS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_line.h"
#include "consts.h"
#include "errors.h"
/*#include "tables.h"
#include "structs.h"
#include "second_pass.h"*/

/* The first pass function. This funcion runs the first scan on the .as file,
 the purpose of the first pass is to check the validation of each line, calculate the file's memory consumption and
 insert each symbol to a symbol table. */ 
void first_pass(char *);

#endif
