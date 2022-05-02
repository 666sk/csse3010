 /** 
 **************************************************************
 * @file mylib/s457527_oled.h
 * @author Kuang Sheng - 45752720
 * @date 03/05/2022
 * @brief mylib oled driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_oled_init() - Initialise the OLED peripheral
 * s4575272TaskOLED() - OLED task
 * s4575272_tsk_oled_init() - Create the OLED task
 *************************************************************** 
 */

#ifndef __S4575272_OLED_H__
#define __S4575272_OLED_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define I2C_DEV_SDA_PIN		9
#define I2C_DEV_SCL_PIN		8
#define I2C_DEV_GPIO		GPIOB
#define I2C_DEV_GPIO_AF 	GPIO_AF4_I2C1
#define I2C_DEV_GPIO_CLK()	__GPIOB_CLK_ENABLE()

#define I2C_DEV				I2C1
#define I2C_DEV_CLOCKSPEED 	100000

// Task Priorities (Idle Priority is the lowest priority)
#define OLEDTASK_PRIORITY		( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define OLEDTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )

void s4575272_reg_oled_init(void);
void s4575272TaskOLED(void);
void s4575272_tsk_oled_init(void);

QueueHandle_t oledMsgQ;

struct oledTextMsg {
	int startX;
	int startY;
	char displayText[20];
};


#endif