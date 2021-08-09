/* This file contains functions that tests the validation of the file. */

#include "validations.h"

/* checks if the label is valid */
int is_valid_label(char *line,int length, int c, char *label, char* p,int* error)
{
    return is_label_valid_characters(label,c,p,length) && is_label_length_valid(length,error) && is_label_save_word && validate_label_is_not_register(label,error) && validate_label_has_next_word(line,error)
}

int is_label_valid_characters(char* label, int c,char* p,int length ,char * error)
{
    /* label characters can be only letters or digits */
			for(c=0;c<length;c++)
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
}

int is_label_length_valid(int length,int * error)
{
    		/* max length of a label is 31 characters */
			if (length >= MAX_LABEL_LEN) 
			{
				*error = LONG_LABEL_ERROR;
				return FALSE;
			}
}

int is_label_save_word(int length,char*label,int * error)
{
    label[length-1] = '\0';

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
}

int validate_label_is_not_register(char*label, int * error)
{
    	/* check if the label is not a register */	
			if(strlen(label) == REG_LEN && label[0] == '$' && label[1] >= '0' && label[1] <= '3' && label[2] <= '1') //32 registers
			{
				*error = LABEL_REG_ERROR;
				return FALSE;
			}
}

int validate_label_has_next_word(char * line,int* error)
{
    		/* after a label it has to be somthing */
			if(next_word(line) == NULL)
			{
				*error = ONLY_LABEL_ERROR;
				return FALSE;
			}
}