#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "configurations.h"

void rescue_word(char **line, char *buffer){
    
    int i=0;
	
	/*Skip leading white spaces*/
    while (isspace(**line)){ 
        ++*line;
    }

    while (i < MAX_LINE_LEN && **line != '\0' && **line != ' ' && **line != '\t' && **line != '\n') {

		/* Copy characters from the line pointer to buffer and promote line and buffer pointers*/
		*buffer++ = *(*line)++;
        i++;
    }

    *buffer = '\0'; /*add a null terminator to the end of the buffer */

    while (isspace(**line)){ /*Skip trailing white spaces*/
        ++*line;
    }
}

void createFileName(char *file_name, char *file_extention, char *extended_file){
    
    strcpy(extended_file,file_name);
    strcat(extended_file,file_extention);

}

void trim_whitespace(char *str) {
    
	/*set start and end pointers to the beginning and end of the string*/ 
	char *start = str;
    char *end = str + strlen(str) - 1;

    /*move the start pointer forward until it points to the first non-whitespace character*/
    while (isspace(*start)) {
        start++;
    }

    /*move the end pointer backward until it points to the last non-whitespace character*/
    while (end > start && isspace(*end)) {
        end--;
    }

    /*add null terminator to the end of the trimmed string*/
    *(end + 1) = '\0';

    /*move the trimmed string to the beginning of the original string*/
    memmove(str, start, strlen(start) + 1);
}

bool is_data_valid(char *str) { 
    
    bool valid = true; /*assume the data is valid until an error is found*/
    int len = strlen(str);
    bool has_number = false; /*flag to check if a number is present*/
    bool has_comma = true; /*Flag to check if a comma is present*/
    int i;

    /*check if the data ends with a comma*/
    for (i = len - 2; i >= 0; i--) {
        char ch = str[i];
        if (ch == ' ' || ch == '\t') { 
            continue; /*ignore whitespace characters*/
        }
        else if(ch != ','){
            break; /*exit loop if a non-comma character is found*/
        }
        else{ /*last char is a comma*/
            printf(COLOR_RED "Error in line %d: data ends with a comma.\n" COLOR_RESET, LINE);
            valid = false;
        }
    }
    
    /*ignore leading whitespace characters*/
    while (*str == ' ' || *str == '\t') {
		(str)++;
	}
	
    /*check if the input string is empty*/
    if (*str == '\0') {
        printf(COLOR_RED "Error in line %d: missing parameters.\n" COLOR_RESET, LINE);
        valid = false;
    }
    
    /*check if the first character is a comma*/
    if (*str == ',') {
        printf(COLOR_RED "Error in line %d: data starts with a comma.\n" COLOR_RESET, LINE);
        str++;
        valid = false;
    }
    
    while (*str != '\0') {

        /*check for space or tab characters*/   
        if (isspace(*str)) {    
            str++;
            continue;/*ignore whitespaces*/
        }

        /*check for sign +/-*/
        if (*str == '+' || *str == '-') {     
            if (has_number) {
                printf(COLOR_RED "Error in line %d: missing comma.\n" COLOR_RESET, LINE);
                valid = false;
            }

            has_comma = false;
            str++;
            continue;
        }
        /* check for comma */
        else if (*str == ',') {  
            if(has_comma){
                printf(COLOR_RED "Error in line %d: multiple commas.\n" COLOR_RESET, LINE);
                valid = false;
            }
            has_comma = true;
            has_number = false;
            str++;
        }
        /* Check for digits*/
        else if (isdigit(*str)) {

            /*check if a comma is missing before the current digit*/
            if (!has_comma && (*(str-1) == ' ' || *(str-1) == '\t')){
                printf(COLOR_RED "Error in line %d: missing comma.\n" COLOR_RESET, LINE);
                valid = false;
            }
			
            has_comma = false;
            has_number = true;
            str++;
        }
        
        
        /*if anything else is encountered, the data is invalid*/
        else {
            printf(COLOR_RED "Error in line %d: data contains an invalid integers.\n" COLOR_RESET, LINE);
            has_comma = false;
            valid = false;
            str++;
        }
    } 
    return valid;
}

void extract_token(char **str_ptr, char *buffer, char delimiter) {
    
	/*find the first occurrence of the delimiter in the string*/
    char *delimiter_ptr = strchr(*str_ptr, delimiter);
    
	/*if the delimiter is not found, copy the entire string into the buffer*/
    if (delimiter_ptr == NULL) {
        
        strcpy(buffer, *str_ptr);
        *str_ptr += strlen(*str_ptr); 
        
    } else {
        
        unsigned int buffer_len = delimiter_ptr - *str_ptr; /*Calculate the length of the token*/
        strncpy(buffer, *str_ptr, buffer_len); /*copy the token into the buffer*/
        buffer[buffer_len] = '\0';
        *str_ptr = delimiter_ptr + 1; /*update the pointer to the start of the next token*/
    }
}

char* int_to_binary(int value, int len){
	
	int i;
    char *binary = calloc(WORD_LEN, sizeof(char));

    if (!binary) {
        printf(COLOR_RED "Error: Failed to allocate memory\n" COLOR_RESET);
        return NULL;
    }

    binary[len] = '\0';

    for (i = len - 1; i >= 0; i--) {
        binary[i] = (value & 1) ? '1' : '0'; /*set the [i] bit in the binary string ot 0\1*/
        value >>= 1;
    }

    return binary;
}

char* int_str_to_binary(char *decimal, int len) {

    int carry, sign, i;
    char *binary;
    long num = strtol(decimal, NULL, 10); /*convert the input string to a long integer*/
    long max, min;

	/*calculate the maximum and minimum values that can be represented by the specified number of bits*/
	max = (1L << len) - 1;
	min = -(1L << len);	

	/*check if the input number fits within the specified number of bits*/
    if (num > max || num < min) {
        printf(COLOR_RED "Error in line %d: Number %s does not fit within %d bits\n" COLOR_RESET, LINE, decimal, len);
        return NULL;
    }

	/*allocate memory for the binary string*/
    binary = malloc(WORD_LEN);

    if (!binary) {
        printf(COLOR_RED "Error: Failed to allocate memory\n" COLOR_RESET);
        return NULL;
    }

    sign = 1;
    if (num < 0) {/*handle negative numbers*/
        sign = -1;
        num = -num;
    }

    for (i = len - 1; i >= 0; i--) {
        binary[i] = (num % 2) + '0'; /*convert the [i] bit to a character and store it in the binary string*/
        num /= 2;
    }

    if (sign == -1) { /* 2's complement handling*/
        for (i = len - 1; i >= 0; i--) {
            binary[i] = binary[i] == '0' ? '1' : '0';
        }

        carry = 1;
        for (i = len - 1; i >= 0 && carry; i--) {
            binary[i] = binary[i] == '0' ? '1' : '0';
            carry = binary[i] == '0';
        }
    }

    binary[WORD_LEN - 1] = '\0';

    for (i = len; i < WORD_LEN - 1; i++) { /* trailing zeros */
        binary[i] = '0';
    }

    return binary;
}

char* ascii_to_binary(char ch){

	int i;
	static char binary[WORD_LEN];

	for(i=0; i<WORD_LEN-1; i++){
		binary[i] = ((ch >> (WORD_LEN-2-i)) & 1) + '0';
	}

	binary[WORD_LEN-1] = '\0';
	return binary;
}

bool is_number(char *num) {
    
    int len, i;
    
    if (num[0] != '#') {  /*check if the string starts with the '#' sign*/ 
        return false;
    }
    num++;  /*move the pointer to the next character*/
    if (num[0] == '+' || num[0] == '-') {  /*check if the string starts with an optional sign*/
        num++;  /*move the pointer to the next character*/
    }
    len = strlen(num);
    for (i = 0; i < len; i++) {
        if (!isdigit(num[i])) {  /*check if any non-digit characters exist*/
            return false;
        }
    }
    return true;
}
