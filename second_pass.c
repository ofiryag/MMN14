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
				to_ent_ext(label, symbol->address, symbol->external_flag);
				return;
			}
		}
		
		else
			return;
	}
	
	/* checks if its an instruction sentence */
	if(is_inst(line) >= NA)
	{
		int instindex = is_inst(line);
		int insttype = check_inst_type(instindex);
		int rs = IRELEVANT, rt = IRELEVANT, rd = IRELEVANT;
		int reg = IRELEVANT,immed=IRELEVANT,address = IRELEVANT,funct = IRELEVANT;
		char label[MAX_LABEL_LEN];
		line = next_word(line);

		if(insttype == R_ARITHMETHIC){
			rs = check_addressing(line, error);
			rt = check_addressing(to_comma(line), error);
			rd = check_addressing(to_comma(to_comma(line)),error);
		}
		else if(insttype == R_COPY){
				rs = check_addressing(line, error);
				rt = check_addressing(to_comma(line), error);
		}
		else if(insttype == I_ARITHMETIC){
				rs = check_addressing(line, error);
				immed = (int)(to_comma(line));
				rd = check_addressing(to_comma(to_comma(line,error),error));
		}
		else if(insttype == I_CONDITIONAL_BRANCHING){
				rs = check_addressing(line, error);
				rt = check_addressing(to_comma(line), error);
				rd = check_addressing(to_comma(to_comma(line,error),error));
		}
		else if(insttype == I_STORAGE){
				rs = check_addressing(line, error);
				immed = check_addressing(to_comma(line), error);
				rt = check_addressing(to_comma(to_comma(line,error),error));
		}
		else if(insttype == J_JMP || insttype == J_LA){
				address = check_addressing(next_word(line), error);

		}
		else if(insttype == J_CALL){
				address = check_addressing(next_word(line), error);
				immed = check_addressing(to_comma(line), error);

		}

		switch(instindex){
			case add:
			to_inst(0,rs,rt,rd,1,immed,reg,address,ic);
			break;

			case sub:
			to_inst(0,rs,rt,rd,2,immed,reg,address,ic);
			break;

			case and:
			to_inst(0,rs,rt,rd,3,immed,reg,address,ic);
			break;

			case or:
			to_inst(0,rs,rt,rd,4,immed,reg,address,ic);
			break;

			case nor:
			to_inst(0,rs,rt,rd,5,immed,reg,address,ic);
			break;

			case move:
			to_inst(1,rs,rt,rd,1,immed,reg,address,ic);
			break;

			case mvhi:
			to_inst(1,rs,rt,rd,2,immed,reg,address,ic);
			break;

			case mvlo:
			to_inst(1,rs,rt,rd,3,immed,reg,address,ic);
			break;

			case addi:
			to_inst(10,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case subi:
			to_inst(11,rs,rd,rd,funct,immed,reg,address,ic);
			break;

			case andi:
			to_inst(12,rs,rd,rd,funct,immed,reg,address,ic);
			break;

			case ori:
			to_inst(13,rs,rd,rd,funct,immed,reg,address,ic);
			break;

			case nori:
			to_inst(14,rs,rd,rd,funct,immed,reg,address,ic);
			break;
			
			case bne:
			to_inst(15,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case beq:
			to_inst(16,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case blt:
			to_inst(17,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case bgt:
			to_inst(18,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case lb:
			to_inst(19,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case sb:
			to_inst(20,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case lw:
			to_inst(21,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case sw:
			to_inst(22,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case lh:
			to_inst(23,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case sh:
			to_inst(24,rs,rt,rd,funct,immed,reg,address,ic);
			break;

			case jmp:
			if(is_label(next_word(line)))
				/* is the next word entry */
				if(is_dir(next_word(line)) == 6)
					to_inst(30,rs,rt,rd,funct,immed,reg,address,ic);
				else
					to_inst(30,rs,rt,rd,funct,immed,reg,address,ic);
			else
				to_inst(30,rs,rt,rd,funct,immed,1,address,ic);
			break;

			case la:
			/* is the next word entry */
			if(is_dir(next_word(line)) == 6)
				to_inst(31,rs,rt,rd,funct,immed,reg,address,ic);
			else
				to_inst(31,rs,rt,rd,funct,immed,1,0,ic);
			break;

			case call:
			/* is the next word entry */
			if(is_dir(next_word(line)) == 6)
				to_inst(32,rs,rt,rd,funct,immed,reg,address,ic);
			else
				to_inst(32,rs,rt,rd,funct,immed,reg,0,ic);;
			break;

			case stop:
			to_inst(63,rs,rt,rd,funct,immed,reg,address,ic);
			break;
			(*ic+=4);

		}

		/* if first operand is a label */
			if(rs == LABEL_ADDRESS)
			{
				for(i=0;*line!=',';i++)
				{
					label[i] = *line;
					line++;
				}
				
				label[i] = '\0';
				symbol = search_sym(label);
				
				if(symbol == NULL)
				{
					*error = NDEF_LABEL_ERROR;
					return;
				}
				
			}

			line = to_comma(line);
		}
		
	}
}
