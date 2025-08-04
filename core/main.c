#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


#include "net.h"
#include "sep_string.h"
const char *env;





int main(int argc, char *argv[]) {
    env = getenv("ENV");
    if (env != NULL) {
        printf("ENV is set to: %s\n", env);
    }else{
        env = "DEBUG";
    }
    char *buffer[256];
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
        .port = atoi(argv[1])
    };
    ConnectionParams params = {
        .listen_port = atoi(argv[1])
    };
    int rc;
    rc = pthread_create(&thread, NULL, accept_connections, &params);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    while(1){

        scanf("%s", buffer);
        if (equal(buffer, "exit") == 0){
            break;
        }
        if (strncmp(buffer, "send", 4) == 0){
            printf("Sending message: %s\n", buffer + 4);
            char* message = buffer + 5;
            send_to(&receiver, message);
        }

    }


    
    return 0;
}