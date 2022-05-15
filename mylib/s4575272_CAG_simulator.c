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
    int nbr_grid[16][64];
    nbr_grid[16][64] = 0;
    //int nbr_count = 0;
    int x,y;

    caMessage_t msgFromGrid;
    xQueueSet = xQueueCreateSet(sizeof(msgFromGrid)+10);
    simulatorMsgQ = xQueueCreate(10, sizeof(msgFromGrid));
    xQueueAddToSet(simulatorMsgQ, xQueueSet);

    for (;;) {

        for (y = 0; y < 16; y++) {
            for (x = 0; x < 64; x++) {
                nbr_grid[y][x] = nbr_count(grid, y, x);
            }
        }
        
        for (y = 0; y < 16; y++) {
            for (x = 0; x < 64; x++) {
                if (grid[y][x]) {
                    if (nbr_grid[y][x] <= 1 || nbr_grid[y][x] >= 4) {
                        grid[y][x] = 0;
                    } 
                } else {
                    if (nbr_grid[y][x] == 3) {
                        grid[y][x] = 1;
                    }
                }
            }
        }

        xActivatedMember = xQueueSelectFromSet(xQueueSet, 20);

        //if (xActivatedMember == simulatorMsgQ) { 
                      
            if (xQueueReceive(simulatorMsgQ, &msgFromGrid, 10)) {
                if (msgFromGrid.type == 0x11) {
                    BRD_LEDGreenOn();
                }
                
            }
        //}
        
        vTaskDelay(1);
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
        if (grid[i+1][j-1]) {
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
