 /** 
 **************************************************************
 * @file mylib/s457527_CAG_menmonic.c
 * @author Kuang Sheng - 45752720
 * @date 15/05/2022
 * @brief mylib CAG mnemonic driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * void s4575272_tsk_CAG_mnemonic_init(void); - Initialise the mnemonic mode task
 * void s4575272TaskCAG_Mnemonic(void); - The task of mnemonic mode
 * void sendClear(caMessage_t* msgToSimulator); - Send clear command
 * void sendStill(caMessage_t* msgToSimulator, char* pcOutputString); - Send Still command
 * void sendOsc(caMessage_t* msgToSimulator, char* pcOutputString); - Send Osc command
 * void sendGlider(caMessage_t* msgToSimulator, char* pcOutputString); - Send glider command
 * void sendStart(caMessage_t* msgToSimulator); - Send start command
 * void sendStop(caMessage_t* msgToSimulator); - Send stop command
 * void sendDel(caMessage_t* msgToSimulator, char* pcOutputString); - Send Del command
 * void driverCre(caMessage_t* msgToSimulator, char* pcOutputString); - Send Cre commans
 *************************************************************** 
 */

#include "s4575272_CAG_mnemonic.h"
#include "s4575272_cli_CAG_mnemonic.h"

//Initialize the task of mnemonic mode
void s4575272_tsk_CAG_mnemonic_init(void) {

	//Register CLI commands
    FreeRTOS_CLIRegisterCommand(&xStill);
	FreeRTOS_CLIRegisterCommand(&xOsc);
	FreeRTOS_CLIRegisterCommand(&xGlider);
	FreeRTOS_CLIRegisterCommand(&xStart);
	FreeRTOS_CLIRegisterCommand(&xStop);
	FreeRTOS_CLIRegisterCommand(&xClear);
	FreeRTOS_CLIRegisterCommand(&xDel);
	FreeRTOS_CLIRegisterCommand(&xCre);
	FreeRTOS_CLIRegisterCommand(&xSystem);
	FreeRTOS_CLIRegisterCommand(&xUsage);
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Mnemonic,     // Function that implements the task
        (const signed char *) "TaskCAG_Mnemonic",   // Text name for the task
        CAG_MNEMONIC_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_MNEMONIC_TASK_PRIORITY+1,              // Priority at which the task is created
        &taskMnem);  
}

//The task of mnemonic mode
void s4575272TaskCAG_Mnemonic(void) {
	BRD_LEDInit();
	BRD_debuguart_init();
	
    int i;
	char cRxedChar;
	char cInputString[100];
	int InputIndex = 0;
	char *pcOutputString;
	BaseType_t xReturned;
	
	//Queue
	caMessage_t msgToSimulator;
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));

	//Semaphore
	pbSemaphore = xSemaphoreCreateBinary();	

	/* Initialise pointer to CLI output buffer. */
	memset(cInputString, 0, sizeof(cInputString));
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	//Suspend ourselves in the begining until switching to mnemonic mode
	vTaskSuspend(NULL);

	for (;;) {
		BRD_LEDGreenOff();

		if (pbSemaphore != NULL) {	
            if ( xSemaphoreTake(pbSemaphore, 10 ) == pdTRUE ) {
                
                vTaskResume(taskGrid);
                vTaskSuspend(NULL);
            }
        }
			
		/* Receive character from terminal */
		cRxedChar = debug_getc();

		/* Process if character if not Null */
		if (cRxedChar != '\0') {

			/* Echo character */
			debug_putc(cRxedChar);   //not fflush yet
			/* Process only if return is received. */
			if (cRxedChar == '\r') {

				//Put new line and transmit buffer
				debug_putc('\n');
				debug_flush();

				/* Put null character in command input string. */
				cInputString[InputIndex] = '\0';

				xReturned = pdTRUE;
				/* Process command input string. */
				while (xReturned != pdFALSE) {

					/* Returns pdFALSE, when all strings have been returned */
					xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

					//debug_log("pcOutputString is %s, cInputString is %s\n\r", pcOutputString, cInputString);
					/* Display CLI command output string (not thread safe) */
					//Doing queue stuff
					
						portENTER_CRITICAL();
						if ((*(cInputString) == 'h') || (*(pcOutputString) == 'I')) {
							for (i = 0; i < (int) strlen(pcOutputString); i++) {
								debug_putc(*(pcOutputString + i));
							}
						}
						portEXIT_CRITICAL();
					

					if (simulatorMsgQ != NULL) {

						if (*(pcOutputString) == '1') {  //still
							sendStill(&msgToSimulator, pcOutputString);
							if (taskSim != NULL) {
								xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
							}
						} else if (*(pcOutputString) == '2') {  //osc
							
							sendOsc(&msgToSimulator, pcOutputString);
							if (taskSim != NULL) {
								xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
							}
						} else if (*(pcOutputString) == '3') {     //glider
							
							sendGlider(&msgToSimulator, pcOutputString);
							if (taskSim != NULL) {
								xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
							}
						} else if (*(pcOutputString) == '4') {    //start
							sendStart(&msgToSimulator);
							if (taskSim != NULL) {
								xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
							}
						} else if (*(pcOutputString) == '5') {    //stop
							sendStop(&msgToSimulator);
							if (taskSim != NULL) {
								xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
							}
						} else if (*(pcOutputString) == '6') {    //clear
							sendClear(&msgToSimulator);
							if (taskSim != NULL) {
								xQueueSendToFront(simulatorMsgQ, ( void * ) &msgToSimulator, ( portTickType ) 10 );
							}
						} else if (*(pcOutputString) == '7') {    //del
							
							sendDel(&msgToSimulator, pcOutputString);
						} else if (*(pcOutputString) == '8') {    //cre
							driverCre(&msgToSimulator, pcOutputString);
						} else if (*(pcOutputString) == '9') {    //system

							debug_log("The system time is %d ms\n\r", HAL_GetTick());
						} else if (*(pcOutputString) == '0') {    //usage

							char pWriteBuffer[2048];
							vTaskList((char *)&pWriteBuffer);
							debug_log("Task_Name  Task_State Priority Stack  Number\n\r");
							debug_log("%s\n\r", pWriteBuffer);
						}	
					}
					
				    vTaskDelay(5);	//Must delay between debug_printfs.
				}


				memset(cInputString, 0, sizeof(cInputString));
				InputIndex = 0;

			} else {

				debug_flush();		//Transmit USB buffer
				if( cRxedChar == '\r' ) {

					/* Ignore the character. */
				} else if( cRxedChar == '\b' ) {

					/* Backspace was pressed.  Erase the last character in the
					 string - if any.*/
					if( InputIndex > 0 ) {
						InputIndex--;
						cInputString[ InputIndex ] = '\0';
					}

				} else {

					/* A character was entered.  Add it to the string
					   entered so far.  When a \n is entered the complete
					   string will be passed to the command interpreter. */
					if( InputIndex < 20 ) {
						cInputString[ InputIndex ] = cRxedChar;
						InputIndex++;
					}
				}
			}
		}

		vTaskDelay(50);	
	}
}

//Obtain the basic info of still command sending to simulator
void sendStill(caMessage_t* msgToSimulator, char* pcOutputString) {

	char* info;
	int type;
	info = strtok(pcOutputString, "<");
	info = strtok(NULL, "<");
	type = *info - 48;

	if (type == 0) {
		msgToSimulator->type = BLOCK_STILL_LIFE;
	} else if (type == 1) {
		msgToSimulator->type = BEEHIVE_STILL_LIFE;
	} else if (type == 2) {
		msgToSimulator->type = LOAF_STILL_LIFE;
	} else {
		msgToSimulator->type = NONE;
	}

	info = strtok(NULL, "<");
	msgToSimulator->cell_x = atoi(info);
	info = strtok(NULL, "<");							
	msgToSimulator->cell_y = atoi(info);
}

//Obtain the basic info of osc command sending to simulator
void sendOsc(caMessage_t* msgToSimulator, char* pcOutputString) {

	char* info;
	int type;
	info = strtok(pcOutputString, "<");
	info = strtok(NULL, "<");
	type = *info - 48;
							
	if (type == 0) {
		msgToSimulator->type = BLINKER_OSCILLATOR;
	} else if (type == 1) {
		msgToSimulator->type = TOAD_OSCILLATOR;
	} else if (type == 2) {
		msgToSimulator->type = BEACON_OSCILLATOR;
	} else {
		msgToSimulator->type = NONE;
	}
	info = strtok(NULL, "<");
	msgToSimulator->cell_x = atoi(info);
	info = strtok(NULL, "<");
	msgToSimulator->cell_y = atoi(info);
}

//Obtain the basic info of glider command sending to simulator
void sendGlider(caMessage_t* msgToSimulator, char* pcOutputString) {

	msgToSimulator->type = GLIDER_SPACESHIP;
	char* info;
	info = strtok(pcOutputString, "<");
	info = strtok(NULL, "<");
	msgToSimulator->cell_x = atoi(info);
	info = strtok(NULL, "<");
	msgToSimulator->cell_y = atoi(info);
}

//Obtain the basic info of clear command sending to simulator
void sendClear(caMessage_t* msgToSimulator) {

	msgToSimulator->type = CLEAR;
	msgToSimulator->cell_x = 0;
	msgToSimulator->cell_y = 0;
}

//Obtain the basic info of start command sending to simulator
void sendStart(caMessage_t* msgToSimulator) {

	msgToSimulator->type = START;
	msgToSimulator->cell_x = 0;
	msgToSimulator->cell_y = 0;
}

//Obtain the basic info of stop command sending to simulator
void sendStop(caMessage_t* msgToSimulator) {

	msgToSimulator->type = STOP;
	msgToSimulator->cell_x = 0;
	msgToSimulator->cell_y = 0;
}

//Delete the specified driver
void sendDel(caMessage_t* msgToSimulator, char* pcOutputString) {

	char* info;
	int type;
	info = strtok(pcOutputString, "<");
	info = strtok(NULL, "<");
	type = *info - 48;
	if (type == 0) {

		TaskHandle_t xTask = taskSim;
		vTaskSuspendAll();
		if (taskSim != NULL) {
			taskSim = NULL;
        	vTaskDelete(xTask);
    	}
		xTaskResumeAll();
	} else if (type == 1) {

		TaskHandle_t xTask = taskJoystick;
		vTaskSuspendAll();
		if (taskJoystick != NULL) {
			taskJoystick = NULL;
        	vTaskDelete(xTask);
    	}
		xTaskResumeAll();
	}
	
}

//Create the specified driver
void driverCre(caMessage_t* msgToSimulator, char* pcOutputString) {

	char* info;
	int type;
	info = strtok(pcOutputString, "<");
	info = strtok(NULL, "<");
	type = *info - 48;
	if (type == 0) {
		vTaskSuspendAll();
		if (taskSim == NULL) {

			debug_log("SIMULATOR Task created\n\r");
        	xTaskCreate(
				(void *) &s4575272TaskCAG_Simulator,     // Function that implements the task
				(const signed char *) "CAGSimulatorTask",   // Text name for the task
				CAG_SIMULATOR_TASK_STACK_SIZE,            // Stack size in words, not bytes
				NULL,                           // No Parameter needed
				CAG_SIMULATOR_TASK_PRIORITY,              // Priority at which the task is created
				&taskSim);
		}
		xTaskResumeAll();
	} else if (type == 1) {

		vTaskSuspendAll();
		if (taskJoystick == NULL) {

			debug_log("Joystick Task created\n\r");
        	xTaskCreate(
				(void *) &s4575272TaskCAG_Joystick,     // Function that implements the task
				(const signed char *) "CAGJoystickTask",   // Text name for the task
				CAG_JOYSTICK_TASK_STACK_SIZE,            // Stack size in words, not bytes
				NULL,                           // No Parameter needed
				CAG_JOYSTICK_TASK_PRIORITY,              // Priority at which the task is created
				&taskJoystick);
			}
		xTaskResumeAll();
	}
	
}
