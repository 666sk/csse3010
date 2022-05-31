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
        CAG_GRID_TASK_PRIORITY+3,              // Priority at which the task is created
        &taskGrid);  
}

//The task of grid mode
void s4575272TaskCAG_Grid(void) {
    BRD_LEDInit();
    //hardware_init();
    BRD_debuguart_init();
    s4575272_reg_lta1000g_init();

    //Event group
    EventBits_t uxBits;
    EventBits_t uxBits1;
    keyctrlEventGroup = xEventGroupCreate();
    
    //Queue
    caMessage_t msgToSimulator;
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));

    //Semaphore
    pbSemaphore = xSemaphoreCreateBinary();	

    
    char recvChar;   //Receiving chars from keyboard
    uint8_t xIndex = 0; 
    uint8_t yIndex = 0;   //indicates the current X and Y values used for LED Bar display cursor location
  
    for (;;) {

        BRD_LEDGreenOn();

        if (pbSemaphore != NULL) {	
            if ( xSemaphoreTake(pbSemaphore, 10 ) == pdTRUE ) {
                
                vTaskResume(taskMnem);
                vTaskSuspend(NULL);
            }
        }
        
       
        if (((recvChar = BRD_debuguart_getc()) != '\0')) {

            uxBits = recvCharHandler(recvChar, &keyctrlEventGroup, &xIndex, &yIndex);

            s4575272_reg_lta1000g_init_write(16 * xIndex + yIndex);   //indicates the location in LED Bar
            
        }

        vTaskDelay(100);
    }

}

//Process the received character and set event bits
EventBits_t recvCharHandler(char recvChar, EventGroupHandle_t *keyctrlEventGroup, uint8_t *xIndex, uint8_t *yIndex) {
    EventBits_t uxBits;

    if (recvChar == 'W') {

        if (*yIndex > 0) {
            *yIndex += (-1);
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_W);
    } else if (recvChar == 'A') {

        if (*xIndex > 0) {
            *xIndex += (-1);
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_A);
    } else if (recvChar == 'S') {

        if (*yIndex < 15) {
            *yIndex += 1;
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_S);
    } else if (recvChar == 'D') {

        if (*xIndex < 63) {
            *xIndex += 1;
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_D);
    } else if (recvChar == 'X') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_X);
    } else if (recvChar == 'Z') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_Z);
    } else if (recvChar == 'P') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_P);
    } else if (recvChar == 'O') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_O);
    } else if (recvChar == 'C') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_C);
    }
    return uxBits;
}
