#include "sep_string.h"
#include <stdio.h>

/**
 * Creates a new String object containing a copy of the given data.
 * @param data The data to copy into the new String
 * @param size The size of the data to copy
 * @return A new String object containing a copy of the given data
 */
String* new_s(const char* data, size_t size){
    String* str = malloc(sizeof(String)+sizeof(char)*size);
    str->data = (char*)str+sizeof(String);
    str->size = size;
    for (size_t i = 0; i < size; i++){
        str->data[i] = data[i];
    }
    return str;  
}

void delete_s(String* string){
    free(string);
}

/**
 * Returns a new String object that is a slice of the given String
 * from the specified start index (inclusive) to the end index (exclusive).
 * If the end index is greater than the size of the original string or
 * if the start index is greater than the end index, an error message
 * is printed and NULL is returned.
 *
 * It allocates memory for structure and data, inefficient but 
 * for the moment ensure memory safety by prevent free a previously freed memory
 * 
 * @param string The original String object
 * @param start The start index (inclusive) of the slice
 * @param end The end index (exclusive) of the slice
 * @return A new String object representing the slice, or NULL if indices are invalid
 */

String* get_slice(String* string, size_t start, size_t end){
    if (end > string->size){printf("Invalid end index\n"); return NULL;}
    if (start > end){printf("Invalid start index\n"); return NULL;}

    String* new_string = (String*) malloc(sizeof(String)+sizeof(char)*(end-start));
    if (new_string == NULL){perror("malloc failed");return NULL;}

    new_string->data = (char*)new_string+sizeof(String);
    for(size_t i = 0; i < end-start; i++){
        new_string->data[i] = string->data[start+i];
    }
    new_string->size = end-start;
    return new_string;
}

/**
 * Compares two String objects for equality.
 * @param string1 The first String object to compare
 * @param string2 The second String object to compare
 * @return true if the two String objects are equal, false otherwise
 */
bool equal(String* string1, String* string2){
    if (string1->size != string2->size){return false;}
    for (size_t i = 0; i < string1->size; i++){
        if (string1->data[i] != string2->data[i]){return false;}
    }
    return true;
}


/**
 * Converts the given String object into a C-style string.
 * The returned string is dynamically allocated and must be freed
 * by the caller.
 *
 * @param string The String object to convert
 * @return A dynamically allocated C-style string containing the same data
 */
char* to_c_string(String* string){
    char* c_string = malloc(sizeof(char)*string->size+1);
    if (c_string == NULL){perror("malloc failed");return NULL;}

    for (size_t i = 0; i < string->size; i++){
        c_string[i] = string->data[i];
    }
    c_string[string->size] = '\0';
    return c_string;
}