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

    grid[16][64] = 0;  //Initialise the global grid
    int nbr_grid[16][64];   
    nbr_grid[16][64] = 0;
    
    uint8_t x, y;  //grid coordinates
    uint8_t cellSelect = 0;   //1 if select the cell

    EventBits_t uxBits;
    keyctrlEventGroup = xEventGroupCreate();

    caMessage_t msgFromGrid;
    xQueueSet = xQueueCreateSet(sizeof(msgFromGrid)+10);
    simulatorMsgQ = xQueueCreate(10, sizeof(msgFromGrid));
    xQueueAddToSet(simulatorMsgQ, xQueueSet);

    for (;;) {
        //BRD_LEDBlueOn();

        for (y = 0; y < 16; y++) {

            //Store the info of neighbour number of each cell
            for (x = 0; x < 64; x++) {

                nbr_grid[y][x] = nbr_count(grid, y, x);
            }
        }
        
        for (y = 0; y < 16; y++) {

            for (x = 0; x < 64; x++) {
            
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

        //Event Group Bits Handling
        uxBits = xEventGroupWaitBits(keyctrlEventGroup, KEYCTRL_EVENT, pdTRUE, pdFALSE, 10);

        if ((uxBits & EVT_KEY_W) != 0) {    //if 'W pressed' detected
            // Turn on LED
			BRD_LEDGreenToggle();
            debug_log("Move UP!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_W);
		} else if ((uxBits & EVT_KEY_A) != 0) {     //if 'A pressed' detected

            debug_log("Move LEFT!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_A);
        } else if ((uxBits & EVT_KEY_S) != 0) {     //if 'S pressed' detected
            
            debug_log("Move DOWN!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_S);
        } else if ((uxBits & EVT_KEY_D) != 0) {     //if 'D pressed' detected
            
            debug_log("Move RIGHT!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_D);
        } else if ((uxBits & EVT_KEY_X) != 0) {     //if 'X pressed' detected
            
            debug_log("Select Cell!\n\r");
            cellSelect = 1;   //set the flag here for further implement
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_X);
        } else if ((uxBits & EVT_KEY_Z) != 0) {     //if 'Z pressed' detected
            
            debug_log("Unselect Cell!\n\r");
            cellSelect = 0;
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_Z);
        } else if ((uxBits & EVT_KEY_P) != 0) {     //if 'P pressed' detected
            
            debug_log("Toggle Start/Stop!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_P);
        } else if ((uxBits & EVT_KEY_O) != 0) {     //if 'O pressed' detected
            
            debug_log("Move to Origin!\n\r");
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_O);
        } else if ((uxBits & EVT_KEY_C) != 0) {     //if 'C pressed' detected
            
            debug_log("Clear Display!\n\r");
            grid[16][64] = 0;
			uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_C);
        }


        xActivatedMember = xQueueSelectFromSet(xQueueSet, 20);

        //Receives inputs from DT3 (Grid) test
        //The Blinker model is used for testing here, will implement further in mnemonic task instead
        //if (xActivatedMember == simulatorMsgQ) { 
                      
            if (xQueueReceive(simulatorMsgQ, &msgFromGrid, 10)) {

                if (msgFromGrid.type == ALIVE_CELL) {
                    //BRD_LEDGreenOn();  //test if received for now

                } else if (msgFromGrid.type == BLINKER_OSCILLATOR) {

                    BRD_LEDBlueToggle();  //INDICATES the simulator is receiving from grid successfully
                    int xIndex = msgFromGrid.cell_x;
                    int yIndex = msgFromGrid.cell_y;
                    for (int a = xIndex; a <= xIndex + 2; a++) {
                        grid[yIndex][a] = 1;
                    }
                }
            }
        //}
        
        vTaskDelay(1);
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
