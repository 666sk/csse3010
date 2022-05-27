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
 * s4575272_tsk_CAG_simulator_init(void) - The task of simulating the game of life
 * s4575272TaskCAG_Simulator(void) - Initialise the CAG Simulator task
 * nbr_count(int grid[16][64], int i, int j) - Count how many neighbours of a cell
 *************************************************************** 
 */

#include "s4575272_CAG_simulator.h"
//The task of simulating the game of life
void s4575272TaskCAG_Simulator(void) {
    BRD_LEDInit();

    grid[16][64] = 0;
    for (int a = 30; a < 33; a++) {
        grid[12][a] = 1;
    }
    // grid[16][64] = 0;  //Initialise the global grid
    int nbr_grid[16][64];   
    nbr_grid[16][64] = 0;


    static uint32_t prevTime = 0;
    uint8_t x, y;  //grid coordinates
    uint8_t xIndex = 0; 
    uint8_t yIndex = 0;
    uint8_t cellSelect = 0;   //1 if select the cell
    uint8_t start = 1;   //0 stop 1 start
    EventBits_t uxBits;
    keyctrlEventGroup = xEventGroupCreate();

    caMessage_t msgFromGrid;
  
    simulatorMsgQ = xQueueCreate(10, sizeof(msgFromGrid));

    displaySemaphore = xSemaphoreCreateBinary();
    int flag = 0;

   

    for (;;) {

        //Event Group Bits Handling
        uxBits = xEventGroupWaitBits(keyctrlEventGroup, KEYCTRL_EVENT, pdTRUE, pdFALSE, 10);

        if ((uxBits & EVT_KEY_W) != 0) {    //if 'W pressed' detected
            if (yIndex > 0) {
                yIndex += (-1);
            }
            debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_W);
		} else if ((uxBits & EVT_KEY_A) != 0) {     //if 'A pressed' detected
            if (xIndex > 0) {
                xIndex += (-1);
            }
            debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_A);
        } else if ((uxBits & EVT_KEY_S) != 0) {     //if 'S pressed' detected
            if (yIndex < 15) {
                yIndex += 1;
            }
            debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_S);
        } else if ((uxBits & EVT_KEY_D) != 0) {     //if 'D pressed' detected
            if (xIndex < 63) {
                xIndex += 1;
            }
            debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_D);
        } else if ((uxBits & EVT_KEY_P) != 0) {     //if 'P pressed' detected
            start = ~start & 0x01;
            debug_log("Toggle Start/Stop!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_P);
        } else if ((uxBits & EVT_KEY_O) != 0) {     //if 'O pressed' detected
            xIndex = 0;
            yIndex = 0;
            debug_log("Move to Origin!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_O);
        }

                      
            //Queue receiving from mnemonic
                if (xQueueReceive(simulatorMsgQ, &msgFromGrid, 10)) {

                    if (msgFromGrid.type == ALIVE_CELL) {
                        //test if received for now
                    } else if (msgFromGrid.type == BLINKER_OSCILLATOR) {
                        //BRD_LEDGreenOn();

                        debug_log("Blinker Received: x=%d, y=%d\n\r",msgFromGrid.cell_x,msgFromGrid.cell_y);
                        grid[msgFromGrid.cell_y][msgFromGrid.cell_x] = 1;
                        grid[msgFromGrid.cell_y][msgFromGrid.cell_x+1] = 1;
                        grid[msgFromGrid.cell_y][msgFromGrid.cell_x+2] = 1;   
                    } else if (msgFromGrid.type == TOAD_OSCILLATOR) {

                        debug_log("Toad Received: x=%d, y=%d\n\r",msgFromGrid.cell_x,msgFromGrid.cell_y);
                        grid[msgFromGrid.cell_y][msgFromGrid.cell_x+1] = 1;
                        grid[msgFromGrid.cell_y][msgFromGrid.cell_x+2] = 1;
                        grid[msgFromGrid.cell_y][msgFromGrid.cell_x+3] = 1; 
                        grid[msgFromGrid.cell_y+1][msgFromGrid.cell_x] = 1;
                        grid[msgFromGrid.cell_y+1][msgFromGrid.cell_x+1] = 1;
                        grid[msgFromGrid.cell_y+1][msgFromGrid.cell_x+2] = 1; 
                    }

                }

            // if (xSemaphoreTake(displaySemaphore, 10 ) == pdTRUE){
            //     flag = 1;
            // } else {
            //     flag = 1;
            // }


            //if (flag) {
                if ((uxBits & EVT_KEY_C) != 0) {     //if 'C pressed' detected
            
                    debug_log("Clear Display!\n\r");
                    for (y = 0; y < 16; y++) {
                            for (x = 0; x < 64; x++) {
                                grid[y][x] = 0;
                            }
                    }
                    uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_C);
                } else if ((uxBits & EVT_KEY_X) != 0) {     //if 'X pressed' detected
            
                    grid[yIndex][xIndex] = 1;
                    debug_log("Select Cell!\n\r");
                    uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_X);
                } else if ((uxBits & EVT_KEY_Z) != 0) {     //if 'Z pressed' detected
                
                    grid[yIndex][xIndex] = 0;
                    debug_log("Unselect Cell!\n\r");
                    cellSelect = 0;
                    uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_Z);
                }

                BRD_LEDGreenToggle();
                
                if ((HAL_GetTick() - prevTime) > 950) {
                for (y = 0; y < 16; y++) {

                //Store the info of neighbour number of each cell
                    for (x = 0; x < 64; x++) {

                        nbr_grid[y][x] = nbr_count(grid, y, x);
                    }
                }
            

                for (y = 0; y < 16; y++) {

                    for (x = 0; x < 64; x++) {

                        if (start == 1) {
                    
                            if (grid[y][x]) {  //if the cell is alive
                    
                                if (nbr_grid[y][x] <= 1 || nbr_grid[y][x] >= 4) {
                                    grid[y][x] = 0;
                                } 
                            } else {    //if the cell is dead(no alive cell)
                    
                                if (nbr_grid[y][x] == 3) {
                                    grid[y][x] = 1;
                                }
                            }
                        }
                    }
                }
                prevTime = HAL_GetTick();
                }
                
            //}

            
            
        //}
        
        vTaskDelay(50);
    }
}

//Initialise the CAG Simulator task
void s4575272_tsk_CAG_simulator_init(void) {
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Simulator,     // Function that implements the task
        (const signed char *) "CAGSimulatorTask",   // Text name for the task
        CAG_SIMULATOR_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_SIMULATOR_TASK_PRIORITY+1,              // Priority at which the task is created
        &taskSim);  

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
