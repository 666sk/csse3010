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

#ifndef __S4575272_HAMMING_H__
#define __S4575272_HAMMING_H__

unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in);
unsigned short s4575272_lib_hamming_byte_encode(unsigned char input);
unsigned char s4575272_lib_hamming_byte_decode(unsigned char value);
int s4575272_lib_hamming_parity_error(unsigned char value);
int s4575272_lib_hamming_bit_error();
int parity;
int bitError = 1;

#endif