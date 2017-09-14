/*
 * packet.h
 * 
 * Defines the 'Packet' type,
 * and a few constants.
 */

#ifndef PACKET_H 
#define PACKET_H
#include <stdio.h>

typedef struct packet_s Packet;

#define PTYPE_DATA 0
#define PTYPE_ACK 1
#define MAX_PACKET_SIZE 2048

struct packet_s{
    int magicno;
    int type;
    int seqno;
    int dataLen;
    int initLen;
    char* data;
};

#endif
