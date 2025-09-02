#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "net.h"
#include "sep_string.h"
const char *env;

#define USER_INPUT_BUFFER_SIZE 256

String* read_user_input(char* buffer){
    char c;
    size_t index = 0;
    do{
        c = getchar();
        if (c != EOF){
            buffer[index++] = c;
        }

    }while(c != EOF && c != '\n' && index < USER_INPUT_BUFFER_SIZE);
    String *s = new_s(buffer, index);
    return s;
}

void print_info(){
    #if __STDC_VERSION__ >= 201710L
        printf("We are using C18!\n");
    #elif __STDC_VERSION__ >= 201112L
        printf("We are using C11!\n");
    #elif __STDC_VERSION__ >= 199901L
        printf("We are using C99!\n");
    #else
        printf("We are using C89/C90!\n");
    #endif
}

int get_port_number(char* argument){
    size_t index = 0;
    while(1){
        putchar(argument[index++]);
        sleep(2);
    }

}

String* get_command(String *buffer){

    size_t cursor = 0;

    size_t start_index = 0 , end_index = 0;
    while(isspace(buffer->data[cursor])){cursor++;};
    start_index = cursor;
    while(!isspace(buffer->data[cursor])){cursor++;};
    end_index = cursor;

    String *command = get_slice(buffer,start_index, end_index-start_index);

    return command;
    
}

String* get_command_arguments(String *command, String *user_input){
    size_t i = 0;
    String** tokens = tokenize(user_input);
    
    while(tokens[i] != 0){
        size_t start_index = 1;
        i++;
    }
    

        
}

void clean_buffer(char* buffer){memset(buffer, 0, USER_INPUT_BUFFER_SIZE);}

int main(int argc, char *argv[]) {
    print_info();
    env = getenv("ENV");
    if (env != NULL) {
        printf("ENV is set to: %s\n", env);
    }else{
        env = "DEBUG";
    }
    
    pthread_t thread;
    
    if (argc < 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }
    if (atoi(argv[1]) < 0 || atoi(argv[1]) > 65535){
        printf("Provide a valid port range\n");
        exit(0);
    }
    Peer receiver = {
        .name = "receiver",
        .id = "12345678",
        .ip = "127.0.0.1",
        .sockfd = -1,
        .port = 8081
    };
    ConnectionParams params = {
        .listen_port = atoi(argv[1])
    };
    int rc;
    rc = pthread_create(&thread, NULL, accept_connections, &params);
    printf("\n---------------------------------------\nAccepting connections on port: %d\n---------------------------------------\n", atoi(argv[1]));
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    char buffer[USER_INPUT_BUFFER_SIZE];
    String *exit = new_s("exit", 4);
    String *send = new_s("send", 4);
    String *connect = new_s("connect", 7);

    while(1){

        String* user_input = read_user_input(buffer);
        String *command = get_command(user_input);
        if (equal(command, exit)){
            printf("Exit command received\n");
            clean_buffer(buffer);
            delete_s(user_input);
            delete_s(command);
            break;
        }
        else if (equal(command, send)){
            String* message = get_slice(user_input, 5, user_input->size);
            printf("Sending message: %s\n", message->data);
            send_to(&receiver, message->data, message->size);
            delete_s(message);
        }
        else if (equal(command,connect)){
            get_command_arguments(command, user_input);
            
            
        }
        else {
            printf("Invalid command\n");
        }

        clean_buffer(buffer);
        delete_s(user_input);
        delete_s(command);
        

    }


    
    return 0;
}