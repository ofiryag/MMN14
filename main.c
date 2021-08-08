/* This file contains the main function of the project that checks if there is a file to assemble and assemble each given file. */

#include "main.h"

int main(int argc, char *argv[])
{
	int i;
	
	/* checks if there is file to compile */
	if(argc < 2)
	{
		printf("\n Error: no file to assemble \n");
		exit(0);
	}
	
	/* compile each file */
	for(i=1;i<argc;i++)
	{
		first_pass(argv[i]);
		free_tables();
	}	
	return 0; 
}
