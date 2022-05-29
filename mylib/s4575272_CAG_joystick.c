 /** 
 **************************************************************
 * @file mylib/s457527_CAG_joystick.c
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

#include "s4575272_CAG_joystick.h"


//Initialise the CAG Joystick task
void s4575272_tsk_CAG_joystick_init(void) {
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Joystick,     // Function that implements the task
        (const signed char *) "CAGJoystickTask",   // Text name for the task
        CAG_JOYSTICK_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_JOYSTICK_TASK_PRIORITY,              // Priority at which the task is created
        &taskJoystick);  

}


//The task of Joystick Control
void s4575272TaskCAG_Joystick(void) {

    //Z signal
    joyPbSemaphore = xSemaphoreCreateBinary();
	s4575272_reg_joystick_pb_init();

    for (;;) {
		

		vTaskDelay(20);
	}

}