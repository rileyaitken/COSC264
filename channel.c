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
#include "serialisation.h"
#include "functions.h"
#include <sys/select.h>

#define VARNAME(name) #name

int main(int argc, char **argv)
{
    int cs_in, cs_out, cr_in, cr_out = 0;
    int retVal = 0, n = 0, i = 0; // simple value for checking the return values of function calls
    int cs_in_portno, cs_out_portno, cr_in_portno, cr_out_portno = 0;
    int sin_portno, rin_portno = 0;
    int s_out_conn, r_out_conn = 0;
    socklen_t soutaddrlen, routaddrlen = 0;
    struct sockaddr_in csinaddr, soutaddr, crinaddr, routaddr, rinaddr, sinaddr, csoutaddr, croutaddr;
    fd_set readSockets;
    Packet inc_packet;
    double p = 0;
    double u, v = 0; // The pseudo-random double variables

    if (argc != 8) {
        fprintf(stderr, "Missing command line arguments for program %s", argv[0]);
        exit(1);
    }

    cs_in_portno = atoi(argv[1]);
    cs_out_portno = atoi(argv[2]);
    cr_in_portno = atoi(argv[3]);
    cr_out_portno = atoi(argv[4]);
    sin_portno = atoi(argv[5]);
    rin_portno = atoi(argv[6]);
    p = atof(argv[7]);

    if (cs_in_portno < 1024 || cs_in_portno > 64000) {
        error("The cs_in port number is out of range.");
    }
    if (cs_out_portno < 1024 || cs_out_portno > 64000) {
        error("The cs_out port number is out of range.");
    }
    if (cr_in_portno < 1024 || cr_in_portno > 64000) {
        error("The cr_in port number is out of range.");
    }
    if (cr_out_portno < 1024 || cr_out_portno > 64000) {
        error("The cr_out port number is out of range.");
    }
    if (sin_portno < 1024 || sin_portno > 64000) {
        error("The s_in port number is out of range.");
    }
    if (rin_portno < 1024 || rin_portno > 64000) {
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

    if ((n = listen(cr_in, 5)) < 0) {
        error("Error getting cr_in socket to listen.");
    }
    if ((n = listen(cs_in, 5)) < 0) {
        error("Error getting cr_in socket to listen.");
    }

    FD_ZERO(&readSockets);
    FD_SET(cr_in, &readSockets);
    FD_SET(cs_in, &readSockets);
    n = cs_in + 1; // cs_in is the 'greater' file descriptor

    while(1) {

        if ((retVal = select(n, &readSockets, NULL, NULL, NULL)) < 0) {
            error("Error on select()");
        }
        for (i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &readSockets)) {
                if (i == cs_in) {
                    soutaddrlen = sizeof(soutaddr);
                    if ((s_out_conn = accept(cs_in, (struct sockaddr*) &soutaddr, &soutaddrlen)) < 0) {
                        error("Error accepting incoming connection on cs_in.");
                    }
                    inc_packet = receivePacket(s_out_conn);
                    close(s_out_conn);

                    if (inc_packet.magicno != 0x497E) {
                        continue;
                    }

                    generateRandNum(&u);
                    // Generate a random int between 0 and 10000, then divide that integer by 10000 to get the floating point value
                    if (u < p) {
                        continue;
                    } else {
                        generateRandNum(&v);
                        if (v < 0.1) {
                            inc_packet.dataLen += ((rand() % 10) + 1);
                        }

                        retVal = sendPacket(cs_out, &inc_packet);
                        if (retVal < 0) {
                            error("Error sending packet on cr_out to r_in.");
                        }
                    }
                } else if (i == cr_in) {
                    routaddrlen = sizeof(routaddr);
                    if ((r_out_conn = accept(cr_in, (struct sockaddr*) &routaddr, &routaddrlen)) < 0) {
                        error("Error accepting incoming connection on cr_in.");
                    }
                    inc_packet = receivePacket(r_out_conn);
                    close(r_out_conn);

                    if (inc_packet.magicno != 0x497E) {
                        continue;
                    }

                    generateRandNum(&u);
                    // Generate a random int between 0 and 10000, then divide that integer by 10000 to get the floating point value
                    if (u < p) {
                        continue;
                    } else {
                        generateRandNum(&v);
                        if (v < 0.1) {
                            inc_packet.dataLen += ((rand() % 10) + 1); // generate random int between 1 and 10
                        }

                        retVal = sendPacket(cs_out, &inc_packet);
                        if (retVal < 0) {
                            error("Error sending packet on cr_out to r_in.");
                        }

                    }
                }
            }
        }
    }
    return 0;
}

