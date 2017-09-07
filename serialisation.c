/*
 * serialisation.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "packet.h"
#include "serialisation.h"

void error(char* errMsg)
{
    perror(errMsg);
    exit(1);
}

char* serialise_int(char* buffer, int value)
{
    // Assuming big-endian int values
    buffer[0] = value >> 24;
    buffer[1] = value >> 16;
    buffer[2] = value >> 8;
    buffer[3] = value;
    return buffer + 4;
}

int deserialise_int(char* buffer)
{  
    int value = 0;
    value |= buffer[0] << 24;
    value |= buffer[1] << 16;
    value |= buffer[2] << 8;
    value |= buffer[3];
    return value; 
}

char* serialise_char_array(char* buffer, const char* data)
{
    int i = 0;
    for (i = 0; i < strlen(buffer); i++) {
        buffer[i] = data[i];
    }
    buffer[i] = '\0';
    return buffer + i;
}

char* deserialise_char_array(const char* buffer)
{
    int i = 0;
    char* datap;
    char data[strlen(buffer) + 1];
    for (i = 0; i < strlen(buffer); i++) {
        data[i] = buffer[i];
    }
    data[i] = '\0';
    datap = &data[0];
    return datap;
}

char* serialise_packet(char* buffer, Packet* packet)
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
    char buffer[MAX_PACKET_SIZE];
    char* ptr;
    ptr = serialise_packet(buffer, packet);
    return send(socket, buffer, ptr - buffer, 0);
}

Packet receivePacket(int socket)
{
    char buffer[MAX_PACKET_SIZE];
    Packet packet;
    int n = read(socket, &buffer, MAX_PACKET_SIZE);
    if (n < 0) {
        error("There was an error receiving the packet.");
    }
    packet.magicno = deserialise_int(&buffer[0]);
    packet.type = deserialise_int(&buffer[4]);
    packet.seqno = deserialise_int(&buffer[8]);
    packet.dataLen = deserialise_int(&buffer[12]);
    packet.data = deserialise_char_array(&buffer[16]);
    return packet;
}
     


