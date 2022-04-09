 /** 
 **************************************************************
 * @file mylib/s457527_hamming.c
 * @author Kuang Sheng - 45752720
 * @date 09/04/2022
 * @brief mylib hamming encoder and decoder
 * REFERENCE:  
 ***************************************************************
 * FUNCTIONS 
 ***************************************************************
 * s4575272_lib_hamming_byte_encode() - Return the 16bit encoded value of a byte
 * s4575272_lib_hamming_hbyte_encode() - Internal function to Encode a half byte
 * s4575272_lib_hamming_byte_decode() - Return the decoded half byte from a byte. Errors must be detected and corrected
 * s4575272_lib_hamming_parity_error() - Return 1 if a parity error has occured, else 0
 * s4575272_lib_hamming_bit_error() - Return 1 if a bit error has occured, else 0
 *************************************************************** 
 */

#include "s4575272_hamming.h"
#include "board.h"
#include "processor_hal.h"

//Return the 16bit encoded value of a byte
unsigned short s4575272_lib_hamming_byte_encode(unsigned char input) {

	uint16_t out;

	/* first encode D0..D3 (first 4 bits),
	 * then D4..D7 (second 4 bits).
	 */
	out = s4575272_lib_hamming_hbyte_encode(input & 0xF) |
		(s4575272_lib_hamming_hbyte_encode(input >> 4) << 8);

	return (out);
}


/**
  * Implement Hamming Code + parity checking
  * Hamming code is based on the following generator and parity check matrices
  * G = [ 0 1 1 | 1 0 0 0 ;
  *       1 0 1 | 0 1 0 0 ;
  *       1 1 0 | 0 0 1 0 ;
  *       1 1 1 | 0 0 0 1 ;
  *
  * hence H =
  * [ 1 0 0 | 0 1 1 1 ;
  *   0 1 0 | 1 0 1 1 ;
  *   0 0 1 | 1 1 0 1 ];
  *
  * y = x * G, syn = H * y'
  *
  *
  * NOTE: !! is used to get 1 out of non zeros
  */
//Internal function to Encode a half byte
unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in) {

    uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;

	/* extract bits */
	d0 = !!(in & 0x1);
	d1 = !!(in & 0x2);
	d2 = !!(in & 0x4);
	d3 = !!(in & 0x8);
	

	/* calculate hamming parity bits */
	h0 = d1 ^ d2 ^ d3;
	h1 = d0 ^ d2 ^ d3;
	h2 = d0 ^ d1 ^ d3;

	/* generate out byte without parity bit P0 */
	out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
		(d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);


	/* calculate even parity bit */ 
	for (z = 1; z < 8; z++)
		p0 = p0 ^ !!(out & (1 << z));

	out |= p0;

	return (out);
}

//Return the decoded half byte from a byte. Errors must be detected and corrected
unsigned char s4575272_lib_hamming_byte_decode(unsigned char value) {
	uint8_t origData;
	uint8_t d3, d2, d1, d0, h2, h1, h0, p0;

	//Extract data, hamming and parity bits
	d3 = ((value & 0x80) >> 7);
	d2 = ((value & 0x40) >> 6);
	d1 = ((value & 0x20) >> 5);
	d0 = ((value & 0x10) >> 4);
	h2 = ((value & 0x08) >> 3);
	h1 = ((value & 0x04) >> 2);
	h0 = ((value & 0x02) >> 1);
	p0 = (value & 0x01);

	parity = d3 ^ d2 ^ d1 ^ d0 ^ h2 ^ h1 ^ h0 ^ p0;

	//Correct parity bit if there's a parity error
	if (s4575272_lib_hamming_parity_error(parity)) {
		p0 = !p0;
	}

	//Calculate syndrome
	uint8_t s0 = d1 ^ d2 ^ d3 ^ h0;
	uint8_t s1 = d0 ^ d2 ^ d3 ^ h1;
	uint8_t s2 = d0 ^ d1 ^ d3 ^ h2;

	//Correct data and hamming bits if there's error
	switch(s0 | (s1 << 1) | (s2 << 2)) {
		case 0:
			bitError = 0;
			break;
		case 1:
			h0 = !h0;
			bitError = 1;
			break;
		case 2:
			h1 = !h1;
			bitError = 1;
			break;
		case 3:
			d2 = !d2;
			bitError = 1;
			break;
		case 4:
			h2 = !h2;
			bitError = 1;
			break;
		case 5:
			d1 = !d1;
			bitError = 1;
			break;
		case 6:
			d0 = !d0;
			bitError = 1;
			break;
		case 7:
		    d3 = !d3;
			bitError = 1;
			break;
	}

	origData = ((d3 << 3) | (d2 << 2) | (d1 << 1) | d0);
	return origData;
}


//Return 1 if a parity error has occured, else 0
int s4575272_lib_hamming_parity_error(unsigned char value) {
	if (value == 0) {
		return 0;
	} else {
		return 1;
	}
}

//Return 1 if a bit error has occured, else 0
int s4575272_lib_hamming_bit_error() {
	if (bitError == 0) {
		return 0;
	} else if (bitError == 1) {
		return 1;
	}
}