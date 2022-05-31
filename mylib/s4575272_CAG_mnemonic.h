 /** 
 **************************************************************
 * @file mylib/s457527_CAG_menmonic.h
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


#ifndef __S4575272_CAG_mnemonic_H__
#define __S4575272_CAG_mnemonic_H__

#include "s4575272_CAG_simulator.h"
#include "s4575272_CAG_joystick.h"
#include <stdlib.h>
#include "string.h"

#define CAG_MNEMONIC_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 10 )
#define CAG_MNEMONIC_TASK_PRIORITY        ( tskIDLE_PRIORITY + 3 )

uint8_t mode;

void s4575272_tsk_CAG_mnemonic_init(void);
void s4575272TaskCAG_Mnemonic(void);
void sendClear(caMessage_t* msgToSimulator);
void sendStill(caMessage_t* msgToSimulator, char* pcOutputString);
void sendOsc(caMessage_t* msgToSimulator, char* pcOutputString);
void sendGlider(caMessage_t* msgToSimulator, char* pcOutputString);
void sendStart(caMessage_t* msgToSimulator);
void sendStop(caMessage_t* msgToSimulator);
void sendDel(caMessage_t* msgToSimulator, char* pcOutputString);
void driverCre(caMessage_t* msgToSimulator, char* pcOutputString);

#endif