/* the second pass purpose is to encode each instruction and it's addresses,
convert each instruction to four base counting and print it to the correct files.*/

#include "second_pass.h"

/* the second pass of the project */
void second_pass(FILE *fp, char *file, int* ic, int* errorCounter, int* lineNumber, int* error, int* dc)
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
	*errorCounter = NO_ERROR;
	*lineNumber = 0;
	*error = NO_ERROR;
	
	/* reads each line of the file */
	while(fgets(line,BUFF_SIZE,fp) != NULL)
	{
		(*lineNumber)++;
		read_line2(line, ob_file, ent_file, ext_file, ic, error,dc,errorCounter,lineNumber);
		check_errors(lineNumber, error, errorCounter);
	}
	
	/* prints the encoded data to the files */
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
	if(*errorCounter > FALSE)
	{
		printf("\n%s contains %d errors.\n",file ,*errorCounter);
		remove(ob_name);
		remove(ent_name);
		remove(ext_name);
		return;
	}	
}

/* the function reads each line, checks its validation and encoding it */
void read_line2(char *line, FILE *ob_file, FILE *ent_file, FILE *ext_file, int *ic, int * error,int *dc,int* errorCounter, int* lineNumber)
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
	    {
	        line= next_word(line);
	    }
		p++;
	}
    p = line;
	while(next_word(p)!=NULL)
	{
	    if((next_word(p)) !=NULL  && is_inst(p)!=NA)
	    {
	        char * word = get_label_name(next_word(p));
	        symbol_node * temp =  search_sym(word);
	        if(temp!=NULL)
	        {
	            if(temp->external_flag)
	            {
	                to_ent_ext(word, (*ic), temp->external_flag);
	            }
	        }
	    }
	    p= next_word(p);
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
	if (is_inst(line) > NA)
	{
	    symbol_node *temp_sym;
	    int instindex = is_inst(line);
	    int insttype = check_inst_type(instindex);
	    int rs = IRRELEVANT, rt = IRRELEVANT, rd = IRRELEVANT;
	    int reg = IRRELEVANT, immed = IRRELEVANT, address = IRRELEVANT, funct = IRRELEVANT;
	    char label[MAX_LABEL_LEN];
	    line = next_word(line);

	    if (insttype == J_JMP) {
	        if (line[0] != '$') {
	            char label_name[MAX_LABEL_LEN];
	            char *x = line;
	            strcpy(label_name, get_next_word(x));
	            temp_sym = search_sym(label_name);
	            int isentry = temp_sym->entry_flag;
	            if (isentry == 1){
 	                if(temp_sym->address < check_addressing(label_name,error))
 	                {
 	                    address= check_addressing(label_name,error);
 	                }
	                else
	                {
	                    address= temp_sym->address;
	                }
	                instruction_node * node = search_instruction_by_address((*ic));
	                if(node!=NULL)
	                    node->instruction_details.instruction_node_j.address = address;	            }
	        }

	    } else if (insttype == J_LA) {
	        char label_name[MAX_LABEL_LEN];
	        char *x = line;
	        strcpy(label_name, get_next_word(x));
	        temp_sym = search_sym(label_name);
	        int isentry = temp_sym->entry_flag;
	        if (isentry == 1)
	        {
	            if(temp_sym->address < check_addressing(label_name,error))
	            {
	                address= check_addressing(label_name,error);
	            }
	            else
	            {
	                address= temp_sym->address;
	            }
	            instruction_node * node = search_instruction_by_address((*ic));
	            if(node!=NULL)
	                node->instruction_details.instruction_node_j.address = address;
	        }
	    }
	    else if (insttype == J_CALL) {
	        char label_name[MAX_LABEL_LEN];
	        char *x = line;
	        strcpy(label_name, get_next_word(x));
	        temp_sym = search_sym(label_name);
	        int isentry = temp_sym->entry_flag;
	        if (isentry == 1){
	            if(temp_sym->address < check_addressing(label_name,error))
	            {
	                address= check_addressing(label_name,error);
	            }
	            else
	            {
	                address= temp_sym->address;
	            }
	            instruction_node * node = search_instruction_by_address((*ic));
	            if(node!=NULL)
	                node->instruction_details.instruction_node_j.address = address;
	        }
	    }

	    else if (insttype == I_CONDITIONAL_BRANCHING) {
	        char label_name[MAX_LABEL_LEN];
	        char *x = line;
	        strcpy(label_name, to_comma(to_comma(get_next_word(x))));
	        temp_sym = search_sym(label_name);
	        int islabel1 = temp_sym->code_flag;
	        if (islabel1 == 1){
	            if(temp_sym->address < check_addressing(label_name,error))
	            {
	                address= check_addressing(label_name,error);
	            }
	            else
	            {
	                address= temp_sym->address;
	            }
	            instruction_node * node = search_instruction_by_address((*ic));
	            if(node!=NULL)
	                node->instruction_details.instruction_node_i.immed = address-*ic;
	        }
	    }
	}
	    (*ic) += INST_SIZE;
}


/*this function will return the label's name if it's the next word*/
char *get_label_name(char *line) {
    int i=0,c=0;
    char* p= line;
    char * label_name = (char*) malloc(strlen(line));
    while(*line!='\0' && !isspace(*line))
    {
        label_name[c] = *line;
        line++;
        i++;
    }
    for(c=0;c<i;c++)
    {
        label_name[c] =*p;
        p++;
    }
    label_name[c] ='\0';
    return label_name;
}
