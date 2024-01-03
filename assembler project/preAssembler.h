#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "configurations.h"

/*
 * Function: preAssembler
 * ----------------------------
 * This function takes a string representing a file name, opens the file,
 * and writes the copied content with the expanded macros to a new file with 
 * .am extension. 
 * function creates a new file even if there are no macros defined in the file.
 *
 * file_name: the file name to open.
 *
 * return: true if the process succeeded, and false if problem occurred opening files.
 */
bool preAssembler(char *file_name);

/*
 * Function: new_macro
 * ----------------------------
 * Function creates a new macro. macro name define by the parameter and
 * the macro value is a temporary space allocation.
 *
 * name: the name of the new macro.
 * 
 * return: a pointer to the macro.
 */
macro* new_macro(char *name);

/*
 * Function: add_macro_to_macroList
 * ----------------------------
 * This function adds a new macroNode to a linked list of macros.
 * it adds the new node to the end of the list as the last node.
 *
 * mcr: a pointer to a macro structure that contains the macro to be added to the list.
 * head: a pointer to the head of the macro list.
 * 
 * return void.
 */
void add_macro_to_macroList(macro *mcr, macroNode **head);

/*
 * Function Name: find_macro
 * ----------------------------
 * This function searches a macro in a list of macros a with the given name.
 *
 * name: a pointer to a string containing the name of the macro to search for.
 * head: a pointer to the head of the macro list to search.
 *
 * Returns: a pointer to the macro structure if a macro with the given name is
 * found in the list, or NULL otherwise.
 */
macro* find_macro(char *name, macroNode *head);

/*
 * Function Name: free_macroList
 * ----------------------------
 * This function frees the memory allocated to a linked list of macros,
 * including the macro structures themselves.
 *
 * head: a pointer to the head of the macro list to be freed.
 *
 * Returns void.
 */
void free_macroList(macroNode *head);

/*
 * Function Name: free_macro
 * ----------------------------
 * This function frees the memory allocated to a macro structure, 
 * including the memory allocated to its name and value strings.
 *
 * macro: a pointer to the macro structure to be freed.
 *
 * Returns void.
 */
void free_macro(macro *macro);

#endif 
