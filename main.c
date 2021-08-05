/* This file contains the main function of the project that checks if there is a file to assemble and assemble each given file. */

#include<main.h>
int main(int argc, char *argv[]) 
{
    int filesCounter;
	/*Is there a file to compile?*/
	if(argc < 2)
	{
		printf("\n Error: no file to assemble \n");
		exit(0);
	}
	/*Compile each file*/
	for(filesCounter=1;filesCounter<argc;filesCounter++)
	{
		first_pass(argv[filesCounter]);
		free_tables();
	}	
	return 0; 
}
