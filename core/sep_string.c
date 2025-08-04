#include "sep_string.h"
#include <stdbool.h>

String* new_s(const char* data, size_t size){
    String* str = malloc(sizeof(String)+sizeof(char)*size);
    str->data = (char*)str+sizeof(String);
    str->size = size;
    memcpy(str->data, data, size);
    return str;  
}

void delete_s(String* string){
    free(string);
}

String* get_slice(String* string, size_t start, size_t end){
    if (end > string->size){printf("Invalid end index\n"); return NULL;}
    if (start > end){printf("Invalid start index\n"); return NULL;}

    String* new_string = (String*) malloc(sizeof(String));
    new_string->data = string->data+start;
    new_string->size = end-start;
    return new_string;
}

bool equal(String* string1, String* string2){
    if (string1->size != string2->size){return false;}
    return memcmp(string1->data, string2->data, string1->size) == 0;
}