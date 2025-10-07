#ifndef FILE_H
#define FILE_H

#include "sep_string.h"

typedef struct{
    String *path;
    bool is_open;
    String* extension;
    int file_descriptor;
    char buffer[512];
}SFile;




SFile *s_open(const char *path);








#endif