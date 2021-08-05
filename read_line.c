/* This file contains functions for scanning the lines of the file, reading and decoding each word in the line. */

#include "read_line.h"

/* reads each line and check its validation */
void read_line(char *line, int *ic, int *dc, int *ec, int *ln, int *error)
{	
	int macro, dirtype;

	/* if the line is empty line or comment ignore it */
	if(ignore_line(line) == TRUE)
		return;

	/* remove the spaces before first letter */
	line = skip_space(line);

	/* if the line is too long */
	if(strlen(line) > MAX_LINE_LEN-1)
	{
		*error = LONG_LINE_ERROR;
		return;
	}

	/* if the first word is a label and it is vaild then put it in symbol table and go to next word */
	if(is_label(line, ic, dc, ec, ln,  error))
		line = next_word(line);

	if(check_errors(ln, error, ec))
		return;
		
	if(!isalpha(*line) && *line != '.')
	{   
		*error = SYNTAX_ERROR;
		return;
	}
	
	/* if the word is directive sentence or macro it check its validation then put it in data table */
	if(*line == '.')
	{
		macro =  is_macro(line+1, error);
		if(macro != NA)
		{
			line=next_word(line);
			return;
		}
		dirtype = is_dir(line+1, error);
		
		if(dirtype != NA) 
		{
			line = next_word(line);
			if(check_dir(line, dirtype, dc, error))
				return;
		}
		if(dirtype != NA || macro != NA){
			
			*error=0;
		}
	}
	if(check_errors(ln, error, ec))
		return;
	
	/* if the word is instruction sentence it checks its validation */
	if(is_inst(line) >= MOV_INST)
	{
		check_inst(line, error, ic);
		check_errors(ln, error, ec);
	}
		
	/* if its not directive or instruction sentence */
	else if((*error == NO_ERROR))
	{
		*error = INST_ERROR;
		return;
	}
}

/* returns true if the line is empty or a comment */
int ignore_line(char *line)
{
	if(line == NULL)
		return TRUE;
	if(*line==';')
		return TRUE;
	while(isspace(*line))
	{
		line++;
		if(*line==';')
			return TRUE;
	}
	if(*line=='\0')
		return  TRUE;
	else 
		return FALSE;
}

/* skipping the space chars before the word */
char *skip_space(char *line)
{
	if(line == NULL)
		return NULL;
	
	while(isspace(*line))
		line++;
	
	if (*line == '\0')
		return NULL;
	
	return line;
}

/* jumps to the next word */
char *next_word(char *line)
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

/* jumps to the word after the comma */
char *to_comma(char *line)
{
	if(line == NULL)
		return NULL;
	
	while(*line != ',' && !isspace(*line))
	{
		if(*line == '\0')
			return NULL;
		
		line++;
	}
	
	line++;
	line = skip_space(line);
	return line;
}
