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
    hardware_init();  //pb 

    grid[16][64] = 0;
    //memset grid
    //for (int i = 0; i < 16; i++) {
    //    for (int j = 0; j < 64; j++) {
    //        grid[i][j] = 0;
    //    }
    //}

    for (int a = 30; a < 33; a++) {
        grid[12][a] = 1;
    }
    
    int nbr_grid[16][64];   
    nbr_grid[16][64] = 0;
    //int nbr_grid[16][64] = {0};


    static uint32_t prevTime = 0;
    uint8_t x, y;  //grid coordinates
    uint8_t xIndex = 0; 
    uint8_t yIndex = 0;
    uint8_t cellSelect = 0;   //1 if select the cell
    uint8_t start = 1;   //0 stop 1 start
    EventBits_t uxBits;
    keyctrlEventGroup = xEventGroupCreate();

    

    caMessage_t msgFromMnem;
  
    simulatorMsgQ = xQueueCreate(10, sizeof(msgFromMnem));

   

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
        } else if ((uxBits & EVT_KEY_C) != 0) {     //if 'C pressed' detected
            
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
            uxBits = xEventGroupClearBits(keyctrlEventGroup, EVT_KEY_Z);
        }

                      
            //Queue receiving from mnemonic
        if (xQueueReceive(simulatorMsgQ, &msgFromMnem, 10)) {

            if (msgFromMnem.type == BLOCK_STILL_LIFE) {

                drawBlock(&msgFromMnem);
            } else if (msgFromMnem.type == BEEHIVE_STILL_LIFE) {
                
                drawBeehive(&msgFromMnem);
            } else if (msgFromMnem.type == LOAF_STILL_LIFE) {

                drawLoaf(&msgFromMnem);
            } else if (msgFromMnem.type == BLINKER_OSCILLATOR) {

                drawBlinker(&msgFromMnem);
            } else if (msgFromMnem.type == TOAD_OSCILLATOR) {

                drawToad(&msgFromMnem);
            } else if (msgFromMnem.type == BEACON_OSCILLATOR) {

                drawBeacon(&msgFromMnem);
            } else if (msgFromMnem.type == GLIDER_SPACESHIP) {

                drawGlider(&msgFromMnem);
            } else if (msgFromMnem.type == START) {
                debug_log("start Received: x=%d, y=%d\n\r",msgFromMnem.cell_x,msgFromMnem.cell_y);
                start = 1;
            } else if (msgFromMnem.type == STOP) {
                debug_log("stop Received: x=%d, y=%d\n\r",msgFromMnem.cell_x,msgFromMnem.cell_y);
                start = 0;
            } else if (msgFromMnem.type == CLEAR) {

                debug_log("stop Received: x=%d, y=%d\n\r",msgFromMnem.cell_x,msgFromMnem.cell_y);
                clearGrid();
            }

        }

                
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



void drawBeacon(caMessage_t* msgFromMnem) {

    debug_log("Beacon Received: x=%d, y=%d\n\r",msgFromMnem->cell_x, msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x] = 1; 
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x+3] = 1;
    grid[msgFromMnem->cell_y+3][msgFromMnem->cell_x+3] = 1;
    grid[msgFromMnem->cell_y+3][msgFromMnem->cell_x+2] = 1; 
}

void drawBlinker(caMessage_t* msgFromMnem) {

    debug_log("Blinker Received: x=%d, y=%d\n\r",msgFromMnem->cell_x,msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x + 1] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x + 2] = 1;   
}

void drawToad(caMessage_t* msgFromMnem) {

    debug_log("Toad Received: x=%d, y=%d\n\r",msgFromMnem->cell_x,msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x + 1] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x + 2] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x + 3] = 1; 
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x + 1] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x + 2] = 1; 
}

void drawBlock(caMessage_t* msgFromMnem) {

    debug_log("Block Received: x=%d, y=%d\n\r",msgFromMnem->cell_x, msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x] = 1; 
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x+1] = 1;
}

void drawBeehive(caMessage_t* msgFromMnem) {

    debug_log("Beehive Received: x=%d, y=%d\n\r",msgFromMnem->cell_x, msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x+2] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x] = 1; 
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x+3] = 1;
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x+2] = 1;
}

void drawLoaf(caMessage_t* msgFromMnem) {

    debug_log("Loaf Received: x=%d, y=%d\n\r",msgFromMnem->cell_x, msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x+2] = 1; 
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x+3] = 1;
    grid[msgFromMnem->cell_y+3][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+3][msgFromMnem->cell_x+2] = 1;
}

void drawGlider(caMessage_t* msgFromMnem) {

    debug_log("Glider Received: x=%d, y=%d\n\r",msgFromMnem->cell_x, msgFromMnem->cell_y);
    grid[msgFromMnem->cell_y][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+1][msgFromMnem->cell_x+2] = 1;
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x] = 1;
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x+1] = 1;
    grid[msgFromMnem->cell_y+2][msgFromMnem->cell_x+2] = 1;
}

void clearGrid(void) {
    for (int i = 0; i < 16; i++) {
       for (int j = 0; j < 64; j++) {
           grid[i][j] = 0;
       }
    }
}

/*
 * Hardware Initialisation.
 */
void hardware_init() {

	portDISABLE_INTERRUPTS();	//Disable interrupts
	// Enable GPIOC Clock
	__GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode
	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13) on EXTICR4.
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->RTSR |= EXTI_RTSR_TR13;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR13;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM13;		//Enable external interrupt
	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	portENABLE_INTERRUPTS();	//Enable interrupts
}

/*
 * Pushbutton callback function
 */
void pb_callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == 13) {

		// Is it time for another Task() to run?
		xHigherPriorityTaskWoken = pdFALSE;

        //mode = 1 - mode; //toggle mode
		if (pbSemaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR(pbSemaphore, &xHigherPriorityTaskWoken );		// Give PB Semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

/*
 * Interrupt handler for EXTI 15 to 10 IRQ Handler
 */ 
void EXTI15_10_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

	// PR: Pending register
	if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) {

		// cleared by writing a 1 to this bit
		EXTI->PR |= EXTI_PR_PR13;	//Clear interrupt flag.
		pb_callback(13);   // Callback for C13
	}
}