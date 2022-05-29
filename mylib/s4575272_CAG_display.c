/** 
 **************************************************************
 * @file mylib/s457527_CAG_display.c
 * @author Kuang Sheng - 45752720
 * @date 11/05/2022
 * @brief mylib CAG display driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_tsk_CAG_display_init(void) - Initialise the CAG Display task
 * s4575272TaskCAG_Display(void) - The task of displaying the simulation
 *************************************************************** 
 */

#include "s4575272_CAG_display.h"
#include "s4575272_oled.h"
#include "s4575272_CAG_simulator.h"
//Initialise the CAG Display task
void s4575272_tsk_CAG_display_init(void) {
	portDISABLE_INTERRUPTS();
	
    xTaskCreate(
        (void *) &s4575272TaskCAG_Display,     // Function that implements the task
        (const signed char *) "TaskCAG_Display",   // Text name for the task
        OLEDTASK_STACK_SIZE * 5,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        OLEDTASK_PRIORITY+1,              // Priority at which the task is created
        NULL);                          // Used to pass out the created task's handle
	portENABLE_INTERRUPTS();
}


//The task of displaying the simulation
void s4575272TaskCAG_Display(void) {
    s4575272_reg_oled_init();

    caMessage_t msgFromSimulator;
    displaySemaphore = xSemaphoreCreateBinary(); 
    //simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));
    xQueueAddToSet(displaySemaphore, xQueueSet);


    for (;;) {
        //vPortEnterCritical();
        ssd1306_Fill(Black);    //Clear Screen
        //BRD_LEDRedOn();  //indicates display working for now
        vTaskSuspend(taskSim);
        for (int i = 0; i < 16; i++) {

            for (int j = 0; j < 64; j++) {
            
                if (grid[i][j]) {  //if there is a cell alive display it as 2x2 pixel
                    
                    ssd1306_DrawPixel(2 * j, 2 * i, SSD1306_WHITE);
                    ssd1306_DrawPixel(2 * j, 2 * i + 1, SSD1306_WHITE);
                    ssd1306_DrawPixel(2 * j + 1, 2 * i, SSD1306_WHITE);
                    ssd1306_DrawPixel(2 * j + 1, 2 * i + 1, SSD1306_WHITE);
                }
            }
        }
        ssd1306_UpdateScreen();
    
        vTaskResume(taskSim);
		vTaskDelay(10);
	}
}

