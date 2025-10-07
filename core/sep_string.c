#include "sep_string.h"
#include "sep_log.h"

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
    if (end > string->size){s_log(ERROR,"Invalid end index\n"); return NULL;}
    if (start >= end || start < 0){s_log(ERROR,"Invalid start index\n"); return NULL;}
    String* new_string = NULL;
    size_t new_string_size = end-start;

    new_string = (String*) malloc(sizeof(String));
    if (new_string == NULL){s_log(ERROR,"malloc  failed");return NULL;}
    new_string->data = (char*) malloc(sizeof(char)*(new_string_size+1));
    if (new_string->data == NULL){s_log(ERROR,"malloc  failed");return NULL;}

    memcpy(new_string->data, string->data+start, end-start); 
    new_string->data[new_string_size] = '\0';
    new_string->size = new_string_size;
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
    tokens[i] = 0;

    return tokens;
}


/**
 * Returns the size of the given String object.
 * If include_terminator is set to true, the size of the null terminator is also included.
 * @param string The String object to get the size of
 * @param include_terminator Whether to include the null terminator in the size
 * @return The size of the given String object
 */
size_t size_s(String* string,bool include_terminator){
    if (include_terminator){return string->size+1;}
    return string->size;
}


/**
 * Finds the first occurrence of the given sub_string in the given string.
 * If the sub-string is not found, -1 is returned.
 * @param string The string to search
 * @param sub_string The sub-string to search for
 * @return The index of the first occurrence of the sub-string in the string, or -1 if not found
 */
size_t find(String* string, String* sub_string){
    size_t string_index = 0;
    size_t sub_string_index = 0;
    while (string_index < string->size){
        if (string->data[string_index] == sub_string->data[sub_string_index]){
            sub_string_index++;
            if (sub_string_index == sub_string->size-1){
                return string_index-sub_string->size;
            }
        }
        else{
            sub_string_index = 0;
        }
        string_index++;
    }
    return -1;
}



/**
 * Appends the data of the second string to the first string.
 * It allocates memory for the new string and copies the data from the second string.
 * If the reallocation fails, an error message is printed and -1 is returned.
 * @param string1 The first string object to append
 * @param string2 The second string object to append
 * @return 0 if successful, -1 if the reallocation failed
 */
int append(String* string1, String* string2){

    if (realloc(string1->data, string1->size+string2->size) == NULL){
        s_log(ERROR,"realloc failed");
        return -1;
    }

    // copy data from string2 to string1, including the null terminator
    memcpy(string1->data+string1->size, string2->data, string2->size+1);
    string1->size += string2->size;
    return 0;
}


/**
 * Compares two String objects for equality.
 * @param string1 The first String object to compare
 * @param string2 The second String object to compare
 * @return true if the two String objects are equal, false otherwise
 */
bool equal_const(String* string1, const char* string2){
    if (string1->size != strlen(string2)){return false;}
    for (size_t i = 0; i < string1->size; i++){
        if (string1->data[i] != string2[i]){return false;}
    }
    return true;
}



/**
 * Concatenates two String objects and returns a new String object containing the concatenated data.
 * It allocates memory for the new string and copies the data from the first string.
 * It then appends the data from the second string to the new string.
 * If the reallocation fails, an error message is printed and NULL is returned.
 * @param string1 The first String object to concatenate
 * @param string2 The second String object to concatenate
 * @return A new String object containing the concatenated data, or NULL if the reallocation failed
 */
String* concat(String* string1, String* string2){

    String *result = new_s(string1->data, string1->size);
    append(result, string2);
    return result;
}