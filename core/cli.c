#include "cli.h"

#include <stdio.h>
#include <string.h>

typedef void (*func_ptr)(void*);

void file_handler(void* arg){
    printf("File handler called\n");
}

func_ptr handlers_factory(String *option){

    if (equal_const(option, "-f")){return &file_handler;}
    
}

int add(int a, int b){
    return a + b;
}

OptionHandler *init(int argc, char* argv[]){
    OptionHandler *op_handler = malloc(sizeof(OptionHandler));
    op_handler->option = new_s(argv[1], strlen(argv[1]));
    op_handler->arguments = malloc(sizeof(String*) * (argc-2));
    op_handler->arguments_count = argc-2;
    for (int i = 2; i < argc; i++){
        op_handler->arguments[i-2] = new_s(argv[i], strlen(argv[i]));
    }

    op_handler->handler = handlers_factory(op_handler->option);
    return op_handler;
}


