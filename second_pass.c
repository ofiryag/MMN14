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
	
	/* checks if its an instruction sentence */
	if(is_inst(line) >= MOV_INST)
	{
		int instindex = is_inst(line);
		int addressing1 = 0, addressing2 = 0, addressing3 = 0;
		int i, data = 0, reg_src = 0, reg_dst = 0,immed=0;
		char label[MAX_LABEL_LEN];
		line = next_word(line);
		char instname = check_inst_name(check_inst_type(instindex));

		switch(instname){
			case 'add':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(0,addressing1,addressing2,addressing3,1,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'sub':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(0,addressing1,addressing2,addressing3,2,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'and':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(0,addressing1,addressing2,addressing3,3,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'or':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(0,addressing1,addressing2,addressing3,4,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'nor':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(0,addressing1,addressing2,addressing3,5,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'move':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			to_inst(1,addressing1,0,addressing3,1,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'mvhi':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			to_inst(1,addressing1,0,addressing3,2,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'mvlo':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			to_inst(1,addressing1,IRELEVANT,addressing3,3,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'addi':
			addressing1 = check_addressing(line, error);
			immed = (int)(to_comma(line));
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(10,addressing1,IRELEVANT,addressing3,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'subi':
			addressing1 = check_addressing(line, error);
			immed = (int)(to_comma(line));
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(11,addressing1,IRELEVANT,addressing3,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'andi':
			addressing1 = check_addressing(line, error);
			immed = (int)(to_comma(line));
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(12,addressing1,IRELEVANT,addressing3,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'ori':
			addressing1 = check_addressing(line, error);
			immed = (int)(to_comma(line));
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(13,addressing1,IRELEVANT,addressing3,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'nori':
			addressing1 = check_addressing(line, error);
			immed = (int)(to_comma(line));
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(14,addressing1,IRELEVANT,addressing3,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;
			
			case 'bne':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(15,addressing1,addressing2,addressing3,IRELEVANT,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'beq':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(16,addressing1,addressing2,addressing3,IRELEVANT,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'blt':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(17,addressing1,addressing2,addressing3,IRELEVANT,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'bgt':
			addressing1 = check_addressing(line, error);
			addressing2 = check_addressing(to_comma(line), error);
			addressing3 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(18,addressing1,addressing2,addressing3,IRELEVANT,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'lb':
			addressing1 = check_addressing(line, error);
			immed = check_addressing(to_comma(line), error);
			addressing2 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(19,addressing1,addressing2,IRELEVANT,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'sb':
			addressing1 = check_addressing(line, error);
			immed = check_addressing(to_comma(line), error);
			addressing2 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(20,addressing1,addressing2,IRELEVANT,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'lw':
			addressing1 = check_addressing(line, error);
			immed = check_addressing(to_comma(line), error);
			addressing2 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(21,addressing1,addressing2,IRELEVANT,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'sw':
			addressing1 = check_addressing(line, error);
			immed = check_addressing(to_comma(line), error);
			addressing2 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(22,addressing1,addressing2,IRELEVANT,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'lh':
			addressing1 = check_addressing(line, error);
			immed = check_addressing(to_comma(line), error);
			addressing2 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(23,addressing1,addressing2,IRELEVANT,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'sh':
			addressing1 = check_addressing(line, error);
			immed = check_addressing(to_comma(line), error);
			addressing2 = check_addressing(to_comma(to_comma(line,error),error));
			to_inst(24,addressing1,addressing2,IRELEVANT,immed,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'jmp':
			addressing1 = check_addressing(next_word(line), error);
			if(is_label(next_word(line)))
				if(is_dir(next_word(line)) == "entry")
					to_inst(30,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,addressing1,ic);
				else
					to_inst(30,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,ic);
			else
				to_inst(30,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,REG_ADDRESS,addressing1,ic);
			(*ic++);
			break;

			case 'la':
			addressing1 = check_addressing(next_word(line), error);
			if(is_dir(next_word(line)) == "entry")
				to_inst(31,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,addressing1,ic);
			else
				to_inst(31,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,REG_ADDRESS,0,ic);
			(*ic++);
			break;

			case 'call':
			addressing1 = check_addressing(next_word(line), error);
			immed = check_addressing(to_comma(line), error);
			if(is_dir(next_word(line)) == "entry")
				to_inst(32,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,addressing1,ic);
			else
				to_inst(32,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,ic);
			(*ic++);
			break;

			case 'stop':
			to_inst(63,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT,IRELEVANT);
			(*ic++);
			break;

		}
			line = to_comma(line);
		}
		
	}
}
