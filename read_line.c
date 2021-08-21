/* This file contains functions for scanning the lines of the file, reading and decoding each word in the line. */

#include "read_line.h"
#include <ctype.h>

/* reads each line and check its validation */
void read_line(char *line, int *ic, int *dc, int *errorCounter, int *lineNumber, int *errorType)
{	
	int dirtype;
	char label_name[MAX_LABEL_LEN];
    strcpy(label_name,"");

	/* if the line is empty line or comment ignore it */
	if(ignore_line(line) == TRUE)
		return;

	/* remove the spaces before first letter */
	line = skip_space(line);

	/* if the line is too long */
	if(strlen(line) > MAX_LINE_LEN-1)
	{
		*errorType = LONG_LINE_ERROR;
		return;
	}

    char *p = line;
	/* if the first word is a label and it is vaild then put it in symbol table and go to next word */
	if(is_label(line, ic, dc, errorCounter, lineNumber,  errorType,label_name))
	{
	    int i=0,c =0;
	    while(*p!='\0' && !isspace(*p))
	    {
	        label_name[c] = *p;
	        p++;
	        i++;
	    }
	    char *p = line;
	    for(c=0;c<i;c++)
	    {
	        label_name[c] =*p;
	        p++;
	    }
	    label_name[c-1] ='\0';
	    line = next_word(line);
	}

	if(check_errors(lineNumber, errorType, errorCounter))
		return;
		
	if(!isalpha(*line) && *line != '.')
	{   
		*errorType = SYNTAX_ERROR;
		return;
	}
	
	/* if the word is directive sentence check its validation then put it in data table */
	if(*line == '.')
	{
		dirtype = is_dir(line+1, errorType);
		if(dirtype != NA) 
		{
			line = next_word(line);
			if(check_dir(line, dirtype, dc, errorType,label_name))
				return;
		}
		if(dirtype != NA)
        {
			
			*errorType=0;
		}
	}
	if(check_errors(lineNumber, errorType, errorCounter))
		return;
	
	/* if the word is instruction sentence it checks its validation */
	if(is_inst(line) != NA)
	{
		check_inst(line, errorType, ic);
		check_errors(lineNumber, errorType, errorCounter);
	}
		
	/* if its not directive or instruction sentence */
	else if((*errorType == NO_ERROR))
	{
		*errorType = INST_ERROR;
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
/* jumps to $ */
char *to_dollar(char *line)
{
    if(line == NULL)
        return NULL;

    while(*line != '$' && !isspace(*line))
    {
        if(*line == '\0')
            return NULL;

        line++;
    }

    line++;
    line = skip_space(line);
    return line;
}

