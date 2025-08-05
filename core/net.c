#include "net.h"

#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



#define LISTEN_PORT 8080

/*------------------------------------------- HELPER FUNCTIONS ---------------------------------------------------- */
bool is_valid(Peer* peer){
    if (peer->name == 0 || peer->id == 0 || peer->ip == 0 || peer->port == 0){
        return false;
    }
    return true;
}

int create_socket(int domain){
    int sockfd = socket(domain, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }
    return sockfd;
}


/*------------------------------------------- HELPER FUNCTIONS  ---------------------------------------------------- */


int connect_to(Peer* peer){
    if (is_valid(peer) == false){
        return -1;
    }

    int sockfd = create_socket(AF_INET);
    if (sockfd < 0){
        return -1;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(peer->port);
    inet_pton(AF_INET, peer->ip, &(servaddr.sin_addr));
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connection failed");
        return -1;
    }

    return sockfd;  
}


int send_to(Peer* peer, String* message){
    int sockfd = connect_to(peer);
    if (sockfd < 0){
        return -1;
    }

    int bytes_sent = send(sockfd, to_c_string(message), message->size, 0);
    if (bytes_sent < 0) {
        perror("send failed");
        return -1;
    }

    close(sockfd);
    return bytes_sent;
}


void* accept_connections(void* arg){
    ConnectionParams* params = (ConnectionParams*) arg;
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buffer[256];

    sockfd = create_socket(AF_INET);
    if (sockfd < 0){
        pthread_exit(NULL);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(params->listen_port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        pthread_exit(NULL);
    }

    if (listen(sockfd, 3) < 0) {
        perror("listen failed");
        pthread_exit(NULL);
    }

    clilen = sizeof(cliaddr);

    while(1){
        connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("accept failed");
            pthread_exit(NULL);
        }
        int n = read(connfd, buffer, 255);
        if (n < 0) {
            perror("read failed");
            pthread_exit(NULL);
        }


        printf("Received message: %s\n", to_c_string(new_s(buffer, n)));    
    }


    close(connfd);
    close(sockfd);
}
