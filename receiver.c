/*
 * receiver.c
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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "packet.h"
#include "serialisation.h"
#include "functions.h"
#include <strings.h>

#define VARNAME(name) #name
#define MAX_PACKET_SIZE 2048


int main(int argc, char **argv)
{
    int n; // Simple variable for checking return values of function calls
    int r_in; // r_in socket
    int r_out; // r_out socket
    int cr_out_conn;
    int r_in_portno;
    int r_out_portno;
    int cr_in_portno; // port no for channel's r_in socket
    socklen_t croutaddrlen;
    FILE* fp;
    struct sockaddr_in rinaddr, crinaddr, croutaddr, routaddr;
    char fileName[255] = {'\0'}; // Maximum filename length is generally 255 bytes
    int i = 0;
    int expected = 0;
    Packet inc_packet, ack_packet;
    
    if (argc != 5) {
        fprintf(stderr, "Missing command line arguments for program %s", argv[0]);
        exit(1);
    }
    r_in_portno = atoi(argv[1]);
    r_out_portno = atoi(argv[2]);
    cr_in_portno = atoi(argv[3]);
    
    while (argv[4][i] != '\0') {
        if (i == 255) {
            error("Given filename too long.");
        }
        fileName[i] = argv[4][i];
        i++;
    }

    if (r_in_portno < 1024 || r_in_portno > 64000) {
        error("The r_in port number is out of range.");
    }
    if (r_out_portno < 1024 || r_out_portno > 64000) {
        error("The r_out port number is out of range.");
    }
    if (cr_in_portno < 1024 || cr_in_portno > 64000) {
        error("The cr_in port number is out of range.");
    }
    if (access(fileName, F_OK) == 0) {
        error("The supplied filename already exists, or a related error occurred.");
    }
    if ((r_in = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the r_in socket.");
    }
    if ((setSocketOptions(&r_in)) < 0) { // Enable local address reuse
        error("Error setting s_in options.");
    }
    if ((r_out = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the r_out socket.");
    }
    if ((setSocketOptions(&r_out)) < 0) { // Enable local address reuse
        error("Error setting s_in options.");
    }

    /*bzero((char*) &rinaddr, sizeof(rinaddr)); // fill the addr buffer space with zero bytes
    rinaddr.sin_family = AF_INET; // Is an Internet address
    rinaddr.sin_addr.s_addr = INADDR_ANY; // Get this socket's IP address (local)
    rinaddr.sin_port = htons((unsigned short) r_in_portno); // Assign the port to listen on to the socket
    if (bind(r_in, (struct sockaddr*) &rinaddr, sizeof(rinaddr)) < 0) {
        error("Error binding r_in socket to its address.");
    }

    bzero((char*) &crinaddr, sizeof(crinaddr));
    crinaddr.sin_family = AF_INET;
    crinaddr.sin_addr.s_addr = INADDR_ANY;
    crinaddr.sin_port = htons(cr_in_portno);
    if (connect(r_out, (const struct sockaddr*) &crinaddr, sizeof(crinaddr)) < 0) {
        error("Error connecting 'r_out' to 'cs_in' port.");
    }
    */

    if ((n = bindSocket(&r_in, &rinaddr, r_in_portno)) < 0) {
        error("Error binding r_in socket to its address.");
    }
    if ((n = bindSocket(&r_out, &routaddr, r_out_portno)) < 0) {
        error("Error binding r_out socket to its address.");
    }
    if (listen(r_in, 5) < 0) { // Maximum packet queue size is 5
        error("Error getting r_in to 'listen'.");
    }

    getchar();

    if ((n = connectSocket(&r_out, &crinaddr, cr_in_portno)) < 0) {
        error("Error connecting r_out socket to cr_in port.");
    }

    if ((fp = fopen(fileName, "w")) < 0) {
        error("Error opening file for writing.");
    }

    getchar();

    expected = 0;

    while(1) {

        croutaddrlen = sizeof(croutaddr);
        if ((cr_out_conn = accept(r_in, (struct sockaddr*) &croutaddr, &croutaddrlen)) < 0) {
            error("Error accepting incoming connection on r_in.");
        }

        inc_packet = receivePacket(cr_out_conn);

        close(cr_out_conn);    // Packet has been extracted from incoming connection; close the file descriptor

        if (inc_packet.magicno != 0x497E || inc_packet.type != PTYPE_DATA) {
            continue;
        }

        if (inc_packet.seqno != expected) {
            ack_packet.magicno = 0x497E;
            ack_packet.type = PTYPE_ACK;
            ack_packet.seqno = inc_packet.seqno;
            ack_packet.dataLen = 0;
            n = sendPacket(r_out, &ack_packet);
            if (n < 0) {
                error("Error sending acknowledgement packet from 'receiver'.");
            }
        } else {
            ack_packet.magicno = 0x497E;
            ack_packet.type = PTYPE_ACK;
            ack_packet.seqno = inc_packet.seqno;
            ack_packet.dataLen = 0;
            n = sendPacket(r_out, &ack_packet);
            if (n < 0) {
                error("Error sending acknowledgement packet from 'receiver'.");
            }
            expected = 1 - expected;
        }

        if (inc_packet.dataLen > 0) {
            if ((n = fputs(inc_packet.data, fp)) < 0) {
                error("Error writing packet data to file on 'reciever'.");
            }
        } else if (inc_packet.dataLen == 0) {
            if ((n = fclose(fp)) < 0) {
                error("Error closing file on 'receiver'.");
            }
            if ((n = close(r_in)) < 0) {
                error("Error closing the r_in socket.");
            }
            if ((n = close(r_out)) < 0) {
                error("Error closing the r_out socket.");
            }
            exit(0);
        }
    }
}

