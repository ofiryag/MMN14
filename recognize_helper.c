/* This file contains functions that tests the validation of the file. */

#include "recognize_helper.h"

keywords instructions[] =
{
  /*R instruction */
  /*Arithmetic*/
  {"add"},{"sub"},{"and"},{"or"},{"nor"},
  /*Copy*/
  {"move"},{"mvhi"},{"mvlo"},
/*<-------------------->*/

  /*I instructions*/
  /*Arithmetic*/
  {"addi"},{"subi"},{"andi"},{"ori"},{"nori"},
  /*Conditional Branching*/
  {"bne"},{"beq"},{"blt"},{"bgt"},
  /*Storage*/
{"lb"},{"sb"},{"lw"},{"sw"},{"lh"},{"sh"},
/*<-------------------->*/

  /*J instructions*/
  {"jmp"}, /*one operand - label or register*/
  {"la"},{"call"},/*one operand - label*/
  {"stop"} /*zero operand*/
};

keywords directives[] = 
{
  {".dh"},{".dw"}.{".db"},{"asciz"},{"extern"},{"entry"}
};


/*TODO - Implement functions below*/
/* checks if the word is a valid label and put it in symbol table */
int is_label(char *line, int* ic, int* dc, int* ec,int *ln, int *error)
{
	char label[MAX_LABEL_LEN];
	char *p = line;
	
	int  i = 0, c = 0, address = 0, ext_flag = FALSE, ext_flag,  data_flag, code_flag, entry_flag;
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

			/* if the label points to directive then the symbol address is the dc */
			if(*(next_word(line)) == '.')
			{
				int i=1;
				if(is_dir(next_word(line)+1, error) <= EXTERN_DIR)
				{
					return TRUE;
				}
				data_flag = TRUE;
				check_errors(ln, error, ec);
			}
				
			/* if the label points to instruction then the symbol addres is the ic */
			else if(is_inst(next_word(line)) !=NA)
			{
				code_flag = TRUE;
				address = *ic;
			}

			/* label cannot be declared twice */
			if(search_sym(label))
			{
				*error = DEF_LABEL_ERROR;
				return FALSE;
			}
			
			to_symbol(label, address, ext_flag, inst_flag,inst_data);
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
	if(instructionIndex>=0 && instructionIndex<5)
    	return R_ARITHMETHIC;

	if(instructionIndex>=5 && instructionIndex<=7)
    	return R_COPY;

    if(instructionIndex>=8 && instructionIndex<=12)
    	return I_ARITHMETIC;

	if(instructionIndex>=13 && instructionIndex<=16)
    	return I_CONDITIONAL_BRANCHING;

	if(instructionIndex>=17 && instructionIndex<=22)
    	return I_STORAGE;

    if(instructionIndex==23)
    	return J_JMP; 

	if(instructionIndex==24)
    	return J_LA; 

	if(instructionIndex==25)
    	return J_CALL; 

	if(instructionIndex==26)
    	return J_STOP; 
}

/* return the instruction name */

char * check_inst_name(int instructionIndex){
	return instructions[instructionIndex];
}

/* checks the validation of the directive sentence */ 
int check_dir(char *line, int dirtype, int *dc, int *error)
{	
	switch(dirtype)
	{
	case DW_DIR:
			if(line != NULL)
			{
				char data[MAX_DW_INT_LENGTH];
				int integer, i;
				symbol_node * node;
				
				while(TRUE)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_DW_INT_LENGTH && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 32 bits */
						if(integer >= MIN_DW_INT && integer <= MAX_DW_INT)
							to_data(integer, dc,DW_SIZE); /*TODO - pass to "to_data" the type of the DIR, so it could extend DC accrodingly - DONE*/
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
				char data[MAX_DH_INT_LENGTH];
				int integer, i;
				symbol_node * node;
				
				while(TRUE)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_DH_INT_LENGTH && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 16 bits */
						if(integer >= MIN_DH_INT && integer <= MAX_DH_INT)
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
				char data[MAX_DB_INT_LENGTH];
				int integer, i;
				symbol_node * node;
				
				while(TRUE)
				{
					if(*line == '-' || *line == '+' || isdigit(*line))
					{
						data[0] = *line;
						line++;
						
						for(i=1;i<MAX_DB_INT_LENGTH && isdigit(*line);i++)
						{
							data[i] = *line;
							line++;
						}
						
						data[i] = '\0';
						integer = atoi(data);
						
						/* checks if the  integer fits 8 bits */
						if(integer >= MIN_DB_INT && integer <= MAX_DB_INT)
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
						to_data(ch, dc,CHAR_SIZE);
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
						to_symbol(label, FALSE, TRUE, FALSE,FALSE,FALSE,FALSE);
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
	int instruction = is_inst(line); //change to inst index
    int instType = check_inst_type(instruction);
	line = next_word(line);
	line = to_comma(line);
	/* if first operand isn't immidiate number */
	if(line == NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}

	switch (instType)
	{
		case R_ARITHMETHIC:validate_inst_r_arithmetic(line,error);
			break;
		case R_COPY:validate_inst_r_copy(line,error);
			break;
		case I_CONDITIONAL_BRANCHING || I_STORAGE:validate_inst_i_conditional_branching_or_storage(line,error);
			break;
		case I_ARITHMETIC:validate_inst_i_arithmetic_or_storage(line,error);
			break;
		case I_ARITHMETIC:validate_inst_i_arithmetic_or_storage(line,error);
			break;
		case J_JMP:validate_inst_j_jmp(line,error);
			break;
		case J_CALL||J_LA:validate_inst_j_call_or_la(line,error);
			break;
		case J_STOP:validate_inst_j_stop(line,error);
			break;
		default:
			break;
	}
	(*ic)+=INST_SIZE;

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
	if(strlen(operand) == REG_LEN && operand[0] == '$' && operand[1] >= '0' && operand[1] <= '3' && operand[2] <= '1') //32 bits per register)
		return REG_ADDRESS;
	
	/*checks if label */
	if(isalpha(operand[0]))
	{
		return LABEL_ADDRESS;
	}
	*error = ADD_ERROR;
	return NA;
}

/*validate that using R arithmetic instruction is int the correct syntax, for example add $1, $2, $3*/
int validate_inst_r_arithmetic(char *line, int *error)
{
	char *p = line;
	if(*p !='$')
	{
			*error = SYNTAX_ERROR;
			return FALSE;
	}
	validate_register_operand(line,error);
	line = to_comma(line);

	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}	
	validate_register_operand(line,error);

	line = to_comma(line);
	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}	
	validate_register_operand(line,error);

	if(next_word(line)!=NULL)
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
}
/*validate that using R copy instruction is int the correct syntax, for example add $1, $2*/

int validate_inst_r_copy(char *line, int *error)
{
	char *p = line;
	if(*p !='$')
	{
			*error = SYNTAX_ERROR;
			return FALSE;
	}
	validate_register_operand(line,error);
	line = to_comma(line);
	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
	validate_register_operand(line,error);
	if(next_word(line)!=NULL)
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
}

/*validate that using I arithmetic instruction is int the correct syntax, for example add $1, -35, $2*/

int validate_inst_i_arithmetic_or_storage(char *line, int *error)
{
	char *p = line;
	if(*p !='$')
	{
			*error = SYNTAX_ERROR;
			return FALSE;
	}
	validate_register_operand(line,error);
	line = to_comma(line);
	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
	validate_immed_operand(line,error);
	line = to_comma(line);
	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
	validate_register_operand(line,error);
	if(next_word(line)!=NULL)
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
}

/*validate that using I conditional branching or storage instruction is int the correct syntax, for example add $1, $2, label*/
int validate_inst_i_conditional_branching_or_storage(char *line, int *error)
{
	char *p = line;
	if(*p !='$')
	{
			*error = SYNTAX_ERROR;
			return FALSE;
	}
	validate_register_operand(line,error);
	line = to_comma(line);
	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
	validate_register_operand(line,error);
	line = to_comma(line);
	if(next_word(line)!=NULL)
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
	validate_label_operand(line,error);
	if(line==NULL)
	{
		*error = SYNTAX_ERROR;
		return FALSE;
	}
}

/*validate that using j jmp instruction is in the correct syntax, for example jmp label OR jmp $reg*/
int validate_inst_j_jmp(char *line, int *error)
{
	char *p = line;
	if (validate_register_operand(line,error) == FALSE && validate_label_operand(line,error))
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
	return TRUE;
}

/*validate that using j call or la instructions is in the correct syntax, for example call label || la label*/
int validate_inst_j_call_or_la(char *line, int *error)
{
	char *p = line;
	if (validate_label_operand(p,error) == FALSE)
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
	return TRUE;
}

/*validate that using j stop instructions is in the correct syntax, for example stop    */
int validate_inst_j_stop(char *line, int *error)
{
	char *p = line;
	to_comma(p);
	if(p!=NULL)
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
	return TRUE;
}


//validate that register operand doesn't contain invalid characters
int validate_register_operand(char *line,char * error)
{
	char *p = line;
	if(p[0] != '$' || p[1] < '0' && p[1] > '3' || p[2] > '1') //migth cause error index out of range if p size is 2 chars
		{
			*error = REG_IS_NOT_VALID;
			return FALSE;
		}
	else
	return TRUE;
}

//validate that immed doesn't contain invalid characters
int validate_immed_operand(char *line,char * error) // validate is contains digits
{
	char *p = line;
	while(isdigit(p))
	{
		p=p+1;
	}

	//check that the are no invalid characters between operands
	if(*p =! ',' || *p =!isspace) 
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
	return TRUE;
}

//validate that label doesn't contain invalid characters
int validate_label_operand(char *line,char * error) // validate is contains digits
{
	char *p = line;
	if(isalpha(p[0]==FALSE)
	{
			*error = SYNTAX_ERROR;
			return FALSE;
	}

	while(isdigit(p)||isalpha)
	{
		p=p+1;
	}

	//check that the are no invalid characters between operands
	if(*p =! ',' || *p =!isspace) 
	{
		*error = SYNTAX_ERROR;
			return FALSE;
	}
	return TRUE;
}

//validate that label doesn't contain invalid characters
int validate_label_exists_on_symbole_table(char *line,char * error) 
{
	char *p = line;
	char label[MAX_LABEL_LEN];
	int c =0;
		/* label characters can be only letters or digits */
		while (*p != '\0') 
		{
			label[c] = *p;
			p++;
			c++;
		}
	if(search_sym(label)==NULL) // label should be on symbole table
	{
	 		*error = LABEL_DOESNT_EXIST;
			return FALSE;
	}
	return TRUE;
}