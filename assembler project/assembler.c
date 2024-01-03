#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "executeFiles.h"
#include "configurations.h"

/*

Welcome to ASSEMBLER!
Authors- Eden Cohen, Einav Mordok.

in this project we created an imaginary assembler to machine code compiler.
this assembler is composed  of three files:
preAssembler ,firstPassc and secondPass. 
then it creates files after the there's an input validation. 
in case of errors, they're printed to the terminal, and do not generate any files.

*/

int main(int argc, char *argv[]){

    char *file_name;
    int i;
	labelNode *list_ptr;
    
    /*if user didnt enter any files to the program*/
    if (argc == 1) {
        printf("ERROR: no files inserted in commend line.\n");
    }

	printf("\n");
	printf(COLOR_RED "┬ ┬┌─┐┬  ┌─┐┌─┐┌┬┐┌─┐  ┌┬┐┌─┐  ┌─┐┌─┐┌─┐┌─┐┌┬┐┌┐ ┬  ┌─┐┬─┐┬\n" COLOR_RESET);
	printf(COLOR_RED "│││├┤ │  │  │ ││││├┤    │ │ │  ├─┤└─┐└─┐├┤ │││├┴┐│  ├┤ ├┬┘│\n" COLOR_RESET);
	printf(COLOR_RED "└┴┘└─┘┴─┘└─┘└─┘┴ ┴└─┘   ┴ └─┘  ┴ ┴└─┘└─┘└─┘┴ ┴└─┘┴─┘└─┘┴└─o\n" COLOR_RESET);

    for (i=1; i<argc ; i++){
        
        file_name = argv[i];

        if(preAssembler(file_name)){

	    	if(firstPass(file_name)){

				if(secondPass(file_name)){
					/*ob*/
					execute_ob(file_name);

					/*ent*/
					list_ptr = label_list;
					while(list_ptr){

						if(list_ptr->data.type == entry){
							execute_ent(file_name);
							break;
						}else{
							list_ptr = list_ptr->next;
						}
					}

					/*ext*/
					list_ptr = label_list;
					while(list_ptr){
						if (list_ptr->data.type == external){
							execute_ext(file_name);
							break;
						}else{
							list_ptr = list_ptr->next;
						}
					}

					printf(COLOR_PURPLE "%s File successfuly done!\n" COLOR_RESET, file_name);
				}
	    	}
		}  
    }
    printf("\n");
    return 0;
}

