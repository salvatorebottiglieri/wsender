#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


#include "net.h"
#include "sep_string.h"
const char *env;

String* read_user_input(char* buffer){
    char c;
    size_t index = 0;
    do{
        c = getchar();
        if (c != EOF && c != '\n'){
            buffer[index++] = c;
        }

    }while(c != EOF && c != '\n');
    String *s = new_s(buffer, index);
    printf("User input: %s\n", to_c_string(s));
    return s;
}



int main(int argc, char *argv[]) {
    
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
    char *buffer = malloc(256);
    if (buffer == NULL){
        printf("Memory allocation failed\n");
        exit(-1);
    }

    String *exit = new_s("exit", 4);
    String *send = new_s("send", 4);

    while(1){

        String* user_input = read_user_input(buffer);
        String* command = get_slice(user_input, 0, 4);
        if (equal(command, exit)){
            printf("Exit command received\n");
            break;
        }
        if (equal(command, send)){
            char* message = buffer + 5;
            printf("Sending message: %s\n", message);
            send_to(&receiver, message);
        }

    }


    
    return 0;
}