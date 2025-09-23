#include "net.h"
#include "sep_log.h"

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
#define SEND_BUFFER_SIZE 512

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

int close_socket(int sockfd){
    if (close(sockfd) < 0){
        perror("close failed");
        return -1;
    }
    return 0;
}

/*------------------------------------------- HELPER FUNCTIONS  ---------------------------------------------------- */


/**
 * Establishes a connection to the given peer.
 * @param peer The peer to connect to.
 * @return The socket file descriptor of the connection, or -1 if the connection failed.
 */
int connect_to(Peer* peer){
    if (is_valid(peer) == false){
        return -1;
    }
    struct sockaddr_in servaddr;
    int socket = create_socket(AF_INET);
    if (socket < 0){
        return -1;
    }
    peer->sockfd = socket;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(peer->port);
    inet_pton(AF_INET, peer->ip, &(servaddr.sin_addr));
    if(connect(peer->sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        close_socket(peer->sockfd);
        peer->sockfd = -1;
        return -1;
    }
    s_log(INFO, "Connection established to peer: %s", peer->name);

    return peer->sockfd;  
}




ssize_t send_to(Peer* peer, char* data, size_t size){
    char* buffer = data;
    size_t num_of_sending = 1;
    ssize_t bytes_sended = 0; 


    if (size > SEND_BUFFER_SIZE){
        buffer = malloc(sizeof(char)*SEND_BUFFER_SIZE);
        num_of_sending = size/SEND_BUFFER_SIZE;
    }

    for (size_t i = 0; i < num_of_sending; i++){
        ssize_t chunk_sent = send(peer->sockfd, buffer, SEND_BUFFER_SIZE, 0);
        if (chunk_sent < 0) {
            perror("send failed");
            return -1;
        }
        bytes_sended += chunk_sent;
        
    }

    return bytes_sended;
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


        printf("Received message: %s\n", new_s(buffer, n)->data);    
    }


    close(connfd);
    close(sockfd);
}
