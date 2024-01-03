#ifndef SECONDPASS_H
#define SECONDPASS_H

/*
 * Function: secondPass
 * ----------------------------
 * This function reads an assembly file and complete all the missing machine code
 * and label adresses to the lists.
 * validating all the labels are defined in file.
 *
 * file_name: the name of the input assembly file to be processed.
 *
 * return: true if the pass was successful and false otherwise.
 */
bool secondPass(char *file_name);

#endif 
