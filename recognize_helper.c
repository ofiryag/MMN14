/* This file contains functions that tests the validation of the file. */

#include "recognize_helper.h"

keywords instructions[] =
{
  /*R instruction */
  {"add"},{"sub"},{"and"},{"or"},{"nor"},{"move"},{"mvhi"},{"mvlo"},

  /*I instructions*/
  {"addi"},{"subi"},{"andi"},{"ori"},{"nori"},{"bne"},{"beq"},{"blt"},{"bgt"},{"lb"},{"sb"},{"lw"},{"sw"},{"lh"},{"sh"},

  /*J instructions*/
  {"jmp"},{"la"},{"call"},{"stop"}
};

keywords directives[] = 
{
  {".dd"},{".dw"}.{".db"},{"asciz"},{"entry"},{"extern"}
};


/*TODO - Implement functions below*/
/* checks if the word is a valid label and put it in symbol table */
int is_label(char *line, int* ic, int* dc, int* ec,int *ln, int *error)
{
	char label[MAX_LABEL_LEN];
	char *p = line;
	
	int  i = 0, c = 0, address = 0, ext_flag = FALSE, inst_flag = FALSE, macro_flag = FALSE;
	while (*p != '\0') 
	{
		/* if it is label */
		if (*p == ':') 
		{
			/* max length of a label is 31 characters */
			if (i >= MAX_LABEL_LEN) 
			{
				*error = LONG_LABEL_ERROR;
				return FALSE;
			}
			
			p = line;
			/* first character of label have to be letter */
			if (!isalpha(*p))
			{
				*error = FCHAR_LABEL_ERROR;
				return FALSE;
			}
			
			/* label characters can be only letters or digits */
			for(c=0;c<i;c++)
			{
				label[c] = *p;
				
				/* label characters can be only letters or digits */
				if(!isdigit(label[c]) && !isalpha(label[c]))
				{
					*error = SYNTAX_ERROR;
					return FALSE;
				}
				
				p++;
			}
			
			label[c] = '\0';
			
			/* check if the label is one of the saved words - instructions */
			if(is_inst(label) > NA)
			{
				*error = LABEL_INST_ERROR;
				return FALSE;
			}

            /* check if the label is one of the saved words - directives */
			if(is_dir(label) > NA)
			{
				*error = DIR_ERROR;
				return FALSE;
			}
			
			/* check if the label is not a register */	
			if(strlen(label) == REG_LEN && label[0] == '$' && label[1] >= '0' && label[1] <= '3' && label[2] <= '1') //32 registers
			{
				*error = LABEL_REG_ERROR;
				return FALSE;
			}

			/* after a label it has to be somthing */
			if(next_word(line) == NULL)
			{
				*error = ONLY_LABEL_ERROR;
				return FALSE;
			}

//TODO - Implement this part
			/* if the label points to directive or macro then the symbol address is the dc */
			if(*(next_word(line)) == '.')
			{
				char key[MAX_MACRO_LEN];
				symbol_node * node;
				int i=1;

				if(is_dir(next_word(line)+1, error) >= ENTRY_DIR)
				{
					return TRUE;
				}
				
				else{
					/* checks if macro */
					while(*(next_word(line)+i) != ',' && *(next_word(line)+i) !='\0')
					{
						key[i-1] = *(next_word(line)+i);
						i++;
					}
					key[i-1] = '\0';

					if((node = search_sym(key)) != NULL){
						*(next_word(line)-i)=(node-> address);
						macro_flag = TRUE;
					}
				}
                					address = *dc;
				check_errors(ln, error, ec);
			}
				
			/* if the label points to instruction then the symbol addres is the ic */
			else if(is_inst(next_word(line)) !=NA)
			{
				inst_flag = TRUE;
				address = *ic;
			}
			
			/* label cannot be declared twice */
			if(search_sym(label))
			{
				*error = DEF_LABEL_ERROR;
				return FALSE;
			}
			
			to_symbol(label, address, ext_flag, inst_flag);
			return TRUE;
		}
		else
		{
			i++;
			p++;
		}
	}
	return FALSE;
}
/* checks if the word is a directive and returns it's index */
int is_dir(char *line, int *error)
{
	int i, c = 0; 
	char *p = line;
	char dir[MAX_DIRECTIVE_LEN];

	while(!isspace(*p) && *p != '\0')
	{
		if(isalpha(*p))
			dir[c]=*p;
		
		else
		{
			*error = DIR_ERROR;
			return NA;
		}
		
		c++;
		p++;
	}
	
	dir[c] = '\0';
	
	for(i=0;i<DIR_SIZE;i++)
	{

		if (!strcmp(directives[i].word, dir))
		{
			if(next_word(line) == NULL)
			{
				*error = NO_ARG_ERROR;
				return NA;
			}
			
			return i;
		}

	}
	*error = DIR_ERROR;
	return NA;
    }
/* checks if the word is an instruction and returns it's index */
int is_inst(char *line)
{
	int i,c = 0;
	char *p = line;
	char inst[MAX_INSTRACTION_LEN];
	
	while(!isspace(*p) && *p != '\0')
	{
		if(isalpha(*p) && c < MAX_INSTRACTION_LEN)
			inst[c]=*p;
		else
			return NA;
		
		c++;
		p++;
	}
	
	inst[c] ='\0';
	
	for(i=0;i<=INST_AMOUNT;i++)
		if (strcmp(instructions[i].word, inst) == FALSE)
			return i ;
	
	return NA;
}

/* checks if the word is an instruction and returns it's index */
int check_inst_type(int instructionIndex)
{
	if(instructionIndex>0 && instructionIndex<8)
    return R;

    if(instructionIndex>7 && instructionIndex<23)
    return I;

    if(instructionIndex>22 && instructionIndex<27)
    return J;
}

/* checks the validation of the directive sentence */
int check_dir(char *line, int dirtype, int *dc, int *error)
{	
	switch(dirtype)
	{
	case DW_DIR:
			if(line != NULL)
			{
				char data[MAX_INTEGER_LEN];
				int integer, i;
				symbol_node * node;
				
				while(TRUE)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_INTEGER_LEN && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 10 bits */
						if(integer >= MIN_DATA_INT && integer <= MAX_DATA_INT)
							to_data(integer, dc,DW_SIZE); /*TODO - pass to "to_data" the type of the DIR, so it could extend DC accrodingly*/
						else
						{
							*error = BAD_ARG_ERROR;
							return FALSE;
						}
						
						line = skip_space(line);
						
						if(line == NULL)
							return TRUE;
						
						if(*line != ',')
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
						
						line = skip_space(line+1);
						
						if(line == NULL)
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
					}
					
					else 
					{
						*error = SYNTAX_ERROR;
						return FALSE;
					}
				}
			}
			
			else
				return TRUE;
			
			break;
			case DH_DIR:
			if(line != NULL)
			{
				char data[MAX_INTEGER_LEN];
				int integer, i;
				symbol_node * node;
				
				while(TRUE)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_INTEGER_LEN && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 10 bits */
						if(integer >= MIN_DATA_INT && integer <= MAX_DATA_INT)
							to_data(integer, dc,DH_SIZE); /*TODO - pass to "to_data" the type of the DIR, so it could extend DC accrodingly*/
						else
						{
							*error = BAD_ARG_ERROR;
							return FALSE;
						}
						
						line = skip_space(line);
						
						if(line == NULL)
							return TRUE;
						
						if(*line != ',')
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
						
						line = skip_space(line+1);
						
						if(line == NULL)
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
					}
					
					else 
					{
						*error = SYNTAX_ERROR;
						return FALSE;
					}
				}
			}
			
			else
				return TRUE;
			
			break;
			case DB_DIR:
			if(line != NULL)
			{
				char data[MAX_INTEGER_LEN];
				int integer, i;
				symbol_node * node;
				
				while(TRUE)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_INTEGER_LEN && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 10 bits */
						if(integer >= MIN_DATA_INT && integer <= MAX_DATA_INT)
							to_data(integer, dc,DB_SIZE); /*TODO - pass to "to_data" the type of the DIR, so it could extend DC accrodingly*/
						else
						{
							*error = BAD_ARG_ERROR;
							return FALSE;
						}
						
						line = skip_space(line);
						
						if(line == NULL)
							return TRUE;
						
						if(*line != ',')
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
						
						line = skip_space(line+1);
						
						if(line == NULL)
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
					}
					
					else 
					{
						*error = SYNTAX_ERROR;
						return FALSE;
					}
				}
			}
			
			else
				return TRUE;
			
			break;

		/* if the directive is a asciz */
		case ASCIZ_DIR:
			if(line != NULL)
			{
				int ch;

				if(*line == '"') 
				{
					line++;
					while(*line != '"')
					{
						if(*line == '\0')
						{
							*error = SYNTAX_ERROR;
							return FALSE;
						}
						ch = (int)*line;
						to_data(ch, dc); //ADD to dc the size of the string +1 for '/0'
						line++;
					}
					
					ch = 0;
					to_data(ch, dc);
					
					if(skip_space(line+1) == NULL)
						return TRUE;
					
					else
					{
						*error = SYNTAX_ERROR;
						return FALSE;
					}
				}
				
				else
				{
					*error = SYNTAX_ERROR;
					return FALSE;
				}
			}
			
			else 
			{
				*error = SYNTAX_ERROR;
				return FALSE;
			}
			
			break;

		/* if the directive is entry label */
		case ENTRY_DIR :
			return TRUE;
			break;
		
		/* if the directive is external label */
		case EXTERN_DIR :
			
			if(line != NULL)
			{
				char label[MAX_LABEL_LEN];
				int  i=0;
				
				if (!isalpha(*line))
				{
					*error = FCHAR_LABEL_ERROR;
					return FALSE;
				}
				
				for(i=0;!isspace(*line);i++)
				{
					label[i] = *line;
					
					if(!isdigit(label[i]) && !isalpha(label[i]))
					{
						*error = SYNTAX_ERROR;
						return FALSE;
					}
					
					line++;
				}
				
				label[i] = '\0';
				
				if(skip_space(line) == NULL)
				{
					if(search_sym(label) == NULL)
					{
						to_symbol(label, FALSE, TRUE, FALSE, macro_flag);
						return TRUE;
					}
					
					else
					{
						*error = DEF_LABEL_ERROR;
						return FALSE;
					}
				}
				
				else
				{
					*error = SYNTAX_ERROR;
					return FALSE;
				}
			}
			
			else 
			{
				*error = SYNTAX_ERROR;
				return FALSE;
			}
			
			break;
	}
	
	return FALSE;
}
/* checks the validation of the instruction sentence */
int check_inst(char *line, int *error, int *ic)

{
	int opcode = is_inst(line); //change to inst index
    int instTye = check_inst_type(opcode);
	int addressing1, addressing2;
	line = next_word(line);
	
	/* if the instruction doesn't need operands */
	if(opcode > JSR_INST)
	{
		if(line == NULL)
		{
			(*ic)++;
			return TRUE;
		}
		
		else
		{
			*error = SYNTAX_ERROR;
			return FALSE;
		}
	}
	
	if(line == NULL)
	{
		*error = BAD_ARG_ERROR;
		return FALSE;
	}
	
	addressing1 = check_addressing(line, error);
	
	if(*error != NO_ERROR)
		return FALSE;
	
	line = to_comma(line);

	/* if first operand is immidiate number */
	if(addressing1 == IMM_ADDRESS)
	{

		if(opcode > SUB_INST && opcode != PRN_INST)
		{
			*error = ADD_ERROR;
			return FALSE;
		}
		
		if(opcode == PRN_INST)
		{
			if(line == NULL)
			{	
				(*ic) += 2;
				return TRUE;
			}
			
			else
			{
				*error = ADD_ERROR;
				return FALSE;
			}			
		}
		
		(*ic)++;
	}
	
	/* if first operand isn't immidiate number */
	else if(addressing1 > IMM_ADDRESS)
	{

		if(opcode == LEA_INST && addressing1 == SUB_INST)
		{
			*error = ADD_ERROR;
			return FALSE;
		}
		
		if(opcode > SUB_INST && opcode != LEA_INST)
		{
			if(line == NULL)
			{
				(*ic)++;
				
				if(addressing1 == INDEX_ADDRESS)
					(*ic) += 2;
				else
					(*ic)++;
					
				return TRUE;
			}
			
			else
			{
				*error = ADD_ERROR;
				return FALSE;
			}	
		}
		
		if(addressing1 == INDEX_ADDRESS)
			(*ic) += 2;
		else
			(*ic)++;
	}
	
	if(line == NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
	
	addressing2 = check_addressing(line, error);
	
	if(*error != NO_ERROR)
		return FALSE;
	
	if(next_word(line) == NULL && to_comma(line) == NULL)
	{
		/* if second operand is immidiate number */
		if(addressing2 == IMM_ADDRESS)
		{
			if(opcode == CMP_INST || opcode == PRN_INST)
			{
				(*ic) += 2;
				return TRUE;
			}
			
			else 
			{
				*error = ADD_ERROR;
				return FALSE;
			}
		}
		
		/* if second operand isn't immidiate number */
		if(addressing2 > IMM_ADDRESS)
		{
			(*ic)++;
			
			if(addressing2 == INDEX_ADDRESS)
				(*ic) += 2;
			else if((addressing2 == REG_ADDRESS && addressing1 != REG_ADDRESS) || addressing2 == LABEL_ADDRESS)
				(*ic)++;
			
			return TRUE;
		}
	}
	
	else
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
	
	return FALSE;
}
/* checks operand addressing */
int check_addressing(char *line, int *error)
{
	char operand[MAX_OPERAND_LEN];
	int i, j=1;
	symbol_node * node;
	char key[MAX_OPERAND_LEN]; 
	
	for(i=0;!isspace(*line) && *line != '\0' && *line != ',';i++)
	{
		operand[i] = *line;
		line++;
	}
	
	operand[i] = '\0';
	
	if(operand[0] == '#')
	{
		/* checks if macro */
		while(j<i)
		{
			key[j-1] = operand[j];
			j++;
		}
		key[j-1] = '\0';

		if((node = search_sym(key)) != NULL){
			operand[1]=node-> address + '0';
			operand[2]='\0';
		}
		
			if(operand[1] == '-' || operand[1] == '+' || isdigit(operand[1]))
			{

				if((operand[1] == '-' || operand[1] == '+') && !isdigit(operand[2]))
				{
					*error = BAD_ARG_ERROR;
					return NA;
				}
			
				for(i=2;operand[i] != '\0';i++)
				{
					if(!isdigit(operand[i]))
					{
						*error = ADD_ERROR;
						return NA;
					}
				}
			
				i = atoi(operand+1);
			
				if(i > MAX_IMM_INT || i < MIN_IMM_INT)
				{
					*error = BAD_ARG_ERROR;
					return NA;
				}
			
				else
					return IMM_ADDRESS;
		}
		
		else
		{
			*error = SYNTAX_ERROR;
			return NA;
		}
	}
	
	if(strlen(operand) == REG_LEN && operand[0] == '$' && operand[1] >= '0' && operand[1] <= '3' && operand[2] <= '1') //32 registers)
		return REG_ADDRESS;
	
	/*checks if macro */
	if(isalpha(operand[0]))
	{
		char key[MAX_MACRO_LEN];
		int j=0,k=0;
		char value[4] = "[ ]";
		int c =0; 
		for(i=1;i<strlen(operand);i++)
		{
			/* checks if macro */
			if(operand[i] == '[')
			{
				i++;
				k=i;
				while(operand[k] != ']')
				{
					key[j] = operand[k];
					j++;
					k++;
				}
				key[j] = '\0';

				if((node = search_sym(key)) != NULL)
				{
					value[1] = node-> address + '0';
					for(c=0;c<3;c++)
					{
						operand[i-1]=value[c];
						i++;
					}
					operand[i-1] = '\0';
					i=i-3;
				}
								
				if(isdigit(operand[i]))
				{
					
					i++;
					if(operand[i] == ']')
					{
						i++;
						if(operand[i] == '\0')
							return INDEX_ADDRESS; 
					}
				}
				*error = SYNTAX_ERROR;
				return NA;
			}

			if(!isalpha(operand[i]) && !isdigit(operand[i]))
			{
				*error = BAD_ARG_ERROR;
				return NA;
			}
		}
		
		return LABEL_ADDRESS;
	}
	
	*error = ADD_ERROR;
	return NA;
}