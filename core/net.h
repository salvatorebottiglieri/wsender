#ifndef NET_H
#define NET_H

#include<stdint.h>

typedef struct Peer_{
    char name[256]; //256 bytes
    char* id; // 8 byte 
    const char* ip; // 8 byte 
    uint16_t port; // 2 bytes
}Peer;

typedef struct ConnectionParams_{
    int listen_port;
}ConnectionParams;



int connect_to(Peer* peer);
int is_alive(Peer* peer);
int send_to(Peer* peer, char* message);
void* accept_connections(void* arg);





#endif