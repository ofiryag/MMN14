/* This file contains functions for handling the data stractures of the project.*/

#include <ctype.h>
#include "tables.h"
#include "read_line.h"

data_node *data_head = NULL;
symbol_node *symbol_head = NULL;
instruction_node *instruction_head = NULL;
ent_ext_node *ent_ext_head = NULL;

/* creates new node for directive data table and initializing it */
data_node *new_data(char * data, int* dc,char * label_name,int * dir_type)
{
	data_node *new = (data_node *)malloc(sizeof(data_node));
	new->address = *dc;
	new->data = malloc(strlen(data));
	strcpy(new->data,data);
	new->next = NULL;
	new->dir_type = *dir_type;
	new->symbol = malloc(strlen(label_name));
	strcpy(new->symbol,label_name);
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
		new->instruction_details.instruction_node_r.rd = rd;
		new->instruction_details.instruction_node_r.rs = rs;
		new->instruction_details.instruction_node_r.rt = rt;
		new->instruction_details.instruction_node_r.funct = funct;
		new->instruction_details.instruction_node_r.opcode = opcode;
	}

	/* if I instruction */
	else if(opcode >= I_MIN_OPCODE && opcode <= I_MAX_OPCODE){
		new->instructiontype = I;
		new->instruction_details.instruction_node_i.opcode = opcode;
		new->instruction_details.instruction_node_i.rs = rs;
		new->instruction_details.instruction_node_i.rt = rt;
		new->instruction_details.instruction_node_i.immed = immed;
	}
	/* if J instruction */
	else if(opcode >= J_MIN_OPCODE && opcode <= J_MAX_OPCODE){
		new->instruction_details.instruction_node_j.opcode = opcode;
		new->instruction_details.instruction_node_j.reg = reg;
		new->instruction_details.instruction_node_j.address = address;
		new->instructiontype = J;

	}
	/* if Stop instruction */
		else if(opcode==STOP_OPCODE){
		    new->instruction_details.instruction_node_j.opcode = opcode;
		    new->instruction_details.instruction_node_j.reg = 0;
		    new->instruction_details.instruction_node_j.address = 0;
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
void to_data(char* data, int *dc,char * label_name,int* dir_type)
{
	data_node *temp = data_head;
	data_node *new = new_data(data, dc,label_name,dir_type);

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
	instruction_node *new = new_inst(opcode, rs,rt, rd, funct,immed,reg,address,ic);
	
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
	char * data_as_binary="";
	fprintf(ob_file, "\t%d\t%d\n", (*ic) - 100, *dc);
	while(temp_inst != NULL)
	{
		switch (temp_inst->instructiontype)
		{
        case R:data_as_binary=build_inst_r_data_as_binary(temp_inst);
			break;
        case I:data_as_binary=build_inst_i_data_as_binary(temp_inst);
			break;
        case J:data_as_binary=build_inst_j_data_as_binary(temp_inst);
			break;
		default:
			break;
		}
		print_output_line_inst_node(data_as_binary,ob_file,temp_inst);
		*ic = temp_inst->address;
		temp_inst = temp_inst->next;
	}

	char binary_data[33];
	int address = 0;
	int counter =0;
	(*ic)+=4;
	strcpy(data_as_binary,"");
	while(temp_data!=NULL)
	{
	    strcpy(binary_data,"");
	    strcpy(data_as_binary,"");
	    char *data = temp_data->data;
	    char * binary=(char*)malloc(33);
	    strcpy(binary,"");
	    if(temp_data->dir_type == ASCIZ_DIR)
	    {
	        data_as_binary = binary;
	        while (*data!='\0')
	        {
	            if(counter==0)
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	            strcpy(binary_data,"");
	            strcpy(binary,"");
	            int x = data[0];
	            strcat(binary_data,convert_decimal_to_binary(x,7)); /*8 bits -> char is 1 byte*/
                strcat(binary,binary_data+1);
                print_byte_data(binary_data,ob_file,*ic);
	            data++;
	            counter+=8;
	            if(counter>=32)
	            {
	                fprintf(ob_file, "%s", "\n");
	                (*ic)+=4;
	                counter-=32;
	            }
	            temp_data->address++;
	        }
	        if(counter==0)
	        {
	            fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	        }
	        counter+=8;
	        fprintf(ob_file, "%s", "00\t");
	    }
	    else if(temp_data->dir_type == DB_DIR)
	    {
	        char *p;
	        while (data!=NULL)
	        {
	            strcpy(binary_data,"");
	            strcpy(binary,"");
	            p=data;
	            if(to_comma(p)!=NULL)
	            {
	                char * x = get_data_until_comma(p);
	                strcat(binary_data,convert_decimal_to_binary(atoi(x),DB_SIZE*8-1)); /*8 bits -> char is 1 byte*/
	            }
	            else
	            {
	                strcat(binary_data,convert_decimal_to_binary(atoi(p),DB_SIZE*8-1)); /*8 bits -> char is 1 byte*/
	            }

	            strcat(binary,binary_data);
	            print_byte_data(binary,ob_file,*ic);
	            data++;
	            counter+=DB_SIZE*8;
	            if(counter==32)
	            {
	                fprintf(ob_file, "%s", "\n");
	                (*ic)+=4;
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter=0;
	            }
	            temp_data->address++;
	            data= to_comma(data);
	        }
	    }
	    else if(temp_data->dir_type == DW_DIR){
	        char *p;
	        while (data!=NULL)
	        {
                strcpy(data_as_binary,"");
	            p=data;
	            if(counter==0)
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	            if(to_comma(p)!=NULL)
	            {
	                char * x = get_data_until_comma(p);
	                strcat(data_as_binary,convert_decimal_to_binary(atoi(x),DW_SIZE*8-1)); /*8 bits -> char is 1 byte*/

	            }
	            else
	            {
	                strcat(data_as_binary,convert_decimal_to_binary(atoi(p),DW_SIZE*8-1)); /*8 bits -> char is 1 byte*/

	            }
	            if(counter>=32)
	            {
	                (*ic)+=4;
	                fprintf(ob_file, "%s", "\n");
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter-=32;
	            }
	            counter+=8;
	            print_byte_data(data_as_binary+24,ob_file,*ic);
	            if(counter>=32)
	            {
	                (*ic)+=4;
	                fprintf(ob_file, "%s", "\n");
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter-=32;
	            }
	            counter+=8;
	            print_byte_data(data_as_binary+16,ob_file,*ic);
	            if(counter>=32)
	            {
	                (*ic)+=4;
	                fprintf(ob_file, "%s", "\n");
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter-=32;
	            }
	            counter+=8;
	            print_byte_data(data_as_binary+8,ob_file,*ic);
	            if(counter>=32)
	            {
	                (*ic)+=4;
	                fprintf(ob_file, "%s", "\n");
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter-=32;
	            }
	            counter+=8;
	            print_byte_data(data_as_binary,ob_file,*ic);
	            temp_data->address++;
	            data= to_comma(data);
	        }
	    }
	    else if(temp_data->dir_type == DH_DIR)
	    {
	        char *p;
	        while (data!=NULL)
	        {
	            strcpy(binary_data,"");
	            p=data;
	            if(to_comma(p)!=NULL)
	            {
	                char * x = get_data_until_comma(p);
	                strcat(data_as_binary,convert_decimal_to_binary(atoi(x),DH_SIZE*8-1)); /*8 bits -> char is 1 byte*/

	            }
	            else
	            {
	                strcat(data_as_binary,convert_decimal_to_binary(atoi(p),DH_SIZE*8-1)); /*8 bits -> char is 1 byte*/
	            }
	            counter+=DH_SIZE*8;
	            print_byte_data(data_as_binary+8,ob_file,*ic);
	            if(counter>=32)
	            {
	                (*ic)+=4;
	                fprintf(ob_file, "%s", "\n");
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter-=32;
	            }
	            print_byte_data(data_as_binary,ob_file,*ic);

	            if(counter>=32)
	            {
	                fprintf(ob_file, "\t%04d\t",*ic); /* print address - IC*/
	                counter-=32;
	            }
	            temp_data->address++;
	            data= to_comma(data);
	        }
	    }
	    address = temp_data->address;
	    temp_data = temp_data->next;
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
char * build_inst_r_data_as_binary(instruction_node * temp_inst)
{
    char * binary=(char*)malloc(33);

    char* binary_opcode = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_r.opcode, 5);
    char* binary_rs =  convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_r.rs, 4);
    char* binary_rt = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_r.rt, 4);
    char* binary_rd = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_r.rd, 4);
    char* binary_funct = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_r.funct, 4);
    char* binary_unused = "000000";

    strcpy(binary,binary_opcode);
    strcat(binary,binary_rs);
    strcat(binary,binary_rt);
    strcat(binary,binary_rd);
    strcat(binary,binary_funct);
    strcat(binary,binary_unused);

    /*Build data as binary string*/
    return binary;
}

/*this function will build I instruction node's data to binary string*/
char * build_inst_i_data_as_binary(instruction_node * temp_inst)
{
    char * binary=(char*)malloc(32);

    char* binary_opcode = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_i.opcode, 5);
    char* binary_rs =convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_i.rs, 4);
    char* binary_rt =convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_i.rt, 4);
    char* binary_immed =convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_i.immed,15);

    strcpy(binary,binary_opcode);
    strcat(binary,binary_rs);
    strcat(binary,binary_rt);
    strcat(binary,binary_immed);

    return binary;
    /*Build data as binary string*/
}



/*this function will build J instruction node's data to binary string*/
char * build_inst_j_data_as_binary(instruction_node * temp_inst)
{
    char * binary=(char*)malloc(32);
    /*Get data as binary*/
    char* binary_opcode = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_j.opcode, 5);
    char* binary_reg = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_j.reg, 0);
    char* binary_address = convert_decimal_to_binary(temp_inst->instruction_details.instruction_node_j.address, 24);

    strcpy(binary,binary_opcode);
    strcat(binary,binary_reg);
    strcat(binary,binary_address);

    /*Build data as binary string*/
    return binary;
}
/*this function will build data node data to binary string*/
char * build_data_as_binary(data_node * temp_data,int * numOfOperands)
{
    char * binary=(char*)malloc(33);
    char * binary_data;
    char * data = temp_data->data;
    strcpy(binary_data,"");
     if(temp_data->dir_type == DB_DIR)
     {
         char *p;
         while (*data!='\0')
         {
             p=data;
             char * x = get_data_until_comma(p);
             strcat(binary_data,convert_decimal_to_binary(x,DB_SIZE)); /*8 bits -> char is 1 byte*/
             data= to_comma(data);
         }
        binary_data = convert_decimal_to_binary(temp_data->data, *numOfOperands);
    }
    else if(temp_data->dir_type == DW_DIR){
         binary_data = convert_decimal_to_binary(temp_data->data, *numOfOperands*DW_SIZE);
        strcpy(binary,binary_data);
    }
    else if(temp_data->dir_type == DH_DIR){
        binary_data = convert_decimal_to_binary(temp_data->data, *numOfOperands*DH_SIZE);
    }
    strcpy(binary,binary_data);
    return binary;
}

char* convert_decimal_to_binary(int n,int bitAmount)
{
    int c, d, t;
    char * p;

    t = 0;
    p = (char*)malloc(bitAmount+1);

    if (p == NULL)
        exit(EXIT_FAILURE);

    for (c = bitAmount ; c >= 0 ; c--)
    {
        d = n >> c;

        if (d & 1)
            *(p+t) = 1 + '0';
        else
            *(p+t) = 0 + '0';

        t++;
    }
    *(p+t) = '\0';

    return  p;
}

/*this function will convert data from binary string into hexadecimal*/
char * convert_binary_to_hexadecimal(char * one_byte)
{
    char x[9];
    memset(x, '\0', sizeof(x));
    strncpy(x, one_byte ,8);

    char *byte_as_hex = (char *)malloc(3*sizeof(char));
	char *a = x;
	int num = 0;
	do {
    int b = *a=='1'?1:0;
    num = (num<<1)|b;
    a++;
	} while (*a);
	sprintf(byte_as_hex, "%02x", num);
	return byte_as_hex;
}

/* jumps get the word between first char and comma */
char * get_data_until_comma(char * line)
{
    int i=0,c=0;
    char* p= line;
    char * data = (char*) malloc(strlen(line));

    while(*line!=',' && !isspace(*line))
    {
        if(*line == '\0')
            return NULL;
        data[c] = *line;
        line++;
        i++;
    }
    for(c=0;c<i;c++)
    {
        data[c] =*p;
        p++;
    }
    data[c] ='\0';
    return data;
}

/*this function is converting the line from binary to hexadecimal and print it according to the required format for example:
0104	FB	FF	22	35
*/
void print_output_line_inst_node(char * data_as_binary,FILE *ob_file,instruction_node *temp_inst)
{
    int i=0,j=0,k=0;
    char one_byte[9];
    char * hexadecimal_line = (char *)malloc(sizeof (char)*9) ;
    char * opposite = (char *)malloc(sizeof (char) * 9) ;
    strcpy(opposite, "");
    strcpy(hexadecimal_line,"");
    fprintf(ob_file, "\t%04d\t",temp_inst->address); /* print address - IC*/
	for ( i = 0; i < 32; i+=8)
	{

		for ( j = 0; j<9; j++)
		{
			one_byte[j] = data_as_binary[i+j];
		}
		char x[3];
		memset(x, '\0', sizeof(x));
		char * hex = convert_binary_to_hexadecimal(one_byte);
		strncpy(x, hex ,2);
		strcat(hexadecimal_line, strcat(x,"\t"));
	}
    opposite = opposite_string(hexadecimal_line);
	fprintf(ob_file, "%s", opposite);

	fprintf(ob_file, "\n"); /*end of line*/
}

void print_output_line_data(char * data_as_binary,FILE *ob_file,int address)
{
    int i=0,j=0,k=0;
    char one_byte[9];
    strcpy(one_byte,"");
    char * hexadecimal_line = (char *)malloc(sizeof (char)*9) ;
    char * opposite = (char *)malloc(sizeof (char) * 9) ;
    strcpy(opposite, "");
    strcpy(hexadecimal_line,"");
    fprintf(ob_file, "\t%04d\t",address); /* print address - IC*/
    for ( i = 0; i < 32; i+=8)
    {

        for ( j = 0; j<8; j++)
        {
            one_byte[j] = data_as_binary[i+j];
        }
        char x[3];
        memset(x, '\0', sizeof(x));
        char * hex = convert_binary_to_hexadecimal(one_byte);
        strncpy(x, hex ,2);
        strcat(hexadecimal_line, strcat(x,"\t"));
    }
    fprintf(ob_file, "%s", hexadecimal_line);

    fprintf(ob_file, "\n"); /*end of line*/
}

/*this function will translate one byte from binary to hexadecimal and print it*/
void print_byte_data(char * one_byte,FILE *ob_file,int address)
{
    int i=0,j=0,k=0;
    char * hexadecimal_line = (char *)malloc(sizeof (char)*9) ;
    strcpy(hexadecimal_line,"");
    char x[3];
    memset(x, '\0', sizeof(x));
    char * hex = convert_binary_to_hexadecimal(one_byte);
    strncpy(x, hex ,2);
    strcat(hexadecimal_line, strcat(x,"\t"));
    fprintf(ob_file, "%s", hexadecimal_line);
}


/*upside down string data*/
char * opposite_string(char * string)
{
    char * opossite = (char *)malloc(sizeof (char)*9) ;
    char *p = string;
    p = to_space(to_space(to_space(string)));
    strcpy(opossite,p);
    p = to_space(to_space(string));
    strncat(opossite+3,p,3);
    p = to_space(string);
    strncat(opossite+6,p,3);
    p = string;
    strncat(opossite+9,p,3);
    return opossite;
}

/* updates the addressess of the directive data table */
void update_DC(int *ic, int *error)
{
    int dc=0;
	data_node *data_temp = data_head;
	symbol_node *sym_temp = symbol_head;
	
	while(data_temp != NULL)
	{
	    dc = data_temp->address;
		data_temp->address = (*ic);
		if(data_temp->address > MAX_FILE_LEN)
		{
			*error = SYNTAX_ERROR;
			return;
		}
		(*ic)+=dc;
		data_temp = data_temp->next;
	}

	while(sym_temp != NULL)
	{
	    if(sym_temp->data_flag)
	    {
	        data_temp = search_data(sym_temp->symbol);
	        if(data_temp!=NULL)
	        {
	            if(sym_temp!=NULL)
	            {
	                sym_temp->address = data_temp->address;
	            }
	        }
	    }
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

/* returns node from symbol table with the given label */
instruction_node * search_instruction_by_address(int *address)
{
    instruction_node *temp = instruction_head;

    while(temp != NULL)
    {
        if(temp->address == address)
            return temp;
        temp = temp->next;
    }
    return NULL;
}



/* returns node from data table with the given label */
data_node *search_data(char *symbol)
{
	data_node *temp = data_head;

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

/* jumps to the next word */
char *to_space(char *line)
{
    if(line == NULL)
        return NULL;

    while(!isspace(*line) && *line != '\0')
    {
        line++;
    }

    line = skip_space(line);

    return line;
}
