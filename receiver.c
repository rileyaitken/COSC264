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

int main(int argc, char **argv)
{
	int r_in; // r_in socket
    int r_out; // r_out socket
    int r_in_portno;
    int r_out_portno;
    int cr_in_portno; // port no for channel's r_in socket
    char fileName[];

    if (argc != 5) {
        fprintf(stderr, "Missing command line arguments for program %s", argv[0]);
        exit(1);
    }
    r_in_portno = atoi(argv[1]);
    r_out_portno = atoi(argv[2]);
    cr_in_portno = atoi(argv[3]);
    if (r_in_portno < 1024 || r_in_portno > 64000) {
        fprintf(stderr, "The r_in port number is out of range.");
        exit(1);
    }
    if (r_out_portno < 10
    
    r_in = socket(AF_INET, SOCK_STREAM, 0);
    

    
	return 0;
}

