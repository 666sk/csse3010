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
        CAG_SIMULATOR_TASK_STACK_SIZE*3,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_SIMULATOR_TASK_PRIORITY,              // Priority at which the task is created
        NULL);  
}

void s4575272TaskCAG_Grid(void) {
    BRD_LEDInit();
    
    caMessage_t msgToSimulator;
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));
    //test grid
    for (int i=9; i < 12; i++) {
        grid[i][30] = 1;
        //grid[i][31] = 1;
    }

    for (;;) {
        
        if (simulatorMsgQ != NULL) {
            BRD_LEDBlueOn();
            msgToSimulator.cell_x = 3;
            msgToSimulator.cell_y = 2;
            msgToSimulator.type = ALIVE_CELL;
            xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
        }
        vTaskDelay(50);
    }

}