#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "passesUtils.h"
#include "configurations.h"
#include "utils.h" 
#include "firstPass.h" 

bool firstPass(char *file_name){
    
    FILE *am_file;
    char *amFileName, *labelName, *line, *line_ptr, *word_in_line; 
    Label *label;
    bool isLabel, isExtern;
    int commend_Code;
	int spare;/*for handling extra line length*/


    label_list = NULL;    
	data_list = NULL;
	inst_list = NULL;
    error = false;
	isSecondPass = false;
	isExtern = false;
    IC=0, DC=0, LINE=0;

    amFileName = calloc(strlen(file_name)+4 , sizeof(char));/*for .am\0 */
    labelName = calloc(MAX_LABEL_LEN + 1, sizeof(char)); /*extra 1 for ':'*/
    line = calloc(MAX_LINE_LEN + 2 , sizeof(char));/*2 to check if the line is to long*/
    word_in_line = calloc(MAX_LINE_LEN + 1 , sizeof(char));
    
    if(!amFileName || !labelName || !line || !word_in_line) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}
    
    createFileName(file_name, ".am", amFileName);
    am_file = fopen(amFileName, "r"); /*open to read*/
    if (!am_file) { 
        printf(COLOR_RED "Error: The program has failed to open \"%s.am\" file\n" COLOR_RESET, amFileName);
        return false;
    }
    
    
    while(fgets(line, MAX_LINE_LEN+1, am_file) != NULL) {

		LINE++;

        if (strlen(line) >= MAX_LINE_LEN && line[strlen(line)-1] != '\n'){

            printf(COLOR_RED "Error in line %d: line is too long.\n" COLOR_RESET, LINE);
			error = true;
        	while ((spare = fgetc(am_file)) != EOF && spare != '\n'); /*handling extra line length*/
        }

		isLabel = false;
        line_ptr = line;
        rescue_word(&line_ptr, word_in_line);

        /*empty line or comment line*/
        if(*word_in_line == '\0' || *word_in_line == ';'){
			continue;
        }
        
        /*check if the first word in line is a label*/
        if(is_label(word_in_line)){
			
            isLabel = true;
            strcpy(labelName, word_in_line);/*saves label in labelName string*/
            rescue_word(&line_ptr, word_in_line);

        }

		/*handling label with whitespace between label name and ':' */
		if (*line_ptr == ':'){
				
			printf(COLOR_RED "Error in line %d: %s : is invalid label\n" COLOR_RESET, LINE, word_in_line);
			error = false;
			line_ptr++;
			rescue_word(&line_ptr, word_in_line);

		}

        /*is it .data or .string?*/
        if (is_data(word_in_line) || is_string(word_in_line)){
            
            if(isLabel){
                
                labelName[strlen(labelName) - 1] = '\0'; /*remove ":"*/
				
				/*validate label not too long*/
                if(MAX_LABEL_LEN <= strlen(labelName)){
                    printf(COLOR_RED "Error in line %d: label %s is too long.\n" COLOR_RESET, LINE, labelName);
                    error = true;
                }

				label = find_label(labelName, label_list);

				/*validate label is not defined more than one time*/
                if(label){

                    printf(COLOR_RED "Error in line %d: label %s has been defined already.\n" COLOR_RESET, LINE, labelName);
                    error = true;
                    continue;
                }

				/*validate label not a reserves word*/
                if(is_reserved_word(labelName, RESERVED_NAMES)){

                    printf(COLOR_RED "Error in line %d: %s is a reserved word and cannot use as label name\n" COLOR_RESET, LINE, labelName);
                    error = true;
                    continue;
                }
              
                /*valid is label-add new label to label_list*/
                label = new_label(labelName, data);
                add_label_to_labelList(label, &label_list);
            }

            if(is_data(word_in_line)){/*if its .data line*/

				if(!save_data(line_ptr)){/*data_list update- save data into machine code*/
					error = true;
					continue;
				}
            }

            if(is_string(word_in_line)){/*data_list update- save string into machine code*/

				if(!save_string(line_ptr)){
					error = true;
					continue;
				}
            }
            continue;
        }
        
        /*if .extern or .entry?*/
        if(is_entry(word_in_line) || is_extern(word_in_line)){

			if (is_extern(word_in_line)){
				isExtern = true;
			}

            if(isLabel){

                printf(COLOR_RED "Warning in line %d: label defined before entry/extern defenitions is ignored.\n" COLOR_RESET, LINE);
            }
            
            rescue_word(&line_ptr, word_in_line);

            if(*word_in_line == '\0'){
                
                if (isExtern){
                    printf(COLOR_RED "Error in line %d: missing label after .extern declatarion.\n" COLOR_RESET, LINE);
                }else{
                    printf(COLOR_RED "Error in line %d: missing label after .entry declatarion.\n" COLOR_RESET, LINE);
                }
                error = true;
				isExtern = false;
                continue;
            }
            if (*line_ptr != '\0'){

                printf(COLOR_RED "Error in line %d: extra characters.\n" COLOR_RESET, LINE);
                error = true;
				isExtern = false;
                continue;
            }
            
            strcat(word_in_line, ":");

            if(is_label(word_in_line)){

				strcpy(labelName, word_in_line);
                labelName[strlen(labelName) - 1] = '\0'; /*remove ":"*/
                    
				if(is_reserved_word(labelName, RESERVED_NAMES)){

                	printf(COLOR_RED "Error in line %d: %s is a reserved word and cannot use as label name\n" COLOR_RESET, LINE, labelName);
                	error = true;
					isExtern = false;
               		continue;
            	}
			}

            if(isExtern){
                
                if(is_label(word_in_line)){
                    
                    strcpy(labelName, word_in_line);
                    labelName[strlen(labelName) - 1] = '\0'; /*remove ":"*/
                    
                    if (find_label(labelName, label_list)){

						printf(COLOR_RED "Error in line %d: label %s has been defined already.\n" COLOR_RESET, LINE, labelName);
                        error = true;
						isExtern = false;
                        continue;
                    }
					
                    /*add new label to label_list*/
                    label = new_label(labelName, external);
                    add_label_to_labelList(label, &label_list);
                }
                
                else{
					word_in_line[strlen(word_in_line) - 1] = '\0'; 
                    printf(COLOR_RED "Error in line %d: label %s is invalid.\n" COLOR_RESET, LINE, word_in_line);
                    error = true;
					isExtern = false;
                    continue;
                }
				isExtern = false; /*initialize flag for next rounds*/
             
            }else{ /*.entry*/

                if(!is_label(word_in_line)){
					
                    printf(COLOR_RED "Error in line %d: label %s is invalid.\n" COLOR_RESET, LINE, word_in_line);
                    error = true;  
                    continue;    
                }
            }
            continue;
        }
        
        /*if instruction line*/
        if (isLabel){
			
			labelName[strlen(labelName) - 1] = '\0';

            if(find_label(labelName, label_list)){

				printf(COLOR_RED "Error in line %d: label %s has been defined already.\n" COLOR_RESET, LINE, labelName);
                error = true;
                continue;
            }

            if(is_reserved_word(labelName, RESERVED_NAMES)){
				
                printf(COLOR_RED "Error in line %d: %s is a reserved word and cannot use as label name.\n" COLOR_RESET, LINE, labelName);
                error = true;
                continue;
            }
            
            /*add new label*/
            label = new_label(labelName, code);
            add_label_to_labelList(label, &label_list);
        }

		
		if (word_in_line[strlen(word_in_line)-1] == ':'){

			printf(COLOR_RED "Error in line %d: label %s is invalid.\n" COLOR_RESET, LINE, word_in_line);
			error = true;
            continue;
		}

        commend_Code = find_commend_code(word_in_line, COMMANDS);

        if (commend_Code == -1){

            printf(COLOR_RED "Error in line %d: %s is invalid command word.\n" COLOR_RESET, LINE, word_in_line);
            error = true;
            continue;
		}

		if(*line_ptr == ','){

			printf(COLOR_RED "Error in line %d: misplaced comma.\n" COLOR_RESET, LINE);
			error = true;
			line_ptr++;
		}

        if (!save_opcode(commend_Code, line_ptr)){

			error = true;
			continue;
		}
        
    }

	if(MAX_MEM_WORDS < IC+DC) {
		printf(COLOR_RED "Error: IC is: %d, DC is: %d. The max capacity of memory is: %d .\n" COLOR_RESET, IC, DC, MAX_MEM_WORDS);
		error = true;
	}

    free(amFileName);
	free(labelName);
	free(line);
	free(word_in_line);
    
    fclose(am_file);

	/*if error-free all allocated mamory relevant for secondPass*/
    if (error == true){
		printf("Errors found in \"%s\" file. You shall not pass to second pass!\n", file_name);
        free_labelList(label_list);
        free_data_list(data_list);
        return false;
    }

    update_label_list(label_list);
	update_data_list(data_list);
    return true;
    
} /*end of firstPass*/

bool save_data(char *str){
	
	char num[MAX_LINE_LEN];
	char *str_ptr = str;
	char *binary_str;
	MachineCode *machine_code;
	
	/*check if data is valid*/
	if(!is_data_valid(str)){
		return false;
	}

	while (*str_ptr != '\0') {   

		extract_token(&str_ptr, num, ','); /*extract integer token using comma as delimiter*/
		trim_whitespace(num); /*remove any leading or trailing whitespace from the token*/
		binary_str = int_str_to_binary(num, WORD_LEN - 1); /*convert integer token to binary representation*/

		if(!binary_str) {
			free(binary_str);
			return false;
		}

		machine_code = newMachineCode(binary_str, DC++);
		free(binary_str);
		add_to_machineCodeList(machine_code, &data_list);
    }
	return true;
}

bool save_string(char *str){

	MachineCode *machine_code; 	

	trim_whitespace(str);
	
	/*string is enpty*/
	if(*str == '\0'){
		printf(COLOR_RED "Error in line %d: missing parameters.\n" COLOR_RESET, LINE);
		return false;
	}
	/*string is 1 char*/
	if(strlen(str) < 2){
		printf(COLOR_RED "Error in line %d: invalid string\n" COLOR_RESET, LINE);
		return false;
	}

	if ( *str != '"'){
		printf(COLOR_RED "Error in line %d: string has to start with \" char\n" COLOR_RESET, LINE);
		return false;
	}

	if (str[strlen(str)-1] != '"'){
		printf(COLOR_RED "Error in line %d: string has to end with \" char\n" COLOR_RESET, LINE);
		return false;
	}
	
	str++;

	while(*str != '"') {
		machine_code = newMachineCode(ascii_to_binary(*str), DC++);
        add_to_machineCodeList(machine_code, &data_list);
        str++; 
	}

	if(*str == '"') {
        machine_code = newMachineCode(ascii_to_binary('\0'), DC++);
        add_to_machineCodeList(machine_code, &data_list);
	}
	return true;
	
}


void update_data_list(MachineCodeNode *head){
	
	MachineCodeNode *ptr;
	ptr = head;
	
	while(ptr) {
		ptr->data.add += IC; /*add IC to the data address*/
		ptr=ptr->next;
	}
}


void update_label_list(labelNode *head){

	labelNode *ptr;
	ptr = head;

	while(ptr) {
		if(ptr->data.type == data){
			ptr->data.value+=IC; /*add IC to the label value*/ 
		}
		ptr=ptr->next;
	}
}

