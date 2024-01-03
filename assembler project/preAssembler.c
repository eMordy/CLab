#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "preAssembler.h"

bool preAssembler(char *file_name){
    
    FILE *as_file, *am_file;
    char *asFileName, *amFileName;
    char *line; /*store a sentence from fgets*/
	char *line_ptr; /*tracks the line*/
	char *word_in_line; /*holds the current word from line*/
    macro *mcr;
    macroNode *macro_list = NULL;
    bool isMCR;
    
    /*define strings*/
    asFileName = calloc(strlen(file_name)+4 , sizeof(char)); /*for .as\0 */
    amFileName = calloc(strlen(file_name)+4 , sizeof(char)); /*for .am\0 */
    line = calloc(MAX_LINE_LEN + 1 , sizeof(char)); /*1 to check if the line is to long*/
    word_in_line = calloc(MAX_LINE_LEN + 1 , sizeof(char));
	
    if(!asFileName || !amFileName || !line || !word_in_line) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}

    /*open .as*/
    createFileName(file_name, ".as", asFileName); /*create a file name string with an extention*/
    as_file = fopen(asFileName, "r"); /*open the file with .as extention*/
    if (!as_file) { /*if unable to open*/
        printf("ERROR: %s file is not found.\n", asFileName);
        return false;
    }
        
    /*create+open .am*/
    createFileName(file_name, ".am", amFileName);
    am_file = fopen(amFileName, "w"); /*create a new file with .am extention*/
    if (!am_file) { 
        printf(COLOR_RED "ERROR: problem occured while creating .am file\n" COLOR_RESET);
        return false;
    }
    
    
    while(fgets(line, MAX_LINE_LEN, as_file) != NULL) {
        
        isMCR = false;
        line_ptr = line;
        rescue_word(&line_ptr, word_in_line); /*stores first word from line into word_in_line*/
		mcr = find_macro(word_in_line, macro_list); /*NULL if not exist*/
        
        /*if its a macro name(mcr is not NULL after searching it in macro_list)*/
    	if(mcr) {
			fprintf(am_file, "%s", mcr->value);
			continue;
		}
		
		/*if its a new macro defenition*/
		if(strcmp(word_in_line, "mcr") == 0){

            isMCR = true;
            rescue_word(&line_ptr, word_in_line); /*stores the second word in line into word_in_line buffer */
            mcr = new_macro(word_in_line); /*creats a new macro*/
            
            while(fgets(line, MAX_LINE_LEN, as_file) != NULL && isMCR){

                line_ptr = line;
                rescue_word(&line_ptr, word_in_line);
		
				if(strcmp(word_in_line, "endmcr") == 0){
                    isMCR = false;
                    add_macro_to_macroList(mcr, &macro_list);
                    break;
                }
                
                else{ /*adding data to the mcr vaule*/
                    mcr->value = realloc(mcr->value, strlen(mcr->value) + strlen(line) + 1); 
                    strcat(mcr->value, line);
                } 
            } /*end of intern while*/
		}

		/*if its not a macro definition or a macro name*/
		else {
			fprintf(am_file, "%s", line);
		}
    } /*end of while*/
    
    /*free all allocated memory*/
    free(asFileName);
    free(amFileName);
    free(line);
    free(word_in_line);
    free_macroList(macro_list);

    fclose(as_file);
    fclose(am_file);

    return true; 

} /*end of preAssembler*/


macro* new_macro(char *name){

    macro* mcr = malloc(sizeof(macro));
    mcr->name = malloc(strlen(name)+1);
    strcpy(mcr->name, name);
    mcr->value = calloc(1, 1);
    
    return mcr;
}

void add_macro_to_macroList(macro *mcr, macroNode **head){
    
    macroNode *new_node;
    new_node = malloc(sizeof(macroNode));
    
    if (!new_node) {
		printf("Memory allocation error\n");
		return;
	}
	
	new_node->data = *mcr; /*add the macro to the new macroNode*/
	new_node->next = NULL;
	
	if(*head == NULL){ /*if the macro list is empty, set the head pointer to the new node*/
	    *head = new_node; 
	}else{ /*if list is not empty*/
        macroNode *curr;
		curr = *head;
		while (curr->next){ /*iterate through the list until the last node is found*/
			curr = curr->next;
		}
		curr->next = new_node; /*add macroNode to the end of the list*/
	}
}

macro* find_macro(char *name, macroNode *head) { 

    macroNode *current;
    current = head;
    
    while (current != NULL) { /*iterate through the list*/

        if (strcmp(current->data.name, name) == 0){ /*compare the name of the current macro to the given name*/
            return &(current->data); /*if the names match, return a pointer to the macro structure*/
        } 
        current = current->next;
    }
    return NULL; /*if the macro is not found in the list*/
}

void free_macroList(macroNode *head) {
    macroNode *next;
    while (head != NULL) {
        next = head->next;
        free_macro(&head->data);
        free(head);
        head = next;
    }
}

void free_macro(macro *macro) {
    free(macro->name);
    free(macro->value);
}

