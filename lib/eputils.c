/**
 * Copyright (C) 2009 Ubixum, Inc. 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **/

#include <eputils.h>

#include <fx2regs.h>

#ifdef DEBUG_EPUTILS
#include <stdio.h>
#else
#define printf(...)
#endif

void readep0( BYTE* dst, WORD len) {
    WORD read = 0; // n bytes read
    BYTE c,avail;
    while (read < len) {
        avail = ep0_recv();
        for (c=0;c<avail;++c)
            dst[read+c] = EP0BUF[c];
        read += avail;
    }
}


void writeep0( BYTE* src, WORD len) {
    WORD written = 0;
    BYTE c;
    while ( written < len ) {
        ep0_busywait();
        for (c=0;c<64 && written<len;++c ) {
            EP0BUF[c] = src[written++];
        }
        ep0_load_length(c);
        printf("Write %d bytes\n", c);
    }
}

void ep0_send_descriptor(__xdata BYTE* src) {
    // The ep0_load_length will be read out of the descriptor.
    ep0_mode(EP0_DATA_AUTO, EP0_LENGTH_AUTO);
    ep0_load_sudptr(src);
}

void ep0_send_auto(__xdata BYTE* src, WORD len) {
    ep0_mode(EP0_DATA_AUTO, EP0_LENGTH_MANUAL);
    ep0_load_length(len);
    ep0_load_sudptr(src);
}

void ep0_send_byte(BYTE data) {
    ep0_mode(EP0_DATA_MANUAL, EP0_LENGTH_MANUAL);
    EP0BUF[0] = data;
    ep0_load_length(sizeof(data));
}

void ep0_send_word(WORD data) {
    ep0_mode(EP0_DATA_MANUAL, EP0_LENGTH_MANUAL);
    EP0BUF[0] = MSB(data);
    EP0BUF[1] = LSB(data);
    ep0_load_length(sizeof(data));
}

BYTE ep0_recv() {
    BYTE len = 0;
    ep0_mode(EP0_DATA_MANUAL, EP0_LENGTH_MANUAL);
    ep0_arm(); //ep0_load_length(len);
    ep0_busywait();
    len = ep0_get_length();
    // EP0BUF now has the requested data
    return len;
}
