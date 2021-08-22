/*This file contains all the structs of the project.*/
#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct keywords 
{
	char *word;
} keywords;

typedef struct data_node /*addresses of the symbols*/
{
	int address;
	int dir_type;
	char * symbol;
	char * data;
	struct data_node *next;
} data_node;

typedef struct symbol_node /*the symbols node*/
{
	int address;
	char *symbol;
	int external_flag;
	int data_flag;
	int code_flag;
	int entry_flag;
	struct symbol_node *next;
} symbol_node;

typedef struct instruction_node
{
	int instructiontype;
	int address : 32;
	struct instruction_node *next;
	union
        {
	        struct instruction_node_r
	        {
	        unsigned int opcode : 6;
	        unsigned int rs : 5;
	        unsigned int rd : 5;
	        unsigned int rt : 5;
	        unsigned int funct : 5;
	        } instruction_node_r;

	        struct instruction_node_j
	        {
	         unsigned int opcode : 6;
	         unsigned int reg : 1;
	         unsigned int address : 25;
	        } instruction_node_j;

	        struct instruction_node_i
	        {
	        unsigned int opcode : 6;
	        unsigned int rs : 5;
	        unsigned int rt : 5;
	        unsigned int immed : 16;
	        } instruction_node_i;
        }instruction_details;
} instruction_node;

typedef struct ent_ext_node
{
	char *label;
	int address;
	int ext_flag;
	struct ent_ext_node *next;
} ent_ext_node;

#endif