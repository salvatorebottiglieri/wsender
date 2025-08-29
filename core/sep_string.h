#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct{
    size_t size;    
    char *data; 
}String;


String* new_s(const char* data, size_t size);
void delete_s(String* string);
String* get_slice(String* string, size_t start, size_t end);
bool equal(String* string1, String* string2);
size_t size_of_string(String* string);
String** tokenize(String* buffer);

#endif // STRING_H