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


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>

int bindSocket(int socket, struct sockaddr_in* inaddr, int portno)
{
    bzero((char*) inaddr, sizeof(*inaddr)); // fill the addr buffer space with zero bytes
    inaddr->sin_family = AF_INET; // Is an Internet address
    inaddr->sin_addr.s_addr = INADDR_ANY; // Get this socket's IP address (local)
    inaddr->sin_port = htons((unsigned short) portno); // Assign the port to listen on to the socket
    return bind(socket, (struct sockaddr*) inaddr, sizeof(*inaddr));
}

int connectSocket(int socket, struct sockaddr_in* inaddr, int portno)
{
    bzero((char*) inaddr, sizeof(*inaddr)); // fill the addr buffer space with zero bytes
    inaddr->sin_family = AF_INET; // Is an Internet address
    inaddr->sin_addr.s_addr = INADDR_ANY; // Get this socket's IP address (local)
    inaddr->sin_port = htons((unsigned short) portno); // Assign the port to listen on to the socket
    return connect(socket, (struct sockaddr*) inaddr, sizeof(*inaddr));
}
