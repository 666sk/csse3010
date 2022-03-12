 /** 
 **************************************************************
 * @file mylib/s457527_joystick.h
 * @author Kuang Sheng - 45752720
 * @date 07/03/2022
 * @brief mylib joystick driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_joystick_pb_init()- Enable the joystick pushbutton source
 * s4575272_reg_joystick_pb_isr() - Joystick pushbutton interrupt service routine. (Callback function)
 * EXTI3_IRQHandler() - Pushbutton EXTI Handler function
 * s4575272_reg_joystick_press_get() - Returns the value of the Joystick pushbutton press counter
 * s4575272_reg_joystick_press_reset() - Reset the Joystick event counter value to 0 
 *************************************************************** 
 */

#ifndef __S4575272_JOYSTICK_H__
#define __S4575272_JOYSTICK_H__

static int joystick_press_counter;
extern void s4575272_reg_joystick_pb_init(void); //Initialization function
extern void s4575272_reg_joystick_pb_isr(void); //callback function
extern void s4575272_reg_joystick_press_reset(void); //reset counter
extern int s4575272_reg_joystick_press_get(void); //return counter value

#endif