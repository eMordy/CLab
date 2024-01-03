#ifndef UTILS_H
#define UTILS_H

#include "configurations.h"

/*
 * Function: createFileName
 * ----------------------------
 * This fucntion create an extanded file name and stores it in extended_file.
 *
 * file_name: a pointer to a char array containing the name of a file.
 * file_extention:  a pointer to a char array containing the extension of a file.
 * extended_file: a pointer to a char array that will be used to store the concatenated file name and extension.
 *
 * return void.
 */
void createFileName(char *file_name, char *file_extention, char *extended_file);

/*
 * Function: rescue_word
 * ----------------------------
 * Extract a word from a given line of text and store it in a buffer.
 * Skips leading white spaces, copies characters to buffer until a white space, 
 * a newline, or the end of the line is encountered,
 * adds null terminator to the end of the buffer.
 * line skips trailing white spaces.
 *
 * line: Pointer to a pointer to the line of text from which to extract a word.
 * buffer: Pointer to a buffer where the extracted word will be stored.
 * 
 * return void
 */
void rescue_word(char **line, char *buffer);

/*
 * Function: trim_whitespace
 * ----------------------------
 * This function trims whitespace from the beginning and end of a string.
 * 
 * str: a pointer to a char array.
 *
 * return: void
 */
void trim_whitespace(char *str);

/*
 * Function: is_data_valid
 * ----------------------------
 * This function checks if a string containing comma-separated integers is valid.
 * a valid integer is a number with an optional sign of +/- every number devided by one comma.
 * comma at the start , at the end, multiple commas or missing commas are not valid.
 *
 * str: takes a pointer to a char array
 *
 *return: true if the string is valid, else return false.
 */
bool is_data_valid(char *str);

/*
 * Function: extract_token
 * ----------------------------
 * This function extracts a token from a string using a given delimiter. 
 * the extracted token is stored in a buffer, and the original string pointer is 
 * updated to point to the character after the delimiter or the end of the string. 
 *
 * str_ptr: a pointer to a pointer of char array.
 * buffer: a character array to store the extracted token.
 * delimiter: the character used to split the string.
 *
 * returns: void.
 */
void extract_token(char **str_ptr, char *buffer, char delimiter);

/*
 * Function: int_to_binary
 * ----------------------------
 * This function takes an integer value and converts it to a binary string of the specified length.
 * 
 * value: the integer value to be converted to binary.
 * len: the length of the binary string to be generated.
 *
 * return: a pointer to character array containing the binary string.
 */
char* int_to_binary(int value, int len);

/*
 * Function: int_str_to_binary
 * ----------------------------
 * This function takes a string representation of an integer value and converts it to a binary string of the
 * specified length. If the input number does not fit within the specified number of bits, the function returns NULL.
 *
 * decimal: a string representation of the integer value to be converted to binary.
 * len: the length of the binary string to be generated.
 *
 * return: a pointer to the character array containing the binary string.
 */
char* int_str_to_binary(char *decimal, int len);

/*
 * Function: ascii_to_binary
 * ----------------------------
 * This function converts an ASCII character into its binary representation 
 * in a spesific length determined by WORD_LEN using a character array for the
 * binary string.
 * 
 * ch: the ASCII character to be converted.
 * 
 * Return: a pointer to a character array containing the binary representation
 * of the input character.
 */
char* ascii_to_binary(char ch);

/*
 * Function: is_number
 * ----------------------------
 * This function takes a string as input and returns true if the string 
 * represents a valid integer number, otherwise it returns false. 
 * A valid integer number must start with '#' char, can be followed by an 
 * optional sign '+' or '-', and must be followed by digits. 
 *
 * num: a pointer to a string representing the number to be checked.
 *
 * return: true if the string represents a valid integer number. false otherwise.
 */
bool is_number(char *num);

#endif 
