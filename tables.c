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
	new->external_flag = ext_flag;
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
		new->instruction_details->inst_r.opcode = opcode;
		new->instruction_details->inst_r.rs = rs;
		new->instruction_details->inst_r.rd = rd;
		new->instruction_details->inst_r.rt = rt;
		new->instruction_details->inst_r.funct = funct;
	}
	/* if I instruction */
	else if(opcode >= I_MIN_OPCODE && opcode <= I_MAX_OPCODE){
		new->instructiontype = I;
		new->instruction_details->inst_i.opcode = opcode;
		new->instruction_details->inst_i.rs = rs;
		new->instruction_details->inst_i.rt = rt;
		new->instruction_details->inst_i.immed = immed;
	}
	/* if J instruction */
	else if(opcode >= J_MIN_OPCODE && opcode <= J_MAX_OPCODE){
		new->instruction_details->inst_j.opcode = opcode;
		new->instruction_details->inst_j.reg = reg;
		new->instruction_details->inst_j.address = address;
		new->instructiontype = J;

	}
	/* if Stop instruction */
		else if(opcode==STOP_OPCODE){
		new->instruction_details->inst_j.opcode = opcode;
		new->instruction_details->inst_j.reg = 0;
		new->instruction_details->inst_j.address = 0;
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
	char one_byte[5];
	char one_byte_as_hex[2];
	char data_as_binary[32]; 
	int mask = MASK_2BIT, i, j;
	
	fprintf(ob_file, "\t%d\t%d\n", (*ic) - 100, *dc);
	while(temp_inst != NULL)
	{
		switch (temp_inst->instructiontype)
		{
		case R:	build_inst_r_data_as_binary(temp_inst,data_as_binary);
			break;
		case I:	build_inst_i_data_as_binary(temp_inst,data_as_binary);
			break;
		case J:	build_inst_j_data_as_binary(temp_inst,data_as_binary);
			break;
		default:
			break;
		}	
		print_output_line(data_as_binary,ob_file,temp_inst,one_byte_as_hex,one_byte);
		temp_inst = temp_inst->next;
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

/*this function will build R instruction node's data to binary string*/
void build_inst_r_data_as_binary(instruction_node * temp_inst, char* data_as_binary)
{
    char binary_Opcode[7];
    char binary_rs[6];
    char binary_rt[6];
    char binary_rd[6];
    char binary_funct[6];
    convert_decimal_to_binary(temp_inst->instruction_details->inst_r.opcode, 6, binary_Opcode);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_r.rs, 5,binary_rs);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_r.rt, 5,binary_rt);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_r.rd, 5,binary_rd);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_r.funct, 5,binary_funct);

    /*Build data as binary string*/
	strcat(data_as_binary, binary_Opcode);
	strcat(data_as_binary, binary_rs);
	strcat(data_as_binary, binary_rt);
	strcat(data_as_binary, binary_rd);
	strcat(data_as_binary, binary_funct);
	strcat(data_as_binary, "00000"); /*unused*/
}

/*this function will build I instruction node's data to binary string*/
void build_inst_i_data_as_binary(instruction_node * temp_inst, char* data_as_binary)
{
    char binary_Opcode[7];
    char binary_rs[6];
    char binary_rt[6];
    char binary_immed[17];
    convert_decimal_to_binary(temp_inst->instruction_details->inst_i.opcode, 6,binary_Opcode);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_i.rs, 5,binary_rs);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_i.rt, 5,binary_rt);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_i.immed,16,binary_immed);

    /*Build data as binary string*/
    strcat(data_as_binary, binary_Opcode);
    strcat(data_as_binary, binary_rs);
    strcat(data_as_binary, binary_rt);
    strcat(data_as_binary, binary_immed);
}

/*this function will build J instruction node's data to binary string*/
void build_inst_j_data_as_binary(instruction_node * temp_inst, char* data_as_binary)
{
    /*Get data as binary*/
    char binary_Opcode[7];
    char binary_reg[6];
    char binary_address[25];
    convert_decimal_to_binary(temp_inst->instruction_details->inst_j.opcode, 6,binary_Opcode);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_j.reg, 1,binary_reg);
    convert_decimal_to_binary(temp_inst->instruction_details->inst_j.address, 24,binary_address);

    /*Build data as binary string*/
     strcat(data_as_binary, binary_Opcode);
     strcat(data_as_binary, binary_reg);
     strcat(data_as_binary, binary_address);
}

/*this function will convert a decimal number into binary*/
void convert_decimal_to_binary(int decimal,int bitSize,char * string)
{
	int c, k;
  for (c = bitSize; c >= 0; c--)
  {
    k = decimal >> c;

    if (k & 1)
      string[bitSize]="1";
    else
      string[bitSize]="0";
  }
  return;
}

void convert_binary_to_hexadecimal(char * one_byte,char * byte_as_hex)
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

/*this function is converting the line from binary to hexadecimal and print it according to the required format for example:
0104	FB	FF	22	35
*/
void print_output_line(char * data_as_binary,FILE *ob_file,instruction_node *temp_inst, char* one_byte_as_hex ,char* one_byte)
{
	if(temp_inst->address<1000)
	{
		fprintf(ob_file, "\t%s%d\t",'0',temp_inst->address); /* print address - IC*/
	}
	else
	{
		fprintf(ob_file, "\t%d\t",temp_inst->address); /* print address - IC*/
	}

	for (int i = 0; i < sizeof(data_as_binary); i+4)
	{
		for (int j = 0; j<4; j++)
		{
			one_byte[j] = data_as_binary[i+j];
		}
		convert_binary_to_hexadecimal(one_byte,one_byte_as_hex);
		fprintf(ob_file, "\t%s", one_byte_as_hex);
		for (int k = 4; k<8; k++)
		{
			one_byte[k] = data_as_binary[i+k];
		}
		fprintf(ob_file, "%s\t", one_byte_as_hex);
	}
	fprintf(ob_file, "\n");
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
		if(!sym_temp->external_flag && !sym_temp->data_flag)
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
