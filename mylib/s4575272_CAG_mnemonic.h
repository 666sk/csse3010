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
 *
 *************************************************************** 
 */


#ifndef __S4575272_CAG_mnemonic_H__
#define __S4575272_CAG_mnemonic_H__

#include "s4575272_CAG_simulator.h"
#include "s4575272_CAG_joystick.h"
#include <stdlib.h>
#include "string.h"

#define CAG_MNEMONIC_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 10 )
#define CAG_MNEMONIC_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )

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