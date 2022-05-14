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
    grid[16][64] = 0;
    caMessage_t msgToDisplay;
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToDisplay));

    for (;;) {

        for (int i =0;i<15;i++) {
            grid[i][20] = 1;
        }
        
        if (simulatorMsgQ != NULL) {
            
            if (xQueueReceive(simulatorMsgQ, &msgToDisplay, 10)) {
                BRD_LEDGreenOn();
            }
        }
        
        vTaskDelay(500);
    }
}


void s4575272_tsk_CAG_simulator_init(void) {
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Simulator,     // Function that implements the task
        (const signed char *) "CAGSimulatorTask",   // Text name for the task
        CAG_SIMULATOR_TASK_STACK_SIZE * 5,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_SIMULATOR_TASK_PRIORITY+1,              // Priority at which the task is created
        NULL);  

}

//Count how many neighbours of a cell

int nbr_count(int grid[16][64], int i, int j) {
    int nbr_number = 0;

    if (i >= 1 && j >= 1) {  //Not in edge/boundary
        if (grid[i-1][j-1]) {
            nbr_number++;
        }
    }

    if (i >= 1) {
        if (grid[i-1][j]) {
            nbr_number++;
        }
    }

    if (i >= 1 && j < 63) {
        if (grid[i-1][j+1]) {
            nbr_number++;
        }
    }

    if (j >= 1) {
        if (grid[i][j-1]) {
            nbr_number++;
        }
    }

    if (j < 63) {
        if (grid[i][j+1]) {
            nbr_number++;
        }
    }

    if (i < 15 && j >= 1) {
        if (grid[i+1][j]) {
            nbr_number++;
        }
    }

    if (i < 15) {
        if (grid[i+1][j]) {
            nbr_number++;
        }
    }

    if (i < 15 && j < 63) {
        if (grid[i+1][j+1]) {
            nbr_number++;
        }
    }

    return nbr_number;
}
