#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "net.h"
#include "sep_string.h"
#include "sep_log.h"
#include "sep_ui.h"


const char *env;

#define USER_INPUT_BUFFER_SIZE 1000

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

String** get_command_and_arguments(String *user_input){
    String** tokens = tokenize(user_input);
    return tokens;
}

void clean_command_and_arguments(String** tokens){
    for (size_t i = 0; tokens[i] != 0; i++){delete_s(tokens[i]);}
    free(tokens);
}

void clean_buffer(char* buffer){memset(buffer, 0, USER_INPUT_BUFFER_SIZE);}


int use_case_send_message(Peer* peer, String* message){

    if (connect_to(peer) == -1){s_log(ERROR, "Failed to connect to peer: %s", peer->name) ;return -1;}

    if (send_to(peer, message->data, message->size) == -1){s_log(ERROR, "Failed to send message to peer: %s", peer->name); return -1;}
    

    return 0;
    
}



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
        String** result = get_command_and_arguments(user_input);
        String* command = result[0];
        String** arguments = &result[1];
        if (equal(command, exit)){
            printf("Exit command received\n");
            clean_buffer(buffer);
            delete_s(user_input);
            delete_s(command);
            break;
        }
        else if (equal(command, send)){
            String* message = get_slice(user_input, 5, user_input->size);

            use_case_send_message(&receiver, message);

            delete_s(message);
        }
        else if (equal(command,connect)){
            printf("Connecting to %s\n", arguments[0]->data);

            
            
            
        }
        else {
            printf("Invalid command\n");
        }

        clean_buffer(buffer);
        clean_command_and_arguments(result);


    }


    
    return 0;
}