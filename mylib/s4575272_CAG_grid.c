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
 * s4575272_tsk_CAG_grid_init(void) - Initialize the task of grid
 * s4575272TaskCAG_Grid(void) - The task of grid mode
 *************************************************************** 
 */

#include "s4575272_CAG_grid.h"


//Initialize the task of grid
void s4575272_tsk_CAG_grid_init(void) {
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Grid,     // Function that implements the task
        (const signed char *) "TaskCAG_Grid",   // Text name for the task
        CAG_GRID_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_GRID_TASK_PRIORITY,              // Priority at which the task is created
        NULL);  
}

//The task of grid mode
void s4575272TaskCAG_Grid(void) {
    BRD_LEDInit();
    BRD_debuguart_init();
    s4575272_reg_lta1000g_init();

    //Event group
    EventBits_t uxBits;
    EventBits_t uxBits1;
    keyctrlEventGroup = xEventGroupCreate();
    
    //Queue
    caMessage_t msgToSimulator;
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));

    
    char recvChar;   //Receiving chars from keyboard
    uint8_t xIndex = 0; 
    uint8_t yIndex = 0;   //indicates the current X and Y values


    for (;;) {
        
        if ((recvChar = BRD_debuguart_getc()) != '\0') {

            if (recvChar == 'W') {

                if (yIndex) {
                    yIndex--;
                }
                debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_W);

            } else if (recvChar == 'A') {

                if (xIndex) {
                    xIndex--;
                }
                debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_A);

            } else if (recvChar == 'S') {

                if (yIndex < 15) {
                    yIndex++;
                }
                debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_S);

            } else if (recvChar == 'D') {

                if (xIndex < 63) {
                    xIndex++;
                }
                debug_log("Current location is (%d, %d)!\n\r", xIndex, yIndex);
                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_D);

            } else if (recvChar == 'X') {

                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_X);
            } else if (recvChar == 'Z') {

                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_Z);
            } else if (recvChar == 'P') {

                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_P);
            } else if (recvChar == 'O') {

                xIndex = 0;
                yIndex = 0;
                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_O);
            } else if (recvChar == 'C') {

                uxBits = xEventGroupSetBits(keyctrlEventGroup, EVT_KEY_C);
            }

            s4575272_reg_lta1000g_init_write(16 * xIndex + yIndex);   //indicates the location in LED Bar
            
        }


        //Doing queue stuff

        //Send a Blinker model to simulator for testing atm
        //Will implement the model in Mnemonic task instead of grid task
        if (simulatorMsgQ != NULL) {

            //BRD_LEDBlueOn();
            msgToSimulator.cell_x = xIndex;
            msgToSimulator.cell_y = yIndex;
            msgToSimulator.type = BLINKER_OSCILLATOR;
            xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
        }

        vTaskDelay(50);
    }

}