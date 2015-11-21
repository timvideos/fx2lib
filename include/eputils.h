// Copyright (C) 2009 Ubixum, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

/*! \file 
 *  Functions and macros for working with endpoints.
 * */

#ifndef EPUTILS_H
#define EPUTILS_H

#include <assert.h>

#include "fx2types.h"
#include "fx2macros.h"

/**
 * NOTE you can't use these unless you define SYNCDELAY 
 * as a macro or function.  The reason is that SYNCDELAY
 * needs to be longer or shorter depending on your IFCONFIG
 * settings.  
 * See delay.h
 *
 * Example:
 * \code
 *  #define SYNCDELAY SYNCDELAY4 // SYNCDELAY4 from delay.h
 * \endcode
 *
 *
 **/

/**
 * Stalls EP0.
 **/
#define STALLEP0() EP0CS |= bmEPSTALL

/**
 * \brief Reset the toggle on an endpoint.
 * To use this, the endpoint needs bit 8 to be IN=1,OUT=0
 **/
#define RESETTOGGLE(ep) do {\
	BYTE x = ep; \
	if (x&0x80) { x |= 0x10; } \
	x &= 0x1F; \
	TOGCTL = x; \
	TOGCTL = x | bmRESETTOGGLE; \
} while (0)


/**
 * RESETFIFO should not use 0x80|epnum for IN endpoints
 * Only use 0x02, 0x04, 0x06, 0x06 for ep value
 **/
#define RESETFIFO(ep) {FIFORESET=0x80; SYNCDELAY;\
                       FIFORESET=0x80 | ep; SYNCDELAY;\
                       FIFORESET=0x00; SYNCDELAY;}
/**
 * Quickly reset all endpoint FIFOS.
 **/
#define RESETFIFOS() {FIFORESET=0x80; SYNCDELAY;\
                     FIFORESET=0x82; SYNCDELAY;\
                     FIFORESET=0x84; SYNCDELAY;\
                     FIFORESET=0x86; SYNCDELAY;\
                     FIFORESET=0x88; SYNCDELAY;\
                     FIFORESET=0x00; SYNCDELAY;}

/**
 * Continually read available bytes from endpoint0 into dst, wait
 * until more bytes are available, and loop until len bytes have 
 * been read.
 **/
void readep0( BYTE* dst, WORD len );

/**
 * Write bytes from src to ep0, allowing host to transfer data
 * between 64 byte blocks.
 **/
void writeep0( BYTE* src, WORD len );

// The Setup Data Pointer can access data in either of two RAM spaces:
//  - On-chip Main RAM (8 KB at 0x0000-0x1FFF)
//  - On-chip Scratch RAM (512 bytes at 0xE000-0xE1FF)
// The base address of SUDPTRH:L must be word-aligned.
#define ep0_load_sudptr(src) \
	assert( \
		(((WORD)src) <= 0x3FFF) || \
		(((WORD)src) >= 0xE000 && ((WORD)src) <= 0xE1FF)); \
	assert(!(LSB(src) & bmBIT0)); \
	LOADWORD(SUDPTR, src);

// For manual mode, SUDPTRCTL must be in "auto read length mode".
//
// |  Data Read  | Data Length | SUDPTRCTL |
// |-------------|-------------|-----------|
// | Auto   (0)  | Manual (0)  |  0|0 = 0  | SUDPTR->EP0BC
// | Manual (1)  | Manual (0)  |  1|0 = 1  | EP0BUF->EP0BC
// | Auto   (0)  | Auto   (1)  |  0|1 = 1  | SUDPTR
// | Manual (1)  | Auto   (1)  |  Invalid  | NA
enum ep0_mode_data {
	EP0_DATA_AUTO    = 0,
	EP0_DATA_MANUAL  = 1,
};
enum ep0_mode_length {
	EP0_LENGTH_AUTO   = 1,
	EP0_LENGTH_MANUAL = 0,
};

#define ep0_mode(mode_data, mode_length) \
	assert(!(mode_data & mode_length)); \
	SUDPTRCTL = mode_data | mode_length;

#define ep0_busywait() \
	while (EP0CS & bmEPBUSY) printf("w\n");

#define ep0_load_length(len) \
	LOADWORD(EP0BC, len);

#define ep0_arm() \
	ep0_load_length(0);

// ep0 can only receive 64 bytes
#define ep0_get_length() \
	EP0BCL

void ep0_send_auto(__xdata BYTE* src, WORD len);
void ep0_send_byte(BYTE data);
void ep0_send_word(WORD data);

BYTE ep0_recv();

#endif
