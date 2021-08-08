/* This file contains functions for handling the data stractures of the priject.*/

#ifndef TABLES_H_
#define TABLES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "consts.h"

/* creates new node for directive data table and initializing it */
data_node *new_data(int, int *);

/* creates new node for symbol table and initializing it */
symbol_node *new_symbol(char *,int,int,int, int);

/* creates new node for instructions table and initializing it */
instruction_node *new_inst(int,int,int,int,int,int, int *);

/* creates new node for entry or extern labels table and initializing it */
ent_ext_node *new_ent_ext(char *,int,int);

/* adds data to directive data table */
void to_data(int, int *);

/* adds symbol to symbol table */
void to_symbol(char *,int,int,int, int);

/* adds instruction to instruction table */
void to_inst(int,int,int,int,int,int, int *);

/* adds label to entry or extern labels table */
void to_ent_ext(char *,int,int);

/* prints all the tables to the correct files */
void print_to_files(FILE *,FILE *,FILE *, int *,int *);

/* updates the addressess of the directive data table */
void update_DC(int *, int *);

/* returns node from symbol table with the given label */
symbol_node *search_sym(char *);

/* free all tables */
void free_tables();

#endif
