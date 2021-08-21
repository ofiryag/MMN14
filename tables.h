/* This file contains functions for handling the data stractures of the priject.*/

#ifndef TABLES_H_
#define TABLES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "consts.h"
#include <stdarg.h>

/* creates new node for directive data table and initializing it */
data_node *new_data(int, int *,char * );

/* creates new node for symbol table and initializing it */
symbol_node *new_symbol(char *, int , int , int ,int , int );

/* creates new node for instructions table and initializing it */
instruction_node *new_inst(int , int , int , int , int ,int ,int ,int ,int *);

/* creates new node for entry or extern labels table and initializing it */
ent_ext_node *new_ent_ext(char *,int,int);

/* adds data to directive data table */
void to_data(int , int *,char * );

/* adds symbol to symbol table */
void to_symbol(char *, int ,int , int ,int , int);

/* adds instruction to instruction table */
void to_inst(int , int , int , int , int ,int ,int ,int ,int *);

/* adds label to entry or extern labels table */
void to_ent_ext(char *,int,int);

/* prints all the tables to the correct files */
void print_to_files(FILE *ob_file, FILE *ent_file, FILE *ext_file, int* ic, int* dc);

/* returns node from data table with the given label */
data_node *search_data(char *);
/* updates the addressess of the directive data table */
void update_DC(int *, int *);

/* returns node from symbol table with the given label */
symbol_node *search_sym(char *);

/*this function will build R instruction node's data to binary string*/
char *  build_inst_r_data_as_binary(instruction_node *);

/*this function will build I instruction node's data to binary string*/
char *  build_inst_i_data_as_binary(instruction_node * );

/*this function will build J instruction node's data to binary string*/
char *  build_inst_j_data_as_binary(instruction_node * );

/*this function will convert a decimal number into binary*/
char * convert_decimal_to_binary(int ,int  );

/*this function will convert data from binary string into hexadecimal*/
char * convert_binary_to_hexadecimal(char * );

/*this function is converting the line from binary to hexadecimal and print it according to the required format for example:
0104	FB	FF	22	35
*/
void print_output_line(char * ,FILE *,instruction_node *);


/* free all tables */
void free_tables();

#endif
