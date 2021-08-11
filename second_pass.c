/* the second pass purpose is to encode each instruction and it's addresses,
convert each instruction to four base counting and print it to the correct files.*/

#include "second_pass.h"

/* the second pass of the project */
void second_pass(FILE *fp, char *file, int* ic,int* ec,int* ln, int* error, int* dc)
{
	char ob_name[FILE_NAME_LEN], ent_name[FILE_NAME_LEN], ext_name[FILE_NAME_LEN];
	FILE *ob_file;
	FILE *ent_file;
	FILE *ext_file;
	char line[BUFF_SIZE];

	/* opens 3 files, object file, entry file and extern file */
	strcpy(ob_name,file);
	strcpy(ent_name,file);
	strcpy(ext_name,file);
	strcat(ob_name,".ob");
	strcat(ent_name,".ent");
	strcat(ext_name,".ext");
	ob_file = fopen(ob_name, "w");
	ent_file = fopen(ent_name, "w");
	ext_file = fopen(ext_name, "w");
	
	/* initializing the variables */
	*ic = IC_START;
	*ec = NO_ERROR;
	*ln = 0;
	*error = NO_ERROR;
	
	/* reads each line of the file */
	while(fgets(line,BUFF_SIZE,fp) != NULL)
	{
		(*ln)++;
		read_line2(line, ob_file, ent_file, ext_file, ic, error); 
		check_errors(ln, error, ec);
	}
	
	/* prints the incoded data to the files */
	print_to_files(ob_file, ent_file, ext_file, ic, dc);
	
	/* if one of the files is empty its delete it */
	if(ftell(ob_file) == FALSE)
	{
		fclose(ob_file);
		remove(ob_name);
	}

	if(ftell(ent_file) == FALSE)
	{	
		fclose(ent_file);
		remove(ent_name);
	}
	
	if(ftell(ext_file) == FALSE)
	{
		fclose(ext_file);
		remove(ext_name);
	}
	
	/* closes the files */ 
	fclose(fp);
	fclose(ob_file);
	fclose(ent_file);
	fclose(ext_file);
	
	/* check if there was errors */
	if(*ec > FALSE)
	{
		printf("\n%s contains %d errors.\n",file ,*ec);
		remove(ob_name);
		remove(ent_name);
		remove(ext_name);
		return;
	}	
}

/* the function reads each line, checks its validation and incoding it */
void read_line2(char *line, FILE *ob_file, FILE *ent_file, FILE *ext_file, int *ic, int * error)
{
	char *p = line;
	symbol_node *symbol;
	int dirtype;

	/* if the line is empty line or comment ignore it */
	if(ignore_line(line) == TRUE)
		return;
	
	line = skip_space(line);

	/* checks if there is a label */
	while(*p != '\0')
	{
		if(*p == ':')
			line = next_word(line);
		p++;
	}
	
	/* checks if it is directive sentence */
	if(*line == '.')
	{
		dirtype = is_dir(line+1, error);
		if(dirtype == NA) 
		{
			*error=0;
		}

		
		/* if the sentence is an entry declaration then put it in table */
		if(dirtype == ENTRY_DIR)
		{
			char label[MAX_LABEL_LEN];
			int  i;
			
			line = next_word(line);
			
			/* copy the entry label to char array */
			for(i=0;!isspace(*line);i++)
			{
				label[i] = *line;
				line++;
			}
			
			label[i] = '\0';
			symbol = search_sym(label);
			
			/* if there is no such symbol */
			if(symbol == NULL)
			{
				*error = NDEF_LABEL_ERROR;
				return;
			}	
			else
			{
				to_ent_ext(label, symbol->address, symbol->ext_flag);
				return;
			}
		}
		
		else
			return;
	}
	
	/* checks if it is instruction sentence */
	if(is_inst(line) >= MOV_INST)
	{
		int instindex = is_inst(line);
		int addressing1 = 0, addressing2 = 0;
		int i, data = 0, reg_src = 0, reg_dst = 0;
		char label[MAX_LABEL_LEN];
		line = next_word(line);
		int insttype=check_inst_type(instindex);

		/* checks if it is R_ARITHMETHIC instruction */
		if(insttype == R_ARITHMETHIC){
			to_inst()
		}
		
		/* if the instruction needs two operands */
		else if(instindex < NOT_INST || instindex == LEA_INST)
		{
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			to_inst(INST_TYPE, instindex
, addressing1, addressing2, data, ABSOLUTE, ic);
			(*ic)++;

			/* if first operand is immidiate number */
			if(addressing1 == IMM_ADDRESS)
			{
				symbol_node * node;
				char key[MAX_MACRO_LEN];
				int i = 0;
				line++;
			
			/* if first operand is a label */
			if(addressing1 == LABEL_ADDRESS)
			{
				for(i=0;*line!=',';i++)
				{
					label[i] = *line;
					line++;
				}
				
				label[i] = '\0';
				symbol = search_sym(label);
				
				if(symbol == NULL)
				{
					*error = NDEF_LABEL_ERROR;
					return;
				}
				
				/* if the label is external */
				else if(symbol->ext_flag == TRUE)
				{
					to_ent_ext(label, *ic, symbol->ext_flag);
					to_inst(LABEL_TYPE, instindex
		, addressing1, addressing2, FALSE, EXTERNAL, ic);
					(*ic)++;
				}
				
				else
				{
					to_inst(LABEL_TYPE, instindex
		, addressing1, addressing2, symbol->address, RELOCATABLE, ic);
					(*ic)++;
				}
			}
			
			/* if first operand is an index */
			if(addressing1 == INDEX_ADDRESS)
			{
				/*check if macro */
				char key[MAX_MACRO_LEN];
				int j=1;
				symbol_node * node;

				for(i=0;*line!='[';i++)
				{
					label[i] = *line;
					line++;
				}
				label[i] = '\0';

				symbol = search_sym(label);
				while(line[j] != ']' && line[j] != '\0')
				{
					key[j-1] = line[j];
					j++;
				}
				key[j-1] = '\0';

				if((node = search_sym(key)) != NULL)
				{
					*(line+1) = node->address + '0';
				}

				if(symbol == NULL)
				{
					*error = NDEF_LABEL_ERROR;
					return;
				}

			/* if the index is external */
			else if(symbol->ext_flag == TRUE)
			{
				to_ent_ext(label, *ic, symbol->ext_flag);
				to_inst(LABEL_TYPE, instindex
	, addressing1, addressing2, FALSE, EXTERNAL, ic);
				(*ic)++;
			}
			else
			{
				to_inst(LABEL_TYPE, instindex
	, addressing1, addressing2, symbol->address, RELOCATABLE, ic);
				(*ic)++;
			}
			line++;

			data = *line - '0';
			
			to_inst(DATA_TYPE, instindex
, reg_src, reg_dst, data, ABSOLUTE, ic);
			(*ic)++;
			}

			
			/* if first operand is  a register */
			if(addressing1 == REG_ADDRESS)
			{
				reg_src = *(line+1) - '0';
				if(addressing2 != REG_ADDRESS)
				{
					to_inst(REG_TYPE, instindex
		, reg_src, reg_dst, data, ABSOLUTE, ic);
					(*ic)++;
				}
			}
			
			line = to_comma(line);
		}
		
		/* if the instruction needs one operands */
		else
		{
			addressing2 = check_addressing(line, error);
			to_inst(INST_TYPE, instindex
, addressing1, addressing2, data, ABSOLUTE, ic);
			(*ic)++;
		}
		/* if second operand is immidiate number */

		if(addressing2 == IMM_ADDRESS)
		{
			symbol_node * node;
			char key[MAX_MACRO_LEN];
			int i = 0;
			line++;
			
		/* if second operand is label */
		if(addressing2 == LABEL_ADDRESS)
		{
			for(i=0;!isspace(*line) && *line!='\0';i++)
			{
				label[i] = *line;
				line++;
			}
			
			label[i] = '\0';
			symbol = search_sym(label);

			if(symbol == NULL)
			{
				*error = NDEF_LABEL_ERROR;
				return;
			}
			
			/* if the label is external */
			else if(symbol->ext_flag == TRUE)
			{
				to_ent_ext(label, *ic, symbol->ext_flag);
				to_inst(LABEL_TYPE, instindex
	, addressing1, addressing2, FALSE, EXTERNAL, ic);
				(*ic)++;
			}
			
			else
			{
				to_inst(LABEL_TYPE, instindex
	, addressing1, addressing2, symbol->address, RELOCATABLE, ic);
				(*ic)++;
			}
		}
		
		/* if second operand is index */

		if(addressing2 == INDEX_ADDRESS)
		{
			/*check if macro */
			char key[MAX_MACRO_LEN];
			int j=1;
			symbol_node * node;
			for(i=0;*line!='[';i++)
			{
				label[i] = *line;
				line++;
			}
			label[i] = '\0';
			symbol = search_sym(label);
			while(line[j] != ']' && line[j] != '\0')
			{
				key[j-1] = line[j];
				j++;
			}
			key[j-1] = '\0';
			if((node = search_sym(key)) != NULL)
			{
				*(line+1) = node->address + '0';
			}

			if(symbol == NULL)
			{
				*error = NDEF_LABEL_ERROR;
				return;
			}


		/* if the label is external */
		else if(symbol->ext_flag == TRUE)
		{
			to_inst(LABEL_TYPE, instindex
, addressing1, addressing2, FALSE, EXTERNAL, ic);
			(*ic)++;
		}
		else
		{
			to_inst(LABEL_TYPE, instindex
, addressing1, addressing2, symbol->address, RELOCATABLE, ic);
			(*ic)++;
		}
		line ++;
		data = *line - '0';

		to_inst(DATA_TYPE, instindex, reg_src, reg_dst, data, ABSOLUTE, ic);
		(*ic)++;
		}
		
		/* if second operand is a register */
		if(addressing2 == REG_ADDRESS)
		{
			reg_dst = *(line+1) - '0';
			to_inst(REG_TYPE, instindex
, reg_src, reg_dst, data, ABSOLUTE, ic);
			(*ic)++;
		}
	}
}
