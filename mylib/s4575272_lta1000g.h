/** 
 **************************************************************
 * @file s4575272_lta1000g.h
 * @author Kuang Sheng - 45752720
 * @date 07/03/2022
 * @brief Light Bar Header file
 * REFERENCE: DON'T JUST COPY THIS BLINDLY.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_lta1000g_init() - intialise LED Bar GPIO pins as outputs
 * lta1000g_seg_set(int segment, unsigned char segment_value) - set the value for the corresponding LED Bar segment GPIO pin 
 * s4575272_reg_lta1000g_write(unsigned short value) - Write the LED Bar segments high or low
 *************************************************************** 
 */
#ifndef __S4575272_LTA1000G_H__
#define __S4575272_LTA1000G_H__

extern void s4575272_reg_lta1000g_init(); //Initialization function
void lta1000g_seg_set(int segment, unsigned char segment_value); //set the value for the corresponding LED Bar segment GPIO pin 
extern void s4575272_reg_lta1000g_write(unsigned short value);//Write the LED Bar segments high or low

#endif