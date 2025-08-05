#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdbool.h>



typedef struct String_{
    char* data;
    size_t size;
    
}String;


String* new_s(const char* data, size_t size);
void delete_s(String* string);
String* get_slice(String* string, size_t start, size_t end);
bool equal(String* string1, String* string2);
char* to_c_string(String* string);

#endif