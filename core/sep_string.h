#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct{
    size_t size;    
    char *data;
    size_t ref_count;
}String;


String* new_s(const char* data, size_t size);

void delete_s(String* string);

String* get_slice(String* string, size_t start, size_t end);

bool equal(String* string1, String* string2);
bool equal_const(String* string1, const char* string2);

size_t size_s(String* string,bool include_terminator);

String** tokenize(String* buffer);

size_t find(String* string, String* sub_string);

int append(String* string1, String* string2);

String* concat(String* string1, String* string2);

#endif // STRING_H