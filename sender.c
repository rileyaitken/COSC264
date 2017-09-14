/*
 * sockets.c
 *
 * Copyright 2017 Riley James Aitken <rai29@cs14136jm>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#define MAX_FILENAME_LEN 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include "packet.h"
#include "serialisation.h"
#include "functions.h"

#define MAGICNO 0x497E

int main(int argc, char **argv)
{
    int s_in_portno, s_out_portno, cs_in_portno = 0;
    int s_in_sock, s_out_sock = 0;
    //int s_in_conn = 0;
    //int s_in_addr_len = 0;
    struct sockaddr_in s_in_addr, s_out_addr, cs_in_addr;
    char fileName[MAX_FILENAME_LEN] = { 0 };
    FILE* file_pointer;
    char data_buffer[1000] = { 0 };
    Packet packet_buffer[5];
    int buffer_counter = 0;
    char c;

    int i, n = 0;
    Packet packet;  /* Packet to be sent */
    Packet rcvd;    /* Received packet */
    int next;
    _Bool exitFlag;

    /* Check there are the correct number of arguments */
    if (argc != 5) {
        error("Wrong number of arguments.");
    }
    /* Get the arguments */
    s_in_portno  = atoi(argv[1]);
    s_out_portno = atoi(argv[2]);
    cs_in_portno = atoi(argv[3]);
    strncpy(fileName, argv[4], MAX_FILENAME_LEN);

    /* Check port numbers are between 1024 and 64000 */
    if (s_in_portno < 1024 || s_in_portno > 64000) {
        error("s_in_portno out of range");
    }
    if (s_out_portno < 1024 || s_out_portno > 64000) {
        error("s_out_portno out of range");
    }
    if (cs_in_portno < 1024 || cs_in_portno > 64000) {
        error("s_in_portno out of range");
    }



    /* Create the sockets */
    if ((s_in_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the s_in socket.");
    }
    if ((setSocketOptions(&s_in_sock)) < 0) { // Enable local address reuse
        error("Error setting s_in options.");
    }
    if ((s_out_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the s_in socket.");
    }
    if ((setSocketOptions(&s_out_sock)) < 0) { // Enable local address reuse
        error("Error setting s_out options.");
    }

    /* Bind the sockets */
    if ((n = bindSocket(&s_in_sock, &s_in_addr, s_in_portno)) < 0) {
        error("Error binding s_in socket to its address.");
    }
    if ((n = bindSocket(&s_out_sock, &s_out_addr, s_in_portno)) < 0) {
        error("Error binding s_in socket to its address.");
    }
///* Make s_in socket listen */
    if (listen(s_in_sock, 5) < 0) { // Maximum packet queue size is 5
        error("Error getting s_in to 'listen'.");
    }

    getchar();

    /* Connect the s_out socket to cs_in */
    connectSocket(&s_out_sock, &cs_in_addr, cs_in_portno);


    /* Check the specified file can be accessed */
    if (access(fileName, F_OK) == -1) {
        error("The supplied filename could not be opened.");
    }

    /* Open the file for binary reading and get a file pointer */
    if ((file_pointer = fopen(fileName, "rb")) < 0) {
        error("Error opening file for reading.");
    }
    next = 0;
    exitFlag = false;

    getchar();
    
    while(1) {

        /* Read the first 512 bytes from the file */
        n = fread(data_buffer, 512, 1, file_pointer);

        if (n == 0) {
            /* If n is 0 data field will be left empty*/
            packet.magicno = MAGICNO;
            packet.type    = PTYPE_DATA;
            packet.seqno   = next;
            packet.dataLen = 0;

            exitFlag= true;
        } else if (n > 0) {
            /* If n is greater than 0 data will be sent */
            packet.magicno = MAGICNO;
            packet.type    = PTYPE_DATA;
            packet.seqno   = next;
            packet.dataLen = n;
            i = 0;
            c = data_buffer[i];
            while (c != '\n') {
                packet.data[i] = c;
                i++;
                c = data_buffer[i];
            }
        }

        /* Place packet in packet buffer */
        packet_buffer[buffer_counter] = packet;
        buffer_counter++;

        while(1) {
            /* Send the packet stored in packet buffer using s_out_sock*/
            sendPacket(s_out_sock, &packet_buffer[buffer_counter]);

            /* Wait at most 1 second for an acknowldgement packet
             * incoming on s_in_sock. If there is no response within the
             * time limit, go back to the start of this (inner) loop.
             */
            rcvd = receivePacket(s_in_sock);

            /* If an acknowledgement packet is received, then check it.
             * If anything is not as expected, go back to the start
             * of this (inner) loop.
             */
            if (rcvd.magicno != MAGICNO || rcvd.type != PTYPE_ACK ||
                    rcvd.dataLen != 0 || rcvd.seqno != next) {
                continue;
            }
            next = 1 - next;

            if (exitFlag == true) {
                fclose(file_pointer);
                exit(0);
            } else {
                /* If exitFlag is false, go back to the start of the
                 * outer loop.
                 */
                ;
            }
        }

    }
    return 0;
}
