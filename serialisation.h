/*
 * serialisation.h
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

#ifndef SERIALISATION_H
#define SERIALISATION_H
#include <stdio.h>
#include "packet.h"

char* serialise_int(char* buffer, int value);

int deserialise_int(char* buffer);

char* serialise_char_array(char* buffer, const char* data);

char* deserialise_char_array(const char* buffer);

char* serialise_packet(char* buffer, Packet* packet);

int sendPacket(int socket, Packet* packet);

Packet receivePacket(int socket);

#endif

