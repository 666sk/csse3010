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
        CAG_OLED_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_OLED_TASK_PRIORITY,              // Priority at which the task is created
        NULL);                          // Used to pass out the created task's handle
	portENABLE_INTERRUPTS();
}


//The task of displaying the simulation
void s4575272TaskCAG_Display(void) {
    s4575272_reg_oled_init();

    caMessage_t msgFromSimulator;
    displaySemaphore = xSemaphoreCreateBinary(); 


    for (;;) {

            ssd1306_Fill(Black);    //Clear Screen
            if (taskSim != NULL) {
                vTaskSuspend(taskSim);
            }
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
            
            if (taskSim != NULL) {
                vTaskResume(taskSim);
            }
            
            vTaskDelay(10);
        
	}
}
