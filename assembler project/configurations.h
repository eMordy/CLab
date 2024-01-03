#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#define MAX_LINE_LEN 81 /*80 chars+'\0'*/
#define MAX_LABEL_LEN 31 /*30 chars+'\0'*/
#define START_ADDRESS 100
#define MAX_MEM_WORDS 256
#define WORD_LEN 15 /*14+1 NULL*/
#define REGISTERS_TOTAL 8

#define COLOR_RED    "\x1b[1;31m" 
#define COLOR_PURPLE    "\x1b[1;35m"
#define COLOR_RESET   "\x1b[0m"

extern const char *RESERVED_NAMES[]; 
extern const char *COMMANDS[];
extern const char *BINARY_COMMANDS[];
extern const char *REGISTERS[];
extern const char *BINARY_REGISTERS[];
extern const char *BINARY_ADDRESS_TYPE[];

/*boolian*/
typedef enum {
	false, 
	true 
} bool;

/*Macro structure*/
typedef struct macro {
	char *name;
	char *value;
} macro;

/*Macro node structure*/
typedef struct macroNode {
	macro data;
	struct macroNode *next;
} macroNode;

/*Label structure*/
typedef struct Label {
	char *name;
	int value;
	int type;
} Label;

/*label node structure*/
typedef struct labelNode {
	Label data;
	struct labelNode *next;
} labelNode;

/*machine code structure*/
typedef struct MachineCode {
	int add;
	char code[WORD_LEN];
} MachineCode;

/*MachineCode node structure*/
typedef struct MachineCodeNode {
	MachineCode data;
	struct MachineCodeNode *next;
} MachineCodeNode;

/*Word data structure*/
typedef struct Word {
	int param1;
	int param2;
	int opcode;
	int source;
	int dest;
	int ERA;
} Word;
	
enum type{
	data,
	string,
	entry,
	external,
	code
};

/*instructions code*/
enum insCodes {
	MOV,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP
};

enum codeType {
	ABSOLUTE, 
	EXTERNAL,
	RELOCATABLE
};

enum addressType {
	INSTANT, /*number*/
	DIRECT,	/*label*/
	JUMP, /*jmp/bne/jsr no parameters*/
	REGISTER /*register*/
};

labelNode *label_list; /*label list head*/
MachineCodeNode *data_list; /*data list head*/
MachineCodeNode *inst_list; /*instruction list head*/

int LINE; /*line counter*/
int IC; /*instruction Counter*/
int DC; /*data Counter*/
bool error; /*error flag*/
bool isSecondPass; /*passes flag*/

#endif 

