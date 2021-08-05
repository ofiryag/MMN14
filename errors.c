/* This file contains the check_errors function that checks if the error variable contains an error number and prints the error on the screen. */

#include "errors.h"

/* returns true if there was an error and prints the error on the screen */
int check_errors(int *ln, int *error, int *ec)
{
	if(*error != NO_ERROR)
	{
		/* if we find an error, ec(error count) is increase by 1 */
		(*ec)++;
		switch (*error)
		{
			case SYNTAX_ERROR:						
				printf("line %d: Syntax error. \n",*ln);
				break ;
			case LABEL_INST_ERROR:
				printf("line %d: label cannot be an instruction.\n", *ln);
				break ;
			case DEF_LABEL_ERROR:
				printf("line %d: Previously defined label. \n",*ln);
				break;
			case LONG_LABEL_ERROR:
				printf("line %d: label contains more than 30 characters. \n",*ln);
				break;
			case DIR_ERROR:
				printf("line %d: Unrecognized directive. \n",*ln);
				break;
			case INST_ERROR:
				printf("line %d: unrecognized insruction. \n",*ln);
				break;
			case FCHAR_LABEL_ERROR:
				printf("line %d: The first character of the label must be a letter. \n", *ln);
				break;
			case LONG_LINE_ERROR:
				printf("line %d: To many characters for one line. The number of characters per line must be less than 80.\n",*ln);
				break;
			case ONLY_LABEL_ERROR:
				printf("Line %d: line should contain more than a label\n",*ln);
				break;
			case ADD_ERROR:
				printf("Line %d: Illegal addressing, check operands. \n",*ln);
				break;
			case LABEL_REG_ERROR: 
				printf("Line %d: label cannot be a register \n",*ln);
				break;
			case NDEF_LABEL_ERROR:
				printf("Line %d: label was not defined in the file \n",*ln);
				break;
			case NO_ARG_ERROR:
				printf("Line %d: Error: expecting argument...\n",*ln);
				break;
			case MACRO_ERROR:
				printf("Line %d: Error: macro error \n",*ln);
				break;
		}
		*error = NO_ERROR;
		return TRUE;
	}
	else 
		return FALSE;
}
