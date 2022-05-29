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
 * s4575272_reg_joystick_init() - Initialise GPIO and ADC
 * s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput) - Generic function to read X or Y joystick value
 *************************************************************** 
 */

#ifndef __S4575272_JOYSTICK_H__
#define __S4575272_JOYSTICK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


// Task Priorities (Idle Priority is the lowest priority)
#define OLEDTASK_PRIORITY		( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define OLEDTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


SemaphoreHandle_t joyPbSemaphore;	// Semaphore for pushbutton interrupt
QueueHandle_t sendToLeftQ;
QueueHandle_t sendToRightQ;

static int joystick_press_counter;
extern void s4575272_reg_joystick_pb_init(void); //Initialization function
extern void s4575272_reg_joystick_pb_isr(void); //callback function
extern void s4575272_reg_joystick_press_reset(void); //reset counter
extern int s4575272_reg_joystick_press_get(void); //return counter value

void s4575272_reg_joystick_init(void);
int s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput);
#define S4575272_REG_JOYSTICK_X_READ() s4575272_joystick_readxy(AdcHandle1)    //read ADC1
#define S4575272_REG_JOYSTICK_Y_READ() s4575272_joystick_readxy(AdcHandle2)    //read ADC2
//Calibrate (add offset) the Joystick X signal, so the stationary value is half the reference voltage
#define S4575272_REG_JOYSTICK_X_ZERO_CAL_OFFSET 30
//Calibrate (add offset) the Joystick Y signal, so the stationary value is half the reference voltage
#define S4575272_REG_JOYSTICK_Y_ZERO_CAL_OFFSET -112

#endif