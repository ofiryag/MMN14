/* This file contains the first_pass function. This funcion runs the first scan on the .as file,
 the purpose of the first pass is to check the validation of each line, calculate the file's memory consumption and
 insert each symbol to a symbol table. */

#include "first_pass.h"
/* the first pass function */
void first_pass(char *file)
{
    char file_name[FILE_NAME_LEN];
	char line[BUFF_SIZE];
    FILE *fp;

    /*initialize variables */
	int ic = IC_START;
	int dc = DC_START;
	int errorCounter = NO_ERROR;
	int lineCounter = 0;
	int error = NO_ERROR;


	/* adds .as ending to the name of the file and opens it */
	strcpy(file_name,file);	
	fp = fopen(file_name,"r");
	strcat(file_name,".as");

	/* checks if the given file is acceptable */
	if(!fp)
	{
		printf("\nError: file \"%s\" does not exist or permission to file is denied.\n",file_name);
		return;
	} 

    /*checks each line and count the ic */
	while(fgets(line,BUFF_SIZE,fp) != NULL)
	{
		lineCounter++;
	    read_line(line, &ic, &dc, &errorCounter, &lineCounter, &error);
		check_errors(&lineCounter, &error, &errorCounter);
	}

    /* updating the dc counting according the IC */
    update_DC(&ic, &error);
    check_errors(&lineCounter, &error, &errorCounter);

    /* prints the total amount of errors */
    if(errorCounter > NO_ERROR)
    {
        printf("\n%s contains %d errors.\n",file_name,errorCounter);
        return;
    }

    /* if there are no errors continue to second pass */
    else
    {
        rewind(fp);
        second_pass(fp, file, &ic, &errorCounter, &lineCounter, &error, &dc);
        return;
    }
}