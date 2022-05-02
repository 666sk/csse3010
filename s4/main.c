/**
  ******************************************************************************
  * @file    stage4/main.c
  * @author  Kuang Sheng
  * @date    03/05/2022
  * @brief   Nucleo429ZI onboard stage4 design task. 
  * Created a dual timer using freeRTOS.
  ******************************************************************************
  */

#include "FreeRTOS.h"
#include "board.h"
#include "processor_hal.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4575272_oled.h"


void TaskTimerLeft(void);
void TaskTimerRight(void);
void TaskTimerDisplay(void);
void s4575272TaskOLED(void);
void s4575272_tsk_joystick_init(void);
void greenled(void);

struct dualTimerMsg {
	char type;  //left or right
	unsigned short timerValue; //data
};

QueueHandle_t queueTimerDisplay;   // Queue used
QueueHandle_t sendToLeftQ;
QueueHandle_t sendToRightQ;


#define TIMERTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 6 )
#define TIMERTASK_PRIORITY  ( tskIDLE_PRIORITY + 2 )

#define TIMERDISPLAY_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 6 )
#define TIMERDISPLAY_PRIORITY  ( tskIDLE_PRIORITY + 2 )

int main(void) {

	HAL_Init();

	s4575272_tsk_oled_init();  //Initialise the OLED task lowest priority
	
	s4575272_tsk_joystick_init();  //Initialise the joystick task highest priority
	
	xTaskCreate( (void *) &TaskTimerLeft, (const signed char *) "LeftTimer", TIMERTASK_STACK_SIZE, NULL, TIMERTASK_PRIORITY + 3, NULL );

	xTaskCreate( (void *) &TaskTimerRight, (const signed char *) "RightTimer", TIMERTASK_STACK_SIZE, NULL, TIMERTASK_PRIORITY + 2, NULL );

	xTaskCreate( (void *) &TaskTimerDisplay, (const signed char *) "TimerDisplay", TIMERDISPLAY_STACK_SIZE, NULL, TIMERDISPLAY_PRIORITY + 1, NULL );
	
	vTaskStartScheduler();

	return 0;
}

//Task of left timer - count every 1s
void TaskTimerLeft(void) {

	BRD_LEDInit();
	// BRD_LEDBlueOn();
	char type;
	unsigned short timerValue;
	uint8_t mode = 0;
	uint8_t pb = 0;
	struct dualTimerMsg leftTimerMsg;

	queueTimerDisplay = xQueueCreate(10, sizeof(leftTimerMsg));

	type = 'l';
	timerValue = 0;

	for (;;) {

		if (queueTimerDisplay != NULL) {
			leftTimerMsg.type = type;
			leftTimerMsg.timerValue = timerValue;
			xQueueSendToFront(queueTimerDisplay, ( void * ) &leftTimerMsg, ( portTickType ) 10 );
		}

		if (sendToLeftQ != NULL) {
			if (xQueueReceive(sendToLeftQ, &pb, 10)) {
				mode = ~mode & 0x01;  //toggle the mode flag
				
			}
		}

		if (mode == 0) {
			timerValue++;
		}

		if (timerValue >= 999) {
			timerValue = 0;
		}
		vTaskDelay(1000);
		

	}
	
	
}

//Task of right timer - count every 10ms
void TaskTimerRight(void) {

	char type;
	unsigned short timerValue;
	struct dualTimerMsg rightTimerMsg;
	uint8_t mode = 0;
	uint8_t pb = 0;

	queueTimerDisplay = xQueueCreate(10, sizeof(rightTimerMsg));
	
	type = 'r';
	timerValue = 0;

	for (;;) {

		BRD_LEDGreenOn();
		if (queueTimerDisplay != NULL) {
			rightTimerMsg.type = type;
			rightTimerMsg.timerValue = timerValue;
			xQueueSendToFront(queueTimerDisplay, ( void * ) &rightTimerMsg, ( portTickType ) 10 );
		}

		//timerValue++;

		if (sendToRightQ != NULL) {
			if (xQueueReceive(sendToRightQ, &pb, 10)) {
				mode = ~mode & 0x01;
				//BRD_LEDBlueOn();
				
			}
		}

		if (mode == 0) {
			timerValue++;
			
		} else {
			BRD_LEDBlueToggle();
		}

		if (timerValue >= 99) {
			timerValue = 0;
		}
		vTaskDelay(10);

	}

}

//Task of displaying timer values
void TaskTimerDisplay(void) {
	
	char type;
	unsigned short leftTimerValue = 0;
	unsigned short rightTimerValue = 0;
	char leftTimerValueStr[5] = {0};
	char rightTimerValueStr[5] = {0};
	char displayText[50] = {0};
	int start = 0;

	struct dualTimerMsg timerDisplay;
	struct oledTextMsg textMsg;
	
	oledMsgQ = xQueueCreate(10, sizeof(displayText));
	
	for (;;) {
		
		if (queueTimerDisplay != NULL) {
			
			if (xQueueReceive(queueTimerDisplay, &timerDisplay, 10)) {
				type = timerDisplay.type;

				if (type == 'l') {
					
					leftTimerValue = timerDisplay.timerValue;
					itoa(leftTimerValue, leftTimerValueStr, 10);
				} else if (type == 'r') {
					rightTimerValue = timerDisplay.timerValue;
					itoa(rightTimerValue, rightTimerValueStr, 10);
					
				}
				if (start == 0) {
					strcpy(leftTimerValueStr, "0");
					start++;
				}
				//BRD_LEDRedOn();
				strcpy(displayText, leftTimerValueStr);
				strcat(displayText, ":");
				strcat(displayText, rightTimerValueStr);
				strcpy(textMsg.displayText, displayText);
				xQueueSendToFront(oledMsgQ, ( void * ) &textMsg, ( portTickType ) 10 );

			}

		}
		vTaskDelay(1);
		
	}
	
}
