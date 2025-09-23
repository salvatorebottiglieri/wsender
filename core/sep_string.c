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
    String* str = malloc(sizeof(String));
    if (str == NULL) return NULL;
    str->data = malloc(size+1);
    if (str->data == NULL) {free(str); return NULL;}
    str->size = size;
    for (size_t i = 0; i < size; i++){
        str->data[i] = data[i];
    }
    str->data[size] = '\0';
    str->ref_count = 1;
    return str;  
}

void delete_s(String* string){
    if (string == NULL) {return;}
    if (string->ref_count > 1) {string->ref_count--;return;}

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
    if (start >= end || start < 0){printf("Invalid start index\n"); return NULL;}
    String* new_string = NULL;

    new_string = (String*) malloc(sizeof(String)+sizeof(char)+(end-start+1));
    if (new_string == NULL){perror("malloc  failed");return NULL;}
    new_string->data = (char*) malloc(sizeof(char)*(end-start+1));
    if (new_string->data == NULL){perror("malloc  failed");return NULL;}

    memcpy(new_string->data, string->data+start, end-start); 
    new_string->data[end] = '\0';
    new_string->size = end-start;
    new_string->ref_count = 1;
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
 * Tokenizes a given String object by splitting it into multiple substrings
 * separated by whitespace characters.
 * We declare an array of pointers to strings with the size of the buffer:
 * since we don't know the number of tokens that we will find but we know that in the
 * worst case we will find the same number of tokens as the number of characters, so we
 * allocate on the stack a buffer that can contain all the possible tokens and, at the end, 
 * on the heap only the tokens that we have found. At the end of the function the memory
 * occupied by the stack buffer will be freed automatically.
 *
 * @param buffer The String object to tokenize
 * @return An array of String objects representing the individual tokens
 */
String** tokenize(String* buffer){
    String** possible_tokens[buffer->size];
    size_t i =0;
    size_t start_token = 0, end_token = 0;
    while(buffer->data[end_token] != '\n'){
        while(isspace(buffer->data[start_token]) && buffer->data[start_token] != '\0'){start_token++;}
        end_token = start_token;
        while(!isspace(buffer->data[end_token]) && buffer->data[end_token] != '\0'){end_token++;}
        if (end_token == start_token){break;}
        
        possible_tokens[i] = get_slice(buffer, start_token, end_token);
        if (possible_tokens[i] == NULL){for (size_t j = 0; j < i; j++){delete_s(possible_tokens[j]);}return NULL;}
        i++;
        start_token = end_token;

    }

    /* Allocate the memory needed for the number of tokens found plus one for a terminator
        byte that will inform the caller where the list of tokens ends*/
    String ** tokens = malloc(sizeof(String*)*i+1);
    memcpy(tokens, possible_tokens, sizeof(String*)*(i));
    tokens[sizeof(String*)*(i)+1] = 0;

    return tokens;
}


/**
 * Returns the size of the given String object.
 * If include_terminator is set to true, the size of the null terminator is also included.
 * @param string The String object to get the size of
 * @param include_terminator Whether to include the null terminator in the size
 * @return The size of the given String object
 */
size_t size_of_string(String* string,bool include_terminator){
    if (include_terminator){return string->size+1;}
    return string->size;
}