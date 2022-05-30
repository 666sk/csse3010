 /** 
 **************************************************************
 * @file mylib/s457527_CAG_display.h
 * @author Kuang Sheng - 45752720
 * @date 11/05/2022
 * @brief mylib CAG display driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_oled_init(void); - Initialise the oled peripheral
 * s4575272_tsk_CAG_display_init(void) - Initialise the CAG Display task
 * s4575272TaskCAG_Display(void) - The task of displaying the simulation
 *************************************************************** 
 */

#ifndef __S4575272_CAG_display_H__
#define __S4575272_CAG_display_H__
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"
#include "s4575272_oled.h"
#include "s4575272_CAG_simulator.h"
#define CAG_OLED_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 10 )
#define CAG_OLED_TASK_PRIORITY        ( tskIDLE_PRIORITY + 3 )
QueueHandle_t displayMsgQ;

void s4575272_reg_oled_init(void);
void s4575272_tsk_CAG_display_init(void);
void s4575272TaskCAG_Display(void);

#endif
