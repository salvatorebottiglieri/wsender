#include "sep_string.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
/**
 * Creates a new String object containing a copy of the given data.
 * For compatibility with the C standard library, the size of the data
 * comprises of the null terminator.
 * @param data The data to copy into the new String
 * @param size The size of the data, NOT include the null terminator
 * @return A new String object containing a copy of the given data
 */
String* new_s(const char* data, size_t size){
    String* str = malloc(sizeof(String)+(sizeof(char)*size)+1);
    str->data = (char*)str+sizeof(String);
    str->size = size;
    for (size_t i = 0; i < size; i++){
        str->data[i] = data[i];
    }
    str->data[size+1] = '\0';
    return str;  
}

void delete_s(String* string){
    free(string->data);
    free(string);
    string = NULL;
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
    if (start > end || start < 0){printf("Invalid start index\n"); return NULL;}

    String* new_string = (String*) malloc(sizeof(String)+sizeof(char)+(end-start+1));
    if (new_string == NULL){perror("malloc  failed");return NULL;}
    new_string->data = (char*) malloc(sizeof(char)*(end-start+1));
    if (new_string->data == NULL){perror("malloc  failed");return NULL;}

    memcpy(new_string->data, string->data+start, end-start); 
    new_string->data[end-start] = '\0';
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


String** tokenize(String* buffer){
    size_t start_index, end_index = 0;
    while(!isspace(buffer->data[end_index])){end_index++;}



}

/**
 * Calculates the size of the given String object in terms of C-style strings.
 * This is the size of the String object plus one, to account for the null
 * terminator.
 * @param string The String object to calculate the size of
 * @return the size of the given String object in terms of C-style strings
 */
size_t size_of_string(String* string){
    return string->size +1;
}