/* This file contains functions for handling the data stractures of the project.*/

#include "tables.h"

data_node *data_head = NULL;
symbol_node *symbol_head = NULL;
instruction_node *instruction_head = NULL;
ent_ext_node *ent_ext_head = NULL;

/* creates new node for directive data table and initializing it */
data_node *new_data(int data, int* dc)
{
	data_node *new = (data_node *)malloc(sizeof(data_node));
	new->address = *dc;
	new->data = data;
	new->next = NULL;
	return new;
}

/* creates new node for symbol table and initializing it */
symbol_node *new_symbol(char *symbol, int address, int ext_flag, int data_flag,int code_flag, int entry_flag)
{
	symbol_node *new = (symbol_node *)malloc(sizeof(symbol_node));
	new->address = address;
	new->symbol = malloc(strlen(symbol));
	strcpy(new->symbol,symbol);
	new->ext_flag = ext_flag;
	new->data_flag = data_flag;
	new->code_flag = code_flag;
	new->entry_flag = entry_flag;
	new->next = NULL;
	return new;
}


/* creates new node for instructions table and initializing it */
instruction_node *new_inst(int insttype, int opcode, int rs, int rd, int data, int era,int *ic)
{
	instruction_node *new = (instruction_node *)malloc(sizeof(instruction_node));
	new->insttype = insttype;
	new->address = *ic;
	new->era = era;
	new->next = NULL;
	
	if(insttype == INST_TYPE)
	{
		new->type.inst.opcode = opcode;
		new->type.inst.rs = rs;
		new->type.inst.rd = rd;
	}
	
	else if(insttype == REG_TYPE)
	{
		new->type.reg.rd = rd;
		new->type.reg.rs = rs;
	}
	
	else if(insttype == DATA_TYPE)
	{
		new->type.data.value = data;
	}
	
	else if(insttype == LABEL_TYPE)
	{
		new->type.label.address = data;
	}
	
	return new;
}

/* creates new node for entry or extern labels table and initializing it */
ent_ext_node *new_ent_ext(char *label, int address, int ext_flag)
{
	ent_ext_node *new = (ent_ext_node *)malloc(sizeof(ent_ext_node));
	new->address = address;
	new->label = malloc(strlen(label));
	strcpy(new->label,label);
	new->ext_flag = ext_flag;
	new->next = NULL;
	return new;
}

/* adds data to directive data table */
void to_data(int data, int *dc,int dataSize)
{
	data_node *temp = data_head;
	data_node *new = new_data(data, dc);
	(*dc)+=dataSize;
	
	if(data_head == NULL)
	{
		data_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* adds symbol to symbol table */
void to_symbol(char *symbol, int address,int ext_flag, int data_flag,int code_flag, int entry_flag)
{
	symbol_node *temp = symbol_head;
	symbol_node *new = new_symbol(symbol, address, ext_flag, data_flag, code_flag, entry_flag);
	
	if(symbol_head == NULL)
	{
		symbol_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* adds instruction to instruction table */
void to_inst_r(int opcode, int rs, int rt, int rd, int funct, int *ic)
{
	instruction_node_r *temp = instruction_head;
	instruction_node_r *new = new_inst(opcode,rs,rt,rd, funct, ic);
	
	if(instruction_head == NULL)
	{
		instruction_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

void to_inst_i(int opcode, int rs, int rt,int immed,int *ic)
{
	instruction_node_i *temp = instruction_head;
	instruction_node_i *new = new_inst(opcode,rs,rt,immed,ic);
	
	if(instruction_head == NULL)
	{
		instruction_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

void to_inst_j(int opcode,int reg,int address,int *ic)
{
	instruction_node_j *temp = instruction_head;
	instruction_node_j *new = new_inst(opcode,reg,address,ic);
	
	if(instruction_head == NULL)
	{
		instruction_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* adds label to entry or extern labels table */
void to_ent_ext(char *label, int  address, int ext_flag)
{
	ent_ext_node *temp = ent_ext_head;
	
	ent_ext_node *new = new_ent_ext(label, address, ext_flag);
	
	if(ent_ext_head == NULL)
	{
		ent_ext_head = new;
		return;
	}
	
	while(temp->next != NULL)
		temp = temp->next;
	
	temp->next = new;
	return;
}

/* prints all the tables to the correct files */
void print_to_files(FILE *ob_file, FILE *ent_file, FILE *ext_file, int* ic, int* dc)
{
	instruction_node *temp_inst = instruction_head;
	data_node *temp_data = data_head;
	ent_ext_node *temp_ent_ext = ent_ext_head;
	char base4_chars[4] = {'*', '#', '%', '!'};
	char data[8]; 
	char era;
	int mask = MASK_2BIT, i, j;
	int line;
	char* unused_bits = "**";
	
	fprintf(ob_file, "\t%d\t%d\n\n", (*ic) - 100, *dc);

	line = 100;
	while(temp_inst != NULL)
	{
		era = base4_chars[temp_inst->era];
		
		if(temp_inst->insttype == INST_TYPE)
		{
			char opcode[3];
			char rs;
			char rd;
			
			for(i=0,j=2;i<2;i++,j-=2)
				opcode[i] = base4_chars[(temp_inst->type.inst.opcode >> j) & mask];
			opcode[2] = '\0';
			
			rs = base4_chars[temp_inst->type.inst.rs];
			rd = base4_chars[temp_inst->type.inst.rd];
			
			fprintf(ob_file, "\t%04d\t%s%s%c%c%c\n", line,unused_bits, opcode, rs, rd, era);
		
		}
		else if(temp_inst->insttype == REG_TYPE)
		{
			char print[4];
			int rsnrd = 0;
			char* temp_unusedbits = "***";
			rsnrd = (temp_inst->type.reg.rs << 3) | temp_inst->type.reg.rd;
			
			for(i=0, j=4;i<4;i++,j-=2)
			{
				print[i] = base4_chars[(rsnrd >> j) & mask];
			}
			print[3] = '\0';
			

		
			fprintf(ob_file, "\t%04d\t%s%s%c\n", line,temp_unusedbits, print, era);
		}
		
		else if(temp_inst->insttype == DATA_TYPE)
		{
			char value[7];
			for(i=0,j=10;i<7;i++,j-=2)
				value[i] = base4_chars[(temp_inst->type.data.value >> j) & mask];
			value[6] = '\0';
			fprintf(ob_file, "\t%04d\t%s%c\n", line,value, era);
		}
	
		else if(temp_inst->insttype == LABEL_TYPE)
		{
			char label[5];
			
			for(i=0,j=6;i<4;i++,j-=2)
				label[i] = base4_chars[(temp_inst->type.label.address >> j) & mask];
			label[4] = '\0';
			
			fprintf(ob_file, "\t%04d\t%s%s%c\n", line, unused_bits,label, era);
		}
			
		line++;
		temp_inst = temp_inst->next;
	}

	while(temp_data != NULL)
	{
		for(i=0,j=12;i<7;i++,j-=2)
			data[i] = base4_chars[(temp_data->data >> j) & mask];
		data[7] = '\0';
		
		fprintf(ob_file, "\t%04d\t%s\n", line, data);
		temp_data = temp_data->next;
		line++;
	}
	
	while(temp_ent_ext != NULL)
	{	
		if(temp_ent_ext->ext_flag == 1)
			fprintf(ext_file, "%s\t%04d\n", temp_ent_ext->label, temp_ent_ext->address);
		else if(temp_ent_ext->ext_flag == 0){

			fprintf(ent_file, "%s\t%04d\n", temp_ent_ext->label, temp_ent_ext->address);
		}
		temp_ent_ext = temp_ent_ext->next;
	}

}

/* updates the addressess of the directive data table */
void update_DC(int *ic, int *error)
{
	data_node *data_temp = data_head;
	symbol_node *sym_temp = symbol_head;
	
	while(data_temp != NULL)
	{
		data_temp->address += (*ic);
		if(data_temp->address > MAX_FILE_LEN)
		{
			*error = SYNTAX_ERROR;
			return;
		}
		data_temp = data_temp->next;
	}
	while(sym_temp != NULL)
	{
		if(!sym_temp->ext_flag && !sym_temp->inst_flag)
			sym_temp->address += (*ic);
		sym_temp = sym_temp->next;
	}
	
}

/* returns node from symbol table with the given label */
symbol_node *search_sym(char *symbol)
{
	symbol_node *temp = symbol_head;
	
	while(temp != NULL)
	{
		if(strcmp(temp->symbol, symbol) == FALSE)
			return temp;
		
		temp = temp->next;
	}
	
	return NULL;
}

/* free all tables */
void free_tables()
{
	data_node *dat = data_head;
	symbol_node *sym = symbol_head;
	instruction_node *inst = instruction_head;
	ent_ext_node *ent_ext = ent_ext_head;
	
	while(data_head != NULL)
	{
		data_head = data_head->next;
		free(dat);
		dat = data_head;
	}
	
	while(symbol_head != NULL)
	{
		symbol_head = symbol_head->next;
		free(sym);
		sym = symbol_head;
	}
	
	while(instruction_head != NULL)
	{
		instruction_head = instruction_head->next;
		free(inst);
		inst = instruction_head;
	}
	
	while(ent_ext_head != NULL)
	{
		ent_ext_head = ent_ext_head->next;
		free(ent_ext);
		ent_ext = ent_ext_head;
	}
}
