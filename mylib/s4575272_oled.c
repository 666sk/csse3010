 /** 
 **************************************************************
 * @file mylib/s457527_oled.c
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


#include "s4575272_oled.h"
#include "board.h"
#include "processor_hal.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


/**
 * Initialise the OLED SSD1306 device with the I2C interface
 */
void s4575272_reg_oled_init(void) {

    uint32_t pclk1;
	uint32_t freqrange;

	BRD_LEDInit();	//Initialise LEDs

	// Turn off LEDs
	//BRD_LEDGreenOff();

	// Enable GPIO clock
	I2C_DEV_GPIO_CLK();

	//******************************************************
	// IMPORTANT NOTE: SCL Must be Initialised BEFORE SDA
	//******************************************************

	//Clear and Set Alternate Function for pin (lower ARF register) 
	MODIFY_REG(I2C_DEV_GPIO->AFR[1], ((0x0F) << ((I2C_DEV_SCL_PIN-8) * 4)) | ((0x0F) << ((I2C_DEV_SDA_PIN-8)* 4)), ((I2C_DEV_GPIO_AF << ((I2C_DEV_SCL_PIN-8) * 4)) | (I2C_DEV_GPIO_AF << ((I2C_DEV_SDA_PIN-8)) * 4)));
	
	//Clear and Set Alternate Function Push Pull Mode
	MODIFY_REG(I2C_DEV_GPIO->MODER, ((0x03 << (I2C_DEV_SCL_PIN * 2)) | (0x03 << (I2C_DEV_SDA_PIN * 2))), ((GPIO_MODE_AF_OD << (I2C_DEV_SCL_PIN * 2)) | (GPIO_MODE_AF_OD << (I2C_DEV_SDA_PIN * 2))));
	
	//Set low speed.
	SET_BIT(I2C_DEV_GPIO->OSPEEDR, (GPIO_SPEED_LOW << I2C_DEV_SCL_PIN) | (GPIO_SPEED_LOW << I2C_DEV_SDA_PIN));

	//Set Bit for Push/Pull output
	SET_BIT(I2C_DEV_GPIO->OTYPER, ((0x01 << I2C_DEV_SCL_PIN) | (0x01 << I2C_DEV_SDA_PIN)));

	//Clear and set bits for no push/pull
	MODIFY_REG(I2C_DEV_GPIO->PUPDR, (0x03 << (I2C_DEV_SCL_PIN * 2)) | (0x03 << (I2C_DEV_SDA_PIN * 2)), (GPIO_PULLUP << (I2C_DEV_SCL_PIN * 2)) | (GPIO_PULLUP << (I2C_DEV_SDA_PIN * 2)));

	// Configure the I2C peripheral
	// Enable I2C peripheral clock
	__I2C1_CLK_ENABLE();

	// Disable the selected I2C peripheral
	CLEAR_BIT(I2C_DEV->CR1, I2C_CR1_PE);

  	pclk1 = HAL_RCC_GetPCLK1Freq();			// Get PCLK1 frequency
  	freqrange = I2C_FREQRANGE(pclk1);		// Calculate frequency range 

  	//I2Cx CR2 Configuration - Configure I2Cx: Frequency range
  	MODIFY_REG(I2C_DEV->CR2, I2C_CR2_FREQ, freqrange);

	// I2Cx TRISE Configuration - Configure I2Cx: Rise Time
  	MODIFY_REG(I2C_DEV->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, I2C_DEV_CLOCKSPEED));

   	// I2Cx CCR Configuration - Configure I2Cx: Speed
  	MODIFY_REG(I2C_DEV->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, I2C_DEV_CLOCKSPEED, I2C_DUTYCYCLE_2));

   	// I2Cx CR1 Configuration - Configure I2Cx: Generalcall and NoStretch mode
  	MODIFY_REG(I2C_DEV->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (I2C_GENERALCALL_DISABLE| I2C_NOSTRETCH_DISABLE));

   	// I2Cx OAR1 Configuration - Configure I2Cx: Own Address1 and addressing mode
  	MODIFY_REG(I2C_DEV->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), I2C_ADDRESSINGMODE_7BIT);

   	// I2Cx OAR2 Configuration - Configure I2Cx: Dual mode and Own Address2
  	MODIFY_REG(I2C_DEV->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), I2C_DUALADDRESS_DISABLE);

  	// Enable the selected I2C peripheral
	SET_BIT(I2C_DEV->CR1, I2C_CR1_PE);

	ssd1306_Init();	//Initialise SSD1306 OLED.
}


//The OLED Task itself
void s4575272TaskOLED(void) {

    s4575272_reg_oled_init();
	uint8_t read_reg_val;
	uint32_t status;
	
	struct oledTextMsg textMsgRecv;
	char displayText[50] = {0};
	oledMsgQ = xQueueCreate(10, sizeof(displayText));


	// Cyclic Executive (CE) loop
	for (;;) {

        ssd1306_Fill(Black);    //Clear Screen

		//Show text and udpate screen
		ssd1306_SetCursor(10,12);
		if (xQueueReceive(oledMsgQ, &textMsgRecv, 10)) {
			ssd1306_WriteString(textMsgRecv.displayText, Font_6x8, SSD1306_WHITE);
			ssd1306_UpdateScreen();
			//vTaskDelay(20);
		}
		
	}
}


/*
* Create OLED task
*/
void s4575272_tsk_oled_init(void) {
	portDISABLE_INTERRUPTS();
	
    xTaskCreate(
        (void *) &s4575272TaskOLED,     // Function that implements the task
        (const signed char *) "OLEDTask",   // Text name for the task
        OLEDTASK_STACK_SIZE * 6,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        OLEDTASK_PRIORITY + 1,              // Priority at which the task is created
        NULL);                          // Used to pass out the created task's handle

	portENABLE_INTERRUPTS();

}