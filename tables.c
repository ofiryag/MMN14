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
instruction_node *new_inst(int opcode, int rs, int rt, int rd, int funct,int immed,int reg,int address,int *ic)
{
	instruction_node *new = (instruction_node *)malloc(sizeof(instruction_node));
	new->address = *ic;
	new->next = NULL;
	/* if R instruction */
	if(opcode==R_MIN_OPCODE || opcode==R_MAX_OPCODE)
	{
		new->instructiontype = R;
		new->type.inst.opcode = opcode;
		new->type.inst.rs = rs;
		new->type.inst.rd = rd;
		new->type.inst.rt = rt;
		new->type.inst.funct = funct;
	}
	/* if I instruction */
	else if(opcode >= I_MIN_OPCODE && opcode <= I_MAX_OPCODE){
		new->type.inst.opcode = opcode;
		new->type.inst.rs = rs;
		new->type.inst.rt = rt;
		new->type.inst.immed = immed;
		new->instructiontype = I;
	}
	/* if J instruction */
	else if(opcode >= J_MIN_OPCODE && opcode <= J_MAX_OPCODE){
		new->type.inst.opcode = opcode;
		new->type.inst.reg = reg;
		new->type.inst.address = address;
		new->instructiontype = J;

	}
	/* if Stop instruction */
		else if(opcode==STOP_OPCODE){
		new->type.inst.opcode = opcode;
		new->type.inst.reg = 0;
		new->type.inst.address = 0;
		new->instructiontype = J;
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
void to_inst(int opcode, int rs, int rt, int rd, int funct,int immed,int reg,int address,int *ic)
{
	instruction_node *temp = instruction_head;
	instruction_node *new = new_inst(opcode, rs,rt, rd, funct,immed,reg,address,*ic);
	
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
	char one_byte[5];
	char one_byte_as_hex[5];
	char data_as_binary[32]; 
	int mask = MASK_2BIT, i, j;
	int line;
	char* unused_bits = "**";
	char data_as_hex [13];

	
	fprintf(ob_file, "\t%d\t%d\n\n", (*ic) - 100, *dc);
	line = 100;
	while(temp_inst != NULL)
	{
		
		if(temp_inst->instructiontype == R)
		{
			char binary_Opcode[7] = convert_decimal_to_binary(temp_inst->instruction_details->inst_r->type->inst->opcode,6);
			char binary_rs[7] = convert_decimal_to_binary(temp_inst->instruction_details->inst_r->type->inst->rs,5);
			char binary_rt[7] = convert_decimal_to_binary(temp_inst->instruction_details->inst_r->type->inst->rt,5);
			char binary_rd[7] = convert_decimal_to_binary(temp_inst->instruction_details->inst_r->type->inst->rd,5);
			char binary_funct[7] = convert_decimal_to_binary(temp_inst->instruction_details->inst_r->type.inst->funct,5);
			strcat(data_as_binary, binary_Opcode);
			strcat(data_as_binary, binary_rs);
			strcat(data_as_binary, binary_rt);
			strcat(data_as_binary, binary_rd);
			strcat(data_as_binary, binary_funct);
			strcat(data_as_binary, "00000"); //unused
			for (i = 0; i < sizeof(data_as_binary); i+4)
			{
				for (int j = 0; j<4; i++)
				{
					one_byte[j] = data_as_binary[i+j];
				}
				convert_binary_to_hexadecimal(one_byte,data_as_hex);
				strcat(data_as_hex,data_as_hex);
			}
			
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

char * convert_decimal_to_binary(int decimal,int bitSize)
{
	int decimal, c, k;
	char data[bitSize+1];
  for (c = bitSize; c >= 0; c--)
  {
    k = decimal >> c;

    if (k & 1)
      data[bitSize]="1";
    else
      data[bitSize]="0";
  }
  return data;
}

void * convert_binary_to_hexadecimal(char * one_byte,char * byte_as_hex)
{
	char *a = one_byte;
	int num = 0;
	do {
    int b = *a=='1'?1:0;
    num = (num<<1)|b;
    a++;
	} while (*a);
	sprintf(byte_as_hex, "%x", num);
}

// Function to create map between binary
// number and its equivalent hexadecimal
void createMap(unordered_map<string, char> *um)
{
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'A';
    (*um)["1011"] = 'B';
    (*um)["1100"] = 'C';
    (*um)["1101"] = 'D';
    (*um)["1110"] = 'E';
    (*um)["1111"] = 'F';
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
