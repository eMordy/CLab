#ifndef FIRSTPASS_H
#define FIRSTPASS_H

/*
 * Function: firstPass
 * ----------------------------
 * This function reads an assembly file and builds the labels table with their type
 * and encodes the instructions. If an error is found in the file syntax,
 * it will not pass to the next passes.
 *
 * file_name: the name of the input assembly file to be processed.
 *
 * return: true if the pass was successful and false otherwise.
 */
bool firstPass(char *file_name);

/*
 * Function: save_data
 * ----------------------------
 * This function extruct integers from a string of numbers devided by commas,
 * if the data is valid (has only one comma between integets, integers that can
 * be represented in 14 bits, and doesn't have a comma at the start or end) 
 * it converts the number to binary representation and saves it to the data list.
 *
 * str: pointer to the string containing the data to be saved.
 *
 * return: true if the data was successfully saved to data list ,false if the 
 * input data is invalid.
 */
bool save_data(char *str);

/*
 * Function: save_string
 * ----------------------------
 * This function converts each character in the string to its ASCII 
 * representation in binary representation and saves it to the data list.
 * string has to start and end with " character.
 *
 * str: a pointer to a character array containing the string to be saved.
 *
 * return true if the string was successfully saved, false otherwise.
 */
bool save_string(char *str);

/*
 * Function: update_data_list
 * ----------------------------
 * This function updates the addresses of the data in the machine code list
 * by adding the value of IC to each data address.
 *
 * head: pointer to the head of the machine code list.
 *
 * return void.
 */
void update_data_list(MachineCodeNode *head);

/*
 * Function: update_label_list
 * ----------------------------
 * This function updates the addresses of the data labels (.data\.string lines)
 * in the label list by adding the value of IC to each label address.
 *
 * head: pointer to the head of the label list.
 *
 * return void.
 */
void update_label_list(labelNode *head);

#endif 
