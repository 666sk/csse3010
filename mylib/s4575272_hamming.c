
#include "board.h"
#include "processor_hal.h"

unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in);

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
	uint8_t d3, d2, d1, d0, h2, h1, h0, p0, parity;
	//debug_log("value = %u\n\r", value);

	d3 = ((value & 0x80) >> 7);
	d2 = ((value & 0x40) >> 6);
	d1 = ((value & 0x20) >> 5);
	d0 = ((value & 0x10) >> 4);
	h2 = ((value & 0x08) >> 3);
	h1 = ((value & 0x04) >> 2);
	h0 = ((value & 0x02) >> 1);
	p0 = (value & 0x01);

	//debug_log("d3, d2, d1, d0 = %u, %u, %u, %u\n\r", d3, d2, d1, d0); //70 54
	parity = d3 ^ d2 ^ d1 ^ d0 ^ h2 ^ h1 ^ h0 ^ p0;

	uint8_t s0 = d1 ^ d2 ^ d3 ^ h0;
	uint8_t s1 = d0 ^ d2 ^ d3 ^ h1;
	uint8_t s2 = d0 ^ d1 ^ d3 ^ h2;

	switch(s0 | (s1 << 1) | (s2 << 2)) {
		case 0:
			break;
		case 1:
			h0 = !h0;
			break;
		case 2:
			h1 = !h1;
			break;
		case 3:
			d2 = !d2;
			break;
		case 4:
			h2 = !h2;
			break;
		case 5:
			d1 = !d1;
			break;
		case 6:
			d0 = !d0;
			break;
		case 7:
		    d3 = !d3;
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