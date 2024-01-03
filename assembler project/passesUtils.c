#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "passesUtils.h"
#include "configurations.h"
#include "utils.h" 

bool save_opcode(int code, char *str){

	/*if the opcode has two operands*/
	if (code == MOV || code == CMP || code == ADD ||
        code == SUB || code == LEA) {
		return save_two_op(code, str);
    }
	/*if the opcode has one operand*/
	else if (code == NOT || code == CLR || code == INC ||
               code == DEC || code == JMP || code == BNE ||
               code == RED || code == PRN || code == JSR) {

        return save_one_op(code, str);
    } 
	/*if the opcode has no operans*/
	else {/* code == RTS || code == STOP*/

        return save_zero_op(code, str);
    }
}


bool save_zero_op(int code, char *str){

	Word structedWord = new_word(code);
	MachineCode *machine_code;
 	char *binary_str;

	if(*str != '\0') {
		printf(COLOR_RED "Error in line %d: zero operands expected.\n" COLOR_RESET, LINE);
		return false;
	}
	
	binary_str = word_to_binary(structedWord); /*create instrunction word string*/
	machine_code = newMachineCode(binary_str, IC++);
	free(binary_str);
	add_to_machineCodeList(machine_code, &inst_list);

	return true;
}

bool save_one_op(int code, char *str){

	Word structedWord = new_word(code);
	MachineCode *machine_code;
	char *binary_str;
	char *operand = calloc(MAX_LINE_LEN , sizeof(char));

	if(!operand) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}

	structedWord.ERA = ABSOLUTE;
	
	rescue_word(&str, operand);

	if(!strlen(operand)){

		printf(COLOR_RED "Error in line %d: missing parameter.\n" COLOR_RESET, LINE);
		free(operand);
		return false;
	}

	if(*str != '\0'){

		printf(COLOR_RED "Error in line %d: extra characters.\n" COLOR_RESET, LINE);
		free(operand);
		return false;
	}

	if(code == JMP || code == BNE || code == JSR) {
		if(!is_jmpBneJsr(structedWord, operand)){ /*save operand and parameters for jmpBneJsr*/
			free(operand);
			return false;
		}else{
			return true;
		}
	}
	
	structedWord.dest = find_addressType(operand); /*dest = -1 if not valid*/

	if (structedWord.dest == -1){

		printf(COLOR_RED "Error in line %d: parameter is not valid.\n" COLOR_RESET, LINE);
		free(operand);
		return false;
	}

	if(!is_op_addressType_valid(code, structedWord.dest)){
		free(operand);
		return false;
	}

	binary_str = word_to_binary(structedWord);
	machine_code = newMachineCode(binary_str, IC++);
	free(binary_str);
	add_to_machineCodeList(machine_code, &inst_list);

	if(!dest_to_binary(structedWord.dest, operand)){ 
		free(operand);
		return false;
	}
	
	free(operand);
	return true;
}

bool is_jmpBneJsr(Word structedWord,char *operand){
	
	char *label_str = calloc(MAX_LINE_LEN, sizeof(char));
	char *firstOp = calloc(MAX_LINE_LEN, sizeof(char));
	char *secOp = calloc(MAX_LINE_LEN, sizeof(char));
	char *binary_str;
	MachineCode *machine_code;
	Label *label;

	extract_token( &operand , label_str,'(');

	extract_token( &operand , firstOp,',');

	extract_token( &operand , secOp,')');

	/*check if label is valid*/
	strcat(label_str, ":");
	if(!is_label(label_str)) {

		label_str[strlen(label_str) - 1] = '\0';
		printf(COLOR_RED "Error in line %d: %s invalid label.\n" COLOR_RESET, LINE, label_str);

		free(label_str);
		free(firstOp);
		free(secOp);
		return false; 
	}

	label_str[strlen(label_str) - 1] = '\0';
	if(is_reserved_word(label_str, RESERVED_NAMES)){

		printf(COLOR_RED "Error in line %d: %s is a reserved word - invalid label.\n" COLOR_RESET, LINE, label_str);
		free(label_str);
		free(firstOp);
		free(secOp);
		return false; 
	}


	structedWord.dest = JUMP;

	/*if only label*/
	if(*label_str != '\0' && *firstOp == '\0' && *secOp == '\0'){

		structedWord.param1 = 0;
		structedWord.param2 = 0;
		structedWord.dest = DIRECT; /*if no parameters the adresstype is direct and not jump*/

	}else{ /*if two parameters*/
	
		structedWord.param1 = find_addressType(firstOp);
		structedWord.param2 = find_addressType(secOp);
 	
	 	if (structedWord.param1 == -1 || structedWord.param2 == -1 ){
			printf(COLOR_RED "Error in line %d: parameter is not valid.\n" COLOR_RESET, LINE);
			free(label_str);
			free(firstOp);
			free(secOp);
			return false;
		}
	}

	/*add the first word of instruction*/
	binary_str = word_to_binary(structedWord);
	machine_code = newMachineCode(binary_str, IC++);
	free(binary_str);
	add_to_machineCodeList(machine_code, &inst_list);

	label = find_label(label_str, label_list);
	
	if (isSecondPass){

		if(label){

			binary_str =label_value_to_binary(label->value);
			
			if (label->type == external){

				label = new_label(label_str, external);
                add_label_to_labelList(label, &label_list);
			}

			machine_code = newMachineCode(binary_str, IC++);

		}else{ /*label not defined at all at first fass*/
			printf(COLOR_RED "Error line %d: label %s is not defined in the file.\n" COLOR_RESET, LINE, label_str);
			free(label_str);
			free(firstOp);
			free(secOp);
			return false;
			
		}
	}else{ /*is first pass*/

		binary_str = calloc(WORD_LEN, sizeof(char));
		strcpy(binary_str, "?");
		machine_code = newMachineCode(binary_str, IC++);
	}
	
	free(binary_str);
	add_to_machineCodeList(machine_code, &inst_list);


	/*if there are two parameters - create binary words for them*/
	if(*firstOp != '\0' && *secOp != '\0'){
		if(structedWord.param1 == REGISTER && structedWord.param2 == REGISTER){
			two_registers_to_binary(firstOp,secOp);
		}else{
			if(!source_to_binary(structedWord.param1, firstOp)){ /*create word for param1*/
				return false;
			}
			if(!dest_to_binary(structedWord.param2, secOp)){/*create word for param2*/
				return false;
			}
		}
	}

	free(label_str);
	free(firstOp);
	free(secOp);
	return true;
}


bool save_two_op(int code,char *str){

	Word structedWord = new_word(code);
	MachineCode *machine_code;
	char *binary_str;
	char *firstOp = calloc(MAX_LINE_LEN, sizeof(char));
	char *secOp = calloc(MAX_LINE_LEN, sizeof(char));

	if(!firstOp || !secOp) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}
	
	extract_token(&str, firstOp,',');
	trim_whitespace(firstOp);

	if(strchr(firstOp, ' ') || strchr(firstOp, '\t')){
		printf(COLOR_RED "Error in line %d: missing comma.\n" COLOR_RESET, LINE);
		free(firstOp);
		free(secOp);
		return false;
	}

	rescue_word(&str, secOp);
	trim_whitespace(secOp);

	if(*str != '\0'){
		printf(COLOR_RED "Error in line %d: extra characters.\n" COLOR_RESET, LINE);
		free(firstOp);
		free(secOp);
		return false;
	}


	if(*firstOp == '\0' || *secOp == '\0') {
		printf(COLOR_RED "Error in line %d: missing parameter.\n" COLOR_RESET, LINE);
		free(firstOp);
		free(secOp);
		return false;
	}


	structedWord.source = find_addressType(firstOp);
	structedWord.dest = find_addressType(secOp);
	if (structedWord.source == -1 || structedWord.dest == -1){
		printf(COLOR_RED "Error in line %d: parameter is not valid.\n" COLOR_RESET, LINE);
		free(firstOp);
		free(secOp);
		return false;
	}

	if(code == LEA && structedWord.source != DIRECT){
		printf(COLOR_RED "Error in line %d: operand address type is not valid.\n" COLOR_RESET, LINE);
		free(firstOp);
		free(secOp);
		return false;
	}	

	if(!is_op_addressType_valid(code, structedWord.dest)){
		free(firstOp);
		free(secOp);
		return false;
	}

	binary_str = word_to_binary(structedWord);
	machine_code = newMachineCode(binary_str, IC++);
	free(binary_str);
	add_to_machineCodeList(machine_code, &inst_list);

	if(structedWord.source == REGISTER && structedWord.dest == REGISTER){
		two_registers_to_binary(firstOp, secOp);
	}else{
		if(!source_to_binary(structedWord.source, firstOp)){
			free(firstOp);
			free(secOp);
			return false;
		}
		if(!dest_to_binary(structedWord.dest, secOp)){
			free(firstOp);
			free(secOp);
			return false;
		}
	}

	return true;
}

bool is_op_addressType_valid(int code, int addressType){

	/*instructions that do not allow an INSTANT value to be used as the destination operand*/
    if (code == MOV||code == ADD||code == SUB||code == NOT || code == CLR||code == LEA||code == INC||code == DEC||code ==RED){
        if (addressType == INSTANT) {
            printf(COLOR_RED "Error in line %d: operand address type is not valid.\n" COLOR_RESET, LINE);
            return false;
        }
    }/*no restrictions for JMP, BNE, JRS, PRN, CMP, RTS, STOP*/
    return true;
}

bool source_to_binary( int addressType, char *operand) {
    
    char *binary_str;
	MachineCode *machine_code;
    Label *label;

    if(addressType == INSTANT){
        operand++;/*remove '#'*/
        trim_whitespace(operand);
        binary_str = int_str_to_binary(operand, WORD_LEN - 3); /*leave 2 chars for ERA*/
        
		if(!binary_str){
            free(binary_str);
            return false;
        }
    }
    else if(addressType == REGISTER){
        
        binary_str = calloc(WORD_LEN, sizeof(char));
        strcpy(binary_str, "00");
        strcat(binary_str, BINARY_REGISTERS[find_register(operand)]);
        strcat(binary_str, "00000000");
    } 
    else{/*DIRECT (label)*/
        
        if(isSecondPass){
            
            label = find_label(operand, label_list);
            if(label){
                
                binary_str = label_value_to_binary(label->value);
                if (label->type == external) {

                    label = new_label(operand, external);
                    add_label_to_labelList(label, &label_list);
                }
            }else{

                printf(COLOR_RED "Error line %d: label is not defined in the file.\n" COLOR_RESET, LINE);
                return false;
            }
        }else{ /*is first pass*/
            
            binary_str = calloc(WORD_LEN, sizeof(char));
			strcpy(binary_str, "?");
        }
    } 
   
    machine_code = newMachineCode(binary_str, IC++);
	add_to_machineCodeList(machine_code, &inst_list);
    free(binary_str);
	return true;
    
}

bool dest_to_binary( int addressType, char *operand) {
    
    char *binary_str;
	MachineCode *machine_code;
    Label *label;

    if(addressType == INSTANT){
        operand++;/*remove '#'*/
        trim_whitespace(operand);
        binary_str = int_str_to_binary(operand, WORD_LEN - 3); /*leave 2 chars for ERA*/
        
		if(!binary_str){
            free(binary_str);
            return false;
        }
    }
    else if(addressType == REGISTER){
        
        binary_str = calloc(WORD_LEN, sizeof(char));
        strcpy(binary_str, "00000000");
        strcat(binary_str, BINARY_REGISTERS[find_register(operand)]);
        strcat(binary_str, "00");
    } 
    else{ /*DIRECT (label)*/
        
        if(isSecondPass){
            
            label = find_label(operand, label_list);
			if(label){

                binary_str = label_value_to_binary(label->value);
                if (label->type == external) {
					
                    label = new_label(operand, external);
                    add_label_to_labelList(label, &label_list);
                }
            }else{
                printf(COLOR_RED "Error line %d: label %s is not defined in the file.\n" COLOR_RESET, LINE, operand);
                return false;
            }
        }else{
            
            binary_str = calloc(WORD_LEN, sizeof(char));
			strcpy(binary_str, "?");
        }
    }    

    machine_code = newMachineCode(binary_str, IC++);
	add_to_machineCodeList(machine_code, &inst_list);
	free(binary_str);

	return true;
    
}

void two_registers_to_binary(char *sourceReg,char *destReg){

	char *binary_str = calloc(WORD_LEN, sizeof(char));
	MachineCode *machine_code;	

	strcpy(binary_str, "00");
	strcat(binary_str, BINARY_REGISTERS[find_register(sourceReg)]); /*add sourceReg to binary str*/
	strcat(binary_str, "00");
	strcat(binary_str, BINARY_REGISTERS[find_register(destReg)]); /*add destReg*/
	strcat(binary_str, "00");
	
	machine_code = newMachineCode(binary_str, IC++);
	free(binary_str);
	add_to_machineCodeList(machine_code, &inst_list);

}

char* label_value_to_binary(int value){

	char *binary;

	if(value == 0){ /*is extern label*/

		binary = int_to_binary(1, WORD_LEN-1);

	}else{ /*is label*/

		binary = int_to_binary(value, WORD_LEN-3);
		strcat(binary, "10");
	}

	return binary;
}

int find_addressType(char *op_ptr){
	
	char *operand = calloc(strlen(op_ptr)+1, sizeof(char));

	if(!operand) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return -1;
	}

	strcpy(operand, op_ptr);

	/*if is register*/
	if (find_register(operand) != -1){

		return REGISTER;
	}
	/*if is number*/
	if (is_number(operand)) {

		return INSTANT;
	}
	/*if is label*/
	strcat(operand, ":");
	if (is_label(operand)){
		operand[strlen(operand) - 1] = '\0';
		if(is_reserved_word(operand, RESERVED_NAMES)){
			return -1;
		}

		return DIRECT;
	}
	free(operand);
	/*not valid*/
	return -1;
}


int find_register(char *r){

	int i;
	for (i = 0; i <= REGISTERS_TOTAL; i++) {
		if (strcmp(REGISTERS[i], r) == 0) {
			return i;
		}
	}
	return -1;
}

Word new_word(int code) {

    Word word;
    word.param1 = 0;
    word.param2 = 0;
    word.opcode = code;
    word.source = 0;
    word.dest = 0;
    word.ERA = 0;
    return word;
}

char* word_to_binary(Word structedWord){

	char *binary_str = calloc(WORD_LEN , sizeof(char));
	char *binary_section;
	
	binary_section = int_to_binary(structedWord.param1, 2);
	strcat(binary_str, binary_section);
	free(binary_section);
	binary_section = int_to_binary(structedWord.param2, 2);
	strcat(binary_str, binary_section);
	free(binary_section);
	binary_section = int_to_binary(structedWord.opcode, 4);
	strcat(binary_str, binary_section);
	free(binary_section);
	binary_section = int_to_binary(structedWord.source, 2);
	strcat(binary_str, binary_section);
	free(binary_section);
	binary_section = int_to_binary(structedWord.dest, 2);
	strcat(binary_str, binary_section);
	free(binary_section);
	binary_section = int_to_binary(structedWord.ERA, 2);
	strcat(binary_str, binary_section);
	free(binary_section);
	
	return binary_str;

}

bool is_label(char *word){

    int i, len;
    len = strlen(word);

    if (isalpha(word[0]) == false || word[len - 1] != ':' || (MAX_LABEL_LEN ) < len ){
        return false;
    }

    for (i=1; i < len-1; i++){
        if (isalnum(word[i]) == false) {
			return false;
		}
    }

    return true;
}

bool is_data(char *word){
    return (strcmp(word, ".data") == 0);
}

bool is_string(char *word){
    return (strcmp(word, ".string") == 0);
}

bool is_entry(char *word){
    return (strcmp(word, ".entry") == 0);
}

bool is_extern(char *word){
    return (strcmp(word, ".extern") == 0);
}

Label* find_label(char *label, labelNode *head){
    
    labelNode *current;
    current = head;
    
    while (current != NULL) {

        if (strcmp(current->data.name, label) == 0) {
            return &(current->data);
        } 
        current = current->next;
    }
    return NULL;
}

Label* new_label(char *name, int type){
    
    Label* label = malloc(sizeof(Label));

	/*name*/
    label->name=malloc(strlen(name)+1);
    strcpy(label->name, name);
    
    /*value*/
    if(type == data){
		label->value = START_ADDRESS+DC;
    }if(type == code){
		label->value = START_ADDRESS+IC;
    }if(type == external){
		if(isSecondPass){
			label->value = START_ADDRESS+IC;
		}else{
			label->value = 0;
		}
    }
    
    /*type*/
	label->type = type;

    return label;
}


void add_label_to_labelList(Label *label, labelNode **head){
    
    labelNode *new_node;
    new_node = malloc(sizeof(labelNode));
    
    if (!new_node) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return;
	}
	
	new_node->data = *label; /*add the label to the new labelNode*/
	new_node->next = NULL;
	
	if(*head == NULL){ /*if the label list is empty, set the head pointer to the new node*/
	    *head = new_node; 
	}else{ /*if list is not empty*/
        labelNode *curr;
		curr = *head;
		while (curr->next) { /*iterate through the list until the last node is found*/
			curr = curr->next;
		}
		curr->next = new_node; /*add labeNode to the end of the list*/
	}
}

void add_to_machineCodeList(MachineCode *machine_code, MachineCodeNode **head){
    
    MachineCodeNode *new_node;
    new_node = malloc(sizeof(MachineCodeNode));
    
    if (!new_node) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return;
	}
	
	new_node->data = *machine_code; /*add the machine code to the new MachineCodeNode*/
	new_node->next = NULL;
	
	if(*head == NULL){/*if the machine code list is empty, set the head pointer to the new node*/
	    *head = new_node; 
	}else{ /*if list is not empty*/
        MachineCodeNode *curr;
		curr = *head;
		while (curr->next) { /*iterate through the list until the last node is found*/
			curr = curr->next;
		}
		curr->next = new_node;  /*add MachineCodeNode to the end of the list*/ 
	}
}


MachineCode* newMachineCode(char *binary, int add) {

	MachineCode* machine_code = malloc(sizeof(MachineCode));

	if (!machine_code) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return NULL;
	}
	
	/*address*/
	machine_code->add = START_ADDRESS + add;

	/*code*/
	strncpy(machine_code->code, binary, WORD_LEN);

	return machine_code;
}

int find_commend_code(char *comp_word,const char *commends_array[]){
    
    int i;
    for(i=0; commends_array[i] != NULL; i++){
        if (strcmp(commends_array[i], comp_word) == 0){
			return i;
		}
    }
    return -1;
    
}

bool is_reserved_word(const char *comp_word, const char *reserved_names[]){
    
    int i;
    for(i=0; reserved_names[i] != NULL; i++){
        if (strcmp(reserved_names[i], comp_word) == 0){
			return true;
		}
    }
    return false;
    
}

void free_labelList(labelNode* head) {
	
	labelNode* next;
	
	while(head != NULL) {
		next = head->next;
		free_label(&head->data);
		free(head);
		head = next;
	}
}

void free_label(Label *label){
    free(label->name);
}

void free_data_list(MachineCodeNode *head){

	MachineCodeNode* next;
	
	while(head != NULL) {
		next = head->next;
		free(head);
		head = next;
	}
}


