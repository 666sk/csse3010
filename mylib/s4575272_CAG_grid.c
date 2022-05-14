 /** 
 **************************************************************
 * @file mylib/s457527_CAG_grid.c
 * @author Kuang Sheng - 45752720
 * @date 15/05/2022
 * @brief mylib CAG grid driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */

#include "s4575272_CAG_grid.h"

void s4575272_tsk_CAG_grid_init(void) {
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Grid,     // Function that implements the task
        (const signed char *) "TaskCAG_Grid",   // Text name for the task
        CAG_SIMULATOR_TASK_STACK_SIZE*5,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_SIMULATOR_TASK_PRIORITY,              // Priority at which the task is created
        NULL);  
}

void s4575272TaskCAG_Grid(void) {
    BRD_LEDInit();
    
    caMessage_t msgToSimulator;
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));
    
    for (;;) {
        
        if (simulatorMsgQ != NULL) {
            BRD_LEDBlueOn();
            msgToSimulator.cell_x = 0;
            xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
        }
        vTaskDelay(50);
    }

}