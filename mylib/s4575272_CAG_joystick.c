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
 * void s4575272_tsk_CAG_joystick_init(void); - Initialise the joystick task
 * void s4575272TaskCAG_Joystick(void); - THe task of joystick
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
    //joyPbSemaphore = xSemaphoreCreateBinary();
	s4575272_reg_joystick_pb_init();
    s4575272_reg_joystick_init();

    signalMsg_t joyToSimulator;
    

    for (;;) {

        HAL_ADC_Start(&AdcHandle1); 
	    HAL_ADC_Start(&AdcHandle2);
		
        if (signalMsgQ != NULL) {

            joyToSimulator.xSignal = S4575272_REG_JOYSTICK_X_READ();
            joyToSimulator.ySignal = S4575272_REG_JOYSTICK_Y_READ();
            xQueueSendToFront(signalMsgQ, ( void * ) &joyToSimulator, ( portTickType ) 1 );
        }

		vTaskDelay(50);
	}

}