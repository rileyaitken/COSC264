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

#define VARNAME(name) #name 
#define MAX_PACKET_SIZE 2048

void error(char* errMsg)
{
    perror(errMsg);
    exit(1);
} 

void bindSocket(int* sock, struct sockaddr_in* sockaddr)
{
    bzero((char*) sockaddr, sizeof(*sockaddr)); // fill the addr buffer space with zero bytes
    *sockaddr.sin_family = AF_INET; // Is an Internet address
    *sockaddr.sin_addr.s_addr = INADDR_ANY; // Get this socket's IP address
    *sockaddr.sin_port = htons((unsigned short) r_in_portno); // Assign the port to listen on to the socket
    if (bind(*r_in, (struct sockaddr*) sockaddr, sizeof(*sockaddr)) < 0) {
        error("Error binding %s socket to its address.", VARNAME(*sock));
    }
}

int main(int argc, char **argv)
{
	int r_in; // r_in socket
    int r_out; // r_out socket
    int cr_out;
    int r_in_portno;
    int r_out_portno;
    int cr_in_portno; // port no for channel's r_in socket
    struct sockaddr_in rinaddr, routaddr, crinaddr, croutaddr;
    char fileName[];
    int i, expected = 0;
    Packet inc_packet, ack_packet;    

    if (argc != 5) {
        fprintf(stderr, "Missing command line arguments for program %s", argv[0]);
        exit(1);
    }
    r_in_portno = atoi(argv[1]);
    r_out_portno = atoi(argv[2]);
    cr_in_portno = atoi(argv[3]);
    while (argv[4][i] != '\0') {
        fileName[i] = argv[4][i];
        i++;
    }
    fileName[i] = '\0';    

    if (r_in_portno < 1024 || r_in_portno > 64000) {
        error("The r_in port number is out of range.");
    }
    if (r_out_portno < 1024 || r_out_portno > 64000) {
        error("The r_out port number is out of range.");
    }
    if (access(fileName, F_OK == -1) {
        error("The supplied filename already exists, or a related error occurred.");
    }
    if (r_in = socket(AF_INET, SOCK_STREAM, 0) < 0) {
        error("Error creating the r_in socket.");
    }
    if (r_out = socket(AF_INET, SOCK_STREAM, 0) < 0) {
        error("Error creating the r_out socket.");
    }
    
    bindSocket(&r_in, &rinaddr);
    
    bzero((char*) &crinaddr, sizeof(crinaddr));
    crinaddr.sin_family = AF_INET;
    crinaddr.sin_addr.s_addr = INADDR_ANY;
    crinaddr.sin_port = htons(cr_in_portno);
    if (connect(r_out, &crinaddr, sizeof(crinaddr)) < 0) {
        error("Error connecting 'r_out' to 'cs_in' port.");
    }

    if ((fp = fopen(fileName, "w")) < 0) {
        error("Error opening file %s for writing.", fileName); 
    }

    if (listen(r_in, 5) < 0) { // Maximum packet queue size is 5
        error("Error getting r_in to 'listen'.");
    }

    while(1) {
        
        if ((cr_out_conn = accept(r_in, (struct sockaddr*) &croutaddr, sizeof(croutaddr))) < 0) {
            error("Error accepting incoming connection on r_in.");
        }

        if (read(cr_out_conn, inc_packet, MAX_PACKET_SIZE) < 0) {
            error("Error receiving packet.");
        }        
    
        if (inc_packet.magicno != 0x497E) {
            ack_packet.magicno = 0x497E;
            ack_packet.type = PTYPE_ACK;
            ack_packet.seqno = inc_packet.seqno;
            ack_packet.dataLen = 0;
            
    
    
	return 0;
}

