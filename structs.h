/*This file contains all the structs of the project.*/
#ifndef STRUCTS_H_
#define STRUCTS_H_
typedef struct data_node /*addresses of the symbols*/
{
	int address;
	int data : 10;
	struct data_node *next;
} data_node;

typedef struct symbol_node /*the symbols node*/
{
	int address;
	char *symbol;
	int external_flag;
	int instruction_flag;
	struct symbol_node *next;
} symbol_node;


typedef struct instruction_node
{
	int instructiontype;
	int address : 32;
	struct instruction_node *next;
	struct data
		{
			int value : 32;
		} data;
		
	struct label
		{
			unsigned int address : 32;
		} label;
	
	struct instruction_node_r{
	union type
	{
		struct inst
		{
			unsigned int opcode : 6;
			unsigned int rs : 5;
			unsigned int rd : 5;
			unsigned int rt : 5;
			unsigned int funct : 5;
		} inst;
		
		struct reg
		{
			unsigned int rs : 5;
			unsigned int rd : 5;
			unsigned int rt : 5;
		} reg;
	} type;

	} instruction_node_r;

	struct instruction_node_i
{

	union type
	{
		struct inst
		{
			unsigned int opcode : 6;
			unsigned int rs : 5;
			unsigned int rt : 5;
			unsigned int immed : 16;
		} inst;
		
		struct reg
		{
			unsigned int rs : 5;
			unsigned int immed : 16;
			unsigned int rt : 5;
		} reg;
	} type;
} instruction_node_i;

struct instruction_node_j
{
	union type
	{
		struct inst
		{
			unsigned int opcode : 6;
			unsigned int reg : 1;
			unsigned int address : 25;
		} inst;
		
		struct reg
		{
			unsigned int rt : 6;
		} reg;
		
	} type;
} instruction_node_j;

} instruction_node;


typedef struct ent_ext_node
{
	char *label;
	int address;
	int ext_flag;
	struct ent_ext_node *next;
} ent_ext_node;

#endif