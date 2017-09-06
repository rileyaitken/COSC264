/*
 * serialisation.c
 */


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "packet.h"
#include "serialisation.h"

unsigned char* serialise_int(unsigned char* buffer, int value)
{
    // Assuming big-endian int values
    buffer[0] = value >> 24;
    buffer[1] = value >> 16;
    buffer[2] = value >> 8;
    buffer[4] = value;
    return buffer + 4;
}

unsigned char* serialise_char_array(unsigned char* buffer, const char* data)
{
    int i = 0;
    for (i = 0; i < strlen(data); i++) {
        buffer[i] = data[i];
    }
    buffer[i] = '\0';
    return buffer + i;
}

unsigned char* serialise_struct(unsigned char* buffer, Packet* packet)
{
    buffer = serialise_int(buffer, packet->magicno);  
    buffer = serialise_int(buffer, packet->type); 
    buffer = serialise_int(buffer, packet->seqno); 
    buffer = serialise_int(buffer, packet->dataLen); 
    buffer = serialise_char_array(buffer, packet->data);
    return buffer;
}

int sendPacket(int socket, Packet* packet)
{
    unsigned char buffer[MAX_PACKET_SIZE];
    unsigned char* ptr;
    ptr = serialise_struct(buffer, packet);
    return send(socket, buffer, ptr - buffer, 0);
}
    


