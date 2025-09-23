#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <sys/types.h>

#include "sep_string.h"

/**
 * Peer represents a peer in the network. It contains
 * information about the peer itself and how to connect to it.
 *
 * @field name The name of the peer, maximum 256 bytes
 * @field id The id of the peer as a string, maximum 8 bytes
 * @field sockfd The socket file descriptor used to connect to the peer, when unused is set to -1
 * @field ip The ip address of the peer as a string
 * @field port The port number of the peer
 */
typedef struct {
    char name[256]; //256 bytes
    char* id; // 8 byte
    int sockfd; // 8 byte
    const char* ip; // 8 byte 
    uint16_t port; // 2 bytes
}Peer;

typedef struct ConnectionParams_{
    int listen_port;
}ConnectionParams;



int connect_to(Peer* peer);
int is_alive(Peer* peer);
bool are_connected(Peer* self, Peer* peer);
ssize_t send_to(Peer* peer, char* data, size_t size);
void* accept_connections(void* arg);





#endif