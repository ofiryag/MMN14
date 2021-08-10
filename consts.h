/* This file contains the constants for the project */

#ifndef CONSTS_H_
#define CONSTS_H_

#define FILE_NAME_LEN 65535

#define BUFF_SIZE 10000

#define IC_START 100
#define DC_START 0

#define MAX_LINE_LEN 81
#define MAX_FILE_LEN 356
#define MAX_LABEL_LEN 32
#define MAX_DIRECTIVE_LEN 7
#define MAX_INSTRACTION_LEN 5
#define MAX_OPERAND_LEN 40

#define MASK_2BIT 3

#define NA -1

#define INST_AMOUNT 26
#define DIR_SIZE 4

#define MAX_IMM_INT 127
#define MIN_IMM_INT -128
#define REG_LEN 2

#define DW_SIZE 4
#define DH_SIZE 2
#define DB_SIZE 1

#define MAX_DATA_INT 511
#define MIN_DATA_INT -512

#define MAX_DW_INT 2147483647
#define MIN_DW_INT -2147483648
#define MAX_DW_INT_LENGTH 11

#define MAX_DH_INT 32767
#define MIN_DH_INT -32768
#define MAX_DH_INT_LENGTH 6

#define MAX_DB_INT 127
#define MIN_DB_INT -128
#define MAX_DB_INT_LENGTH 4

#define CHAR_SIZE 1



/*Error Types*/
enum{NO_ERROR, SYNTAX_ERROR, LABEL_INST_ERROR, DEF_LABEL_ERROR, 
	LONG_LABEL_ERROR, DIR_ERROR, INST_ERROR, FCHAR_LABEL_ERROR, 
	LONG_LINE_ERROR, BAD_ARG_ERROR, ONLY_LABEL_ERROR, ADD_ERROR, 
	LABEL_REG_ERROR, NDEF_LABEL_ERROR, NO_ARG_ERROR,LABEL_DOESNT_EXIST};

/*Boolean*/
enum{FALSE, TRUE};

/*Direction types*/
enum{NUMBER_DIR,DH_DIR,DW_DIR,DB_DIR, ASCIZ_DIR,ENTRY_DIR, EXTERN_DIR};

/*Instruction types*/
enum{R_ARITHMETHIC,R_COPY,I_ARITHMETIC,I_CONDITIONAL_BRANCHING,I_STORAGE,J_JMP,J_LA,J_CALL,J_STOP};

/*Address types*/
enum{LABEL_ADDRESS, REG_ADDRESS};


#endif