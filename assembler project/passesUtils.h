#ifndef PASS_UTILS_H
#define PASS_UTILS_H

#include "configurations.h"

/*
 * Function: save_opcode
 * ----------------------------
 * This function calls the appropriate function to save the instruction line
 * to the instruction list according to the opcode code determines the number of 
 * operand for this opcode.
 *
 * code: integer representing the opcode code.
 * str: string representing the operand/s of the opcode.
 *
 * returns a boolean value indicating whether the opcode was successfully  saved or not.
 */
bool save_opcode(int code, char *str);

/*
 * Function: save_zero_op
 * ----------------------------
 * This function saves an opcode with no operands to the instruction list.
 *
 * code: integer representing the opcode code.
 * str: string representing the operand/s of the opcode.(expected to be empty).
 *
 * returns a boolean value indicating whether the opcode was successfully  saved or not.
 */
bool save_zero_op(int code, char *str);

/*
 * Function: save_one_op
 * ----------------------------
 * This function saves an opcode with one operand to the instruction list.
 *
 * code: integer representing the opcode code.
 * str: string representing the operand/s of the opcode.(expected to be empty).
 *
 * returns a boolean value indicating whether the opcode was successfully  saved or not.
 */
bool save_one_op(int code, char *str);

/*
 * Function: is_jmpBneJsr
 * ----------------------------
 *  This function saves an operand and parameters to the instruction list for the
 *  JMP, BNE or JSR instruction.
 *
 * word: structed word containing the instruction's opcode and addressing mode.
 * operand: the instruction's operand string.
 *
 * returns true if the instruction is valid and was processed successfully, 
 * or false otherwise.
 */
bool is_jmpBneJsr(Word structedWord,char *operand);

/*
 * Function: save_two_op
 * ----------------------------
 * This function saves an opcode with two operands to the instruction list.
 *
 * code: integer representing the opcode code.
 * str: string representing the operand(s) of the opcode.
 *
 * returns true if the processing and translation of the operands were successful,
 * or false otherwise.
 */
bool save_two_op(int code,char *str);

/*
 * Function: is_op_addressType_valid
 * ----------------------------
 * This function validates the address mode of an operand for a given opcode.
 *
 * code: integer representing the opcode of the instruction being processed.
 * str: integer representing the address mode of the operand being validated.
 *
 * returns true if the address mode is valid for the given opcode, or false otherwise.
 */
bool is_op_addressType_valid(int code, int addressType);

/*
 * Function: source_to_binary
 * ----------------------------
 * This function converts the source operand into its binary representation.
 *
 * addressType: integer representing the addressing type of the operand(INSTANT, DIRECT, or REGISTER).
 * operand: char pointer to a string representing the operand.
 *
 * returns a true if the function successfully converted the operand to binary, false otherwise.
 */
bool source_to_binary( int addressType, char *operand);

/*
 * Function: dest_to_binary
 * ----------------------------
 * This function converts the destination operand into its binary representation.
 *
 * addressType: integer representing the addressing type of the operand(INSTANT, DIRECT, or REGISTER).
 * operand: char pointer to a string representing the operand.
 *
 * returns true if the operation was successful, and false if an error occurred.
 */
bool dest_to_binary( int addressType, char *operand);

/*
 * Function: two_registers_to_binary
 * ----------------------------
 * This function creates a binary string representation out of the two registers
 * given as parameters.
 *
 * sourceReg: a pointer to a string representing the name of the source register.
 * destReg: a pointer to a string representing the name of the destination register.
 *
 * return void.
 */
void two_registers_to_binary(char *sourceReg,char *destReg);

/*
 * Function: label_value_to_binary
 * ----------------------------
 * This function returns a binary string of the given label adress depending if
 * the value is external or not. label value returns the value in a 
 * binary representation while external label is 1.
 * 
 * value: label address.(0 = external label, 0 != label).
 *
 * return a pointer to a character string that represents the binary value 
 * of the label or external label.
 */
char* label_value_to_binary(int value);

/*
 * Function: find_addressType
 * ----------------------------
 * This function determines the addressing type of an operand string.
 * register\number\label. 
 *
 * op_ptr: a pointer to a string representing the operand to be checked.
 *
 * returns an integer value representing the addressing type of the operand,
 * -1 if the operand not valid.
 */
int find_addressType(char *op_ptr);

/*
 * Function: find_register
 * ----------------------------
 * This function returns the register number matching to a given register name string.
 *
 * r: a string representing the name of a register.
 *
 * returns the register number,or -1 if the string does not represent a valid register.
 */
int find_register(char *r);

/*
 * Function: new_word
 * ----------------------------
 * This function creates and returns a new Word struct with initial values for 
 * its fields and the opcode is set to the code parameter.
 *
 * code: an integer represents the opcode value of the word.
 *
 * return a new Word struct.
 */
Word new_word(int code);

/*
 * Function: word_to_binary
 * ----------------------------
 * This function receives a Word struct as a parameter and returns a string 
 * representing the binary representation of that Word.
 *
 * structedWord: a Word struct containing the data to be converted to binary.
 *
 * return a string containing the binary representation of the Word struct, 
 * with a length of WORD_LEN.
 */
char* word_to_binary(Word structedWord);



/*
 * Function: is_label
 * ----------------------------
 * This function checks whether a given word is a valid label or not.
 *
 * word: a pointer to a string representing a word to check.
 *
 * return true if the word is a valid label, false otherwise.
 */
bool is_label(char *word);


/*
 * Function: is_data
 * ----------------------------
 * This function  identifies if a given string is equal to the ".data" directive
 *
 * returns true if it is, and false otherwise.
 */
bool is_data(char *word);

/*
 * Function: is_string
 * ----------------------------
 * This function  identifies if a given string is equal to the ".string" directive
 *
 * returns true if it is, and false otherwise.
 */
bool is_string(char *word);

/*
 * Function: is_entry
 * ----------------------------
 * This function identifies if a given string is equal to the ".entry" directive.
 *
 * returns true if it is, and false otherwise.
 */
bool is_entry(char *word);

/*
 * Function: is_extern
 * ----------------------------
 * This function identifies if a given string is equal to the ".entry" directive.
 *
 * returns true if it is, and false otherwise.
 */
bool is_extern(char *word);



/*
 * Function: find_label
 * ----------------------------
 * This function searches for a label with the given name in a linked list of labels.
 *
 * label: the name of the label to search for.
 * head: a pointer to the head of the linked list.
 *
 * return a pointer to the label with the given name if found, or NULL if not found.
 */
Label* find_label(char *label, labelNode *head);

/*
 * Function: new_label
 * ----------------------------
 * This function creates a new label and define the name, type and value.
 *
 * name: the name of the label.
 * type: the type of the label (data/code/external).
 *
 * return a pointer to the newly created label.
 */
Label* new_label(char *name, int type);

/*
 * Function: add_label_to_labelList
 * ----------------------------
 * Adds a given label to the label list.
 *
 * label: pointer to the label to be added.
 * head: pointer to the head of the label list.
 *
 * return void.
 */
void add_label_to_labelList(Label *label, labelNode **head);

/*
 * Function: add_to_machineCodeList
 * ----------------------------
 * This function adds given machine code struct to a linked list of machine codes.
 *
 * machine_code: pointer to the machine code struct to be added to the list.
 * head: a double pointer to the head of the linked list.
 *
 * return void.
 */
void add_to_machineCodeList(MachineCode *machine_code, MachineCodeNode **head);

/*
 * Function: newMachineCode
 * ----------------------------
 * This function creates a new machine code struct with the given binary code and address.
 *
 * binary: a pointer to a string containing the binary code for the machine code.
 * add: an integer representing the address of the machine code.
 *
 * return a pointer to the newly created machine code object.
 */
MachineCode* newMachineCode(char *binary, int add);


/*
 * Function: find_commend_code
 * ----------------------------
 * This function validate a given string is a commend word and return its opcode.
 *
 * comp_word: a string that represents a command to search for
 * commends_array:  an array of strings that represent commands to search in.
 *
 * returns the opcode number if commend is valid, else returns -1.
 */
int find_commend_code(char *comp_word,const char *commends_array[]);

/*
 * Function: is_reserved_word
 * ----------------------------
 * This function checks if the given word is a reserved word by comparing it 
 * to an array of reserved words.
 *
 * comp_word: a pointer to a string that represents a word to be checked.
 * reserved_names: an array of strings that represent reserved words.
 *
 * returns true if the comp_word is found in the reserved_names[] array, and false otherwise.
 */
bool is_reserved_word(const char *comp_word, const char *reserved_names[]);

/*
 * Function: free_labelList
 * ----------------------------
 * This function frees all the memory allocated for the label list.
 *
 * head: a pointer to the head of the label list.
 *
 * return void.
 */
void free_labelList(labelNode* head);

/*
 * Function: free_label
 * ----------------------------
 * This function frees the allocated memory of the name string in a Label struct.
 *
 * label: a pointer to a Label struct.
 *
 * return void.
 */
void free_label(Label *label);

/*
 * Function: free_dataList
 * ----------------------------
 * This function frees the allocated memory for the machine code linked list.
 *
 * head: a pointer to the head of the linked list.
 *
 * return void.
 */
void free_data_list(MachineCodeNode *head);

#endif 

