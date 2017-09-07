/*
 * channel.c
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
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define VARNAME(name) #name

void error(char* errMsg)
{
    perror(errMsg);
    exit(1);
}

int main(int argc, char **argv)
{
    int cs_in, cs_out, cr_in, cr_out;
    int n; // simple value for checking the return values of function calls
    int cs_in_portno, cs_out_portno, cr_in_portno, cr_out_portno;
    int sin_portno, rin_portno;
    struct sockaddr_in csinaddr, soutaddr, crinaddr, routaddr, rinaddr, sinaddr; 
    double p;

    if (argc != 8) {
        error("Missing command line arguments for program %s", argv[0]);
    }
    cs_in_portno = atoi(argv[1]);
    cs_out_portno = atoi(argv[2]);
    cr_in_portno = atoi(argv[3]);
    cr_out_portno = atoi(argv[4]);
    sin_portno = atoi(argv[5]);
    rin_portno = atoi(argv[6]);
    p = atof(argv[7]);

    if (cs_in_portno < 1024 || r_in_portno > 64000) {
        error("The cs_in port number is out of range.");
    }
    if (cs_out_portno < 1024 || r_out_portno > 64000) {
        error("The cs_out port number is out of range.");
    }
    if (cr_in_portno < 1024 || r_in_portno > 64000) {
        error("The cr_in port number is out of range.");
    }
    if (cr_out_portno < 1024 || r_out_portno > 64000) {
        error("The cr_out port number is out of range.");
    }
    if (sin_portno < 1024 || r_in_portno > 64000) {
        error("The s_in port number is out of range.");
    }
    if (rin_portno < 1024 || r_out_portno > 64000) {
        error("The r_in port number is out of range.");
    }
    if (p < 0 || p > 1) {
        error("The given 'P' value is out of range.");
    }

    if ((cr_in = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the cr_in socket.");
    }
    if ((cr_out = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the cr_out socket.");
    }
    if ((cs_in = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the cs_in socket.");
    }
    if ((cs_out = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Error creating the cs_out socket.");
    }

    if ((n = bindSocket(cr_in, &crinaddr, cr_in_portno)) < 0) {
        error("Error binding cr_in socket to its address.");
    }
    if ((n = bindSocket(cr_out, &croutaddr, cr_out_portno)) < 0) {
        error("Error binding cr_in socket to its address.");
    }
    if ((n = bindSocket(cs_in, &csinaddr, cs_in_portno)) < 0) {
        error("Error binding cr_in socket to its address.");
    }
    if ((n = bindSocket(cs_out, &csoutaddr, cs_out_portno)) < 0) {
        error("Error binding cr_in socket to its address.");
    }

    if ((n = connectSocket(cr_out, &rinaddr, rin_portno)) < 0) {
        error("Error connecting cr_out socket to r_in socket.");
    }
    if ((n = connectSocket(cs_out, &sinaddr, sin_portno)) < 0) {
        error("Error connecting cs_out socket to s_in socket.");
    }
    
    while
    
    return 0;
}

