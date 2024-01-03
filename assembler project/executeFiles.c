#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "passesUtils.h"
#include "configurations.h"
#include "utils.h" 
#include "executeFiles.h"

bool execute_ob(char *file_name){

	FILE *ob_file;
	char *obFileName, *machine_str;
	MachineCodeNode *list_ptr;

	obFileName = calloc(strlen(file_name)+4 , sizeof(char));/*for .ob\0 */
	
	if(!obFileName) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}

	createFileName(file_name, ".ob", obFileName);
    ob_file = fopen(obFileName, "w"); 
    if (!ob_file) { 
        printf(COLOR_RED "Error: The program has failed to open \"%s.ob\" file\n" COLOR_RESET, obFileName);
        return false;
    }
	
	fprintf(ob_file, "%d %d", IC,DC);
	
	/*points on startaderss*/
	list_ptr = inst_list;
	list_ptr = list_ptr->next;
	while (list_ptr->data.add != START_ADDRESS){
		list_ptr = list_ptr->next;
	}

	/*prints instructions to file*/
	while(list_ptr) {
		machine_str = replaceZerosOnes(list_ptr->data.code);
		fprintf(ob_file, "\n0%d	%s",list_ptr->data.add, machine_str);
		list_ptr = list_ptr->next;
	}

	/*prints data to file*/
	list_ptr = data_list;
	while(list_ptr) {
		machine_str = replaceZerosOnes(list_ptr->data.code);
		fprintf(ob_file, "\n0%d	%s",list_ptr->data.add, machine_str);
		list_ptr = list_ptr->next;
	}

	free(machine_str);
	free(obFileName);
	free_data_list(data_list);
	free_data_list(inst_list);
	fclose(ob_file);
	return true;

}

bool execute_ent(char *file_name){
	
	FILE *ent_file;
	char *entFileName;
	labelNode *list_ptr;
	bool has_external_labels = false; /*if there is no external labels for .ext execution- free label_list*/

	entFileName = calloc(strlen(file_name)+4 , sizeof(char));/*for .ent\0 */
	
	if(!entFileName) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}

	createFileName(file_name, ".ent", entFileName);
    ent_file = fopen(entFileName, "w"); 
    if (!ent_file) { 
        printf(COLOR_RED "Error: The program has failed to open \"%s.ob\" file\n" COLOR_RESET, entFileName);
        return false;
    }

	list_ptr = label_list;

	/*iterates through the label list and writes the entries to the .ent file.*/
	while(list_ptr){

		/*free label_list if no externals in file*/
		if(list_ptr->data.type == external){
			has_external_labels = true;
		}

		if(list_ptr->data.type == entry){
			fprintf(ent_file,"%-30s %d\n",list_ptr->data.name,list_ptr->data.value);
		}
		list_ptr = list_ptr->next;
	}

	if(!has_external_labels){
		free_labelList(label_list); 
	}
	free(entFileName);
	fclose(ent_file);

	return true;

}

bool execute_ext(char *file_name){
	
	FILE *ext_file;
	char *extFileName;
	labelNode *list_ptr;

	extFileName = calloc(strlen(file_name)+4 , sizeof(char));/*for .ext\0 */
	
	if(!extFileName) {
		printf(COLOR_RED "Memory allocation error\n" COLOR_RESET);
		return false;
	}

	createFileName(file_name, ".ext", extFileName);
    ext_file = fopen(extFileName, "w"); 
    if (!ext_file) { 
        printf(COLOR_RED "Error: The program has failed to open \"%s.ob\" file\n" COLOR_RESET, extFileName);
        return false;
    }

	list_ptr = label_list;

	/*iterates through the label list and writes the entries to the .ent file.*/
	while(list_ptr){
		
		if(list_ptr->data.type == external && list_ptr->data.value != 0){
			fprintf(ext_file,"%-30s %d\n",list_ptr->data.name,list_ptr->data.value);
		}
		list_ptr = list_ptr->next;
	}

	free_labelList(label_list);  
	free(extFileName);
	fclose(ext_file);

	return true;
}

char* replaceZerosOnes(char *binaryStr) {
    
	char *machine_str = calloc(WORD_LEN, sizeof(char));
	int i;

    for (i = 0; i < WORD_LEN - 1; i++) {
        if (binaryStr[i] == '0') {
            machine_str[i] = '.';
        } else if (binaryStr[i] == '1') {
            machine_str[i] = '/';
        }
    }
	return machine_str;
}
