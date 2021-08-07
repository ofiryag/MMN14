/* This file contains the constants for the project */

#ifndef CONSTS_H_
#define CONSTS_H_

#define FILE_NAME_LEN 65535
#define BUFF_SIZE 10000
#define IC_START 100
#define DC_START 0
#define MAX_LINE_LEN 81
#define MAX_FILE_LEN 356
#define MAX_MACRO_LEN 32
#define MAX_LABEL_LEN 32
#define MAX_DIRECTIVE_LEN 7
#define MAX_INSTRACTION_LEN 5
#define MAX_OPERAND_LEN 40
#define MASK_2BIT 3
#define NA -1
#define INST_SIZE 15
#define DIR_SIZE 4
#define MAX_IMM_INT 127
#define MIN_IMM_INT -128
#define REG_LEN 2
#define MAX_INTEGER_LEN 4
#define MAX_DATA_INT 511
#define MIN_DATA_INT -512

/*Error Types*/
enum{NO_ERROR, SYNTAX_ERROR, LABEL_INST_ERROR, DEF_LABEL_ERROR, 
	LONG_LABEL_ERROR, DIR_ERROR, INST_ERROR, FCHAR_LABEL_ERROR, 
	LONG_LINE_ERROR, BAD_ARG_ERROR, ONLY_LABEL_ERROR, ADD_ERROR, 
	LABEL_REG_ERROR, NDEF_LABEL_ERROR, NO_ARG_ERROR, MACRO_ERROR};

/*Boolean*/
enum{FALSE, TRUE};

/*Direction types*/
enum{NUMBER_DIR,DD_DIR,DW_DIR,DB_DIR, ASCIZ_DIR,ENTRY_DIR, EXTERN_DIR};

/*Instruction types*/
enum{R,I,J};

#endif