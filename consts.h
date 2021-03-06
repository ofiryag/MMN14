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

#define NA -1

#define INST_AMOUNT 26
#define DIR_SIZE 5

#define INST_SIZE 4


#define REG_LEN 3

#define DW_SIZE 4
#define DH_SIZE 2
#define DB_SIZE 1

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

#define R_MIN_OPCODE 0
#define R_MAX_OPCODE 1
#define I_MIN_OPCODE 10
#define I_MAX_OPCODE 24
#define J_MIN_OPCODE 30
#define J_MAX_OPCODE 32
#define STOP_OPCODE 63

#define IRRELEVANT 0




/*Error Types*/
enum{NO_ERROR, SYNTAX_ERROR, LABEL_INST_ERROR, DEF_LABEL_ERROR, 
	LONG_LABEL_ERROR, DIR_ERROR, INST_ERROR, FCHAR_LABEL_ERROR, 
	LONG_LINE_ERROR, BAD_ARG_ERROR, ONLY_LABEL_ERROR, ADD_ERROR, 
	LABEL_REG_ERROR, NDEF_LABEL_ERROR, NO_ARG_ERROR,LABEL_DOESNT_EXIST,REG_IS_NOT_VALID};

/*Boolean*/
enum{FALSE, TRUE};

/*Direction types*/
enum{DH_DIR,DW_DIR,DB_DIR, ASCIZ_DIR, EXTERN_DIR,ENTRY_DIR};

/*Instruction types*/
enum{R,I,J};

/*Sub-Instruction types*/
enum{R_ARITHMETHIC,R_COPY,I_ARITHMETIC,I_CONDITIONAL_BRANCHING,I_STORAGE,J_JMP,J_LA,J_CALL,J_STOP};
/*Address types*/
enum{LABEL_ADDRESS, REG_ADDRESS};

enum{add,sub,and,or,nor,move,mvhi,mvlo,addi,subi,andi,ori,nori,bne,beq,blt,bgt,lb,sb,lw,sw,lh,sh,jmp,la,call,stop};



#endif