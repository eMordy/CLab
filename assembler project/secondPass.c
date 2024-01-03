#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "passesUtils.h"
#include "configurations.h"
#include "utils.h" 

bool secondPass(char *file_name){

	FILE *am_file;
    char *amFileName, *line, *line_ptr, *word_in_line; 
    Label *label;
    int commend_Code;

	isSecondPass = true;
	IC=0, LINE=0;

	amFileName = calloc(strlen(file_name)+4 , sizeof(char));/*for .am\0 */
    line = calloc(MAX_LINE_LEN + 2 , sizeof(char));/*2 to check if the line is too long*/
    word_in_line = calloc(MAX_LINE_LEN + 1 , sizeof(char));

	if(!amFileName || !line || !word_in_line) {
		printf("Memory allocation error\n");
		return false;
	}

	/*opening .am file*/
	createFileName(file_name, ".am", amFileName);
    am_file = fopen(amFileName, "r"); 
    if (!am_file) { 
        printf("Error: The program has failed to open \"%s.am\" file\n", amFileName);
        return false;
    }

	while(fgets(line, MAX_LINE_LEN+2, am_file) != NULL) {

		LINE++;
		line_ptr = line;
		rescue_word(&line_ptr, word_in_line);

		/*empty line or comment line*/
    	if(*word_in_line == '\0' || *word_in_line == ';'){
			continue;
    	}

		/*check if the first word in line is a label*/
    	if(is_label(word_in_line)){
        	rescue_word(&line_ptr, word_in_line); /*move to the next word*/
    	}
	
		/*if .extern .data .string - all handles in first pass*/
		if(is_data(word_in_line) || is_string(word_in_line) || is_extern(word_in_line)){
			continue; /*done in first pass*/
		}

		/*.entry handling*/
		if(is_entry(word_in_line)){

			/*change type for code to entry */
			rescue_word(&line_ptr, word_in_line);
			label = find_label(word_in_line, label_list);
			if(label){
				label->type = entry;
			}else{ /*label not defined in first pass*/
				printf("Error in line %d: entry label has not defined in file %s.\n", LINE, file_name);
				error = true;
			}
			continue;
		}

		/*its an insrtruction line*/
		commend_Code = find_commend_code(word_in_line, COMMANDS);
		if (!save_opcode(commend_Code, line_ptr)){
			error = true;
			continue;
		}
	}

	free(amFileName);
	free(line);
	free(word_in_line);
    
    fclose(am_file);

	/*if error-free all allocated mamory*/
    if (error == true){
		printf("Errors found in \"%s\" file. You shall not pass to execute files pass!\n", file_name);
        free_labelList(label_list);
        free_data_list(data_list);
		free_data_list(inst_list);
		
        return false;
    }

	return true;
} 

