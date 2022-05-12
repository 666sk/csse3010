 /** 
 **************************************************************
 * @file mylib/s457527_CAG_simulator.c
 * @author Kuang Sheng - 45752720
 * @date 11/05/2022
 * @brief mylib CAG simulator driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */

#include "s4575272_CAG_simulator.h"


void s4575272TaskCAG_Simulator(void) {
    BRD_LEDInit();

    for (;;) {
        BRD_LEDGreenToggle();
        BRD_LEDBlueOn();
        vTaskDelay(1000);
    }
}


void s4575272_tsk_CAG_simulator_init(void) {

    xTaskCreate(
        (void *) &s4575272TaskCAG_Simulator,     // Function that implements the task
        (const signed char *) "CAGSimulatorTask",   // Text name for the task
        CAG_SIMULATOR_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_SIMULATOR_TASK_PRIORITY,              // Priority at which the task is created
        NULL);  

}


