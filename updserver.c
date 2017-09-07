/*
 * udpclient.c
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
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv)
{
	int sockfd, portno, m, clientlen, optval;
    struct sockaddr_in serveraddr, clientaddr;
    struct hostent *hostp;
    char buf[BUFSIZE], *hostaddrp;
    
    /*
    * check command line arguments
    * argv[0] = 'name' of program 
    */   
    if (argc != 2) {
        fprinf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    // convert command line argument to int portno
    portno = atoi(argv[1]);

    /*
    *  create server side socket
    */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // AF_INET = address family for IPv4 addresses
    // SOCK_DGRAM = specify socket as datagram socket
    // (protocol=) 0 = specify the protocol as IPv6 Hop-by-Hop option - header needs to be analysed by all nodes on the datagram's path
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    
    /*
    * uses setsockopt so the server can be rerun
    * quickly after it has been terminated
    * also solves a potential binding error:
    * "Address already in use"
    */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
        (const void *)&optval, sizeof(int));
    // SOL_SOCKET = level at which options are being manipulated, in this case, the sockets API level
    // SO_REUSEADDR = the optname being used - allow reuse of local addresses
    // &optval = sets SO_REUSEADDR to 'true'
    // sizeof(int) = the length of the socket, 4 bits

    /*
    * build server Internet address
    */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    // intialises a number of bytes, determined by 'sizeof(serveraddr)' 
    // at the 'serveraddr' 'location' to a number of bytes equalling zero ('\0')
	return 0;
}

