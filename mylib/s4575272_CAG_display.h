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
 * 
 *************************************************************** 
 */

#ifndef __S4575272_CAG_display_H__
#define __S4575272_CAG_display_H__

#include "board.h"
#include "processor_hal.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4575272_oled.h"


void s4575272_reg_oled_init(void);

#endif