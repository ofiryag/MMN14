/*This file contains all the structs of the project.*/
#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct keywords 
{
	char *word;
} keywords;

typedef struct symbol_node
{
	int address;
	char *symbol;
	int ext_flag;
	int inst_flag;
	struct symbol_node *next;
} symbol_node;

