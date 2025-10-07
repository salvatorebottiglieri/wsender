#ifndef CLI_H
#define CLI_H

#include "sep_string.h"


typedef struct {
    String *option;
    String **arguments;
    int arguments_count;
    void (*handler)(void*);
}OptionHandler;

OptionHandler *init(int argc, char* argv[]);




#endif