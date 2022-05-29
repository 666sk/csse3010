 /** 
 **************************************************************
 * @file mylib/s457527_CAG_joystick.h
 * @author Kuang Sheng - 45752720
 * @date 15/05/2022
 * @brief mylib CAG joystick driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 *
 *************************************************************** 
 */

#ifndef __S4575272_CAG_JOYSTICK_H__
#define __S4575272_CAG_JOYSTICK_H__

#include "s4575272_CAG_simulator.h"
#include "s4575272_joystick.h"

#define CAG_JOYSTICK_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 5 )
#define CAG_JOYSTICK_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )

ADC_HandleTypeDef AdcHandle1,AdcHandle2;
ADC_ChannelConfTypeDef AdcChanConfig1,AdcChanConfig2;

#define S4575272_REG_JOYSTICK_X_READ() s4575272_joystick_readxy(AdcHandle1)
#define S4575272_REG_JOYSTICK_Y_READ() s4575272_joystick_readxy(AdcHandle2)



void s4575272_tsk_CAG_joystick_init(void);
void s4575272TaskCAG_Joystick(void);

#endif