 /** 
 **************************************************************
 * @file mylib/s457527_CAG_simulator.h
 * @author Kuang Sheng - 45752720
 * @date 11/05/2022
 * @brief mylib CAG simulator driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */

#ifndef __S4575272_CAG_simulator_H__
#define __S4575272_CAG_simulator_H__

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define DEAD_CELL 0x10
#define ALIVE_CELL 0x11
#define BLOCK_STILL_LIFE 0x20
#define BEEHIVE_STILL_LIFE 0x21
#define LOAF_STILL_LIFE 0x22
#define BLINKER_OSCILLATOR 0x30
#define TOAD_OSCILLATOR 0x31
#define BEACON_OSCILLATOR 0x32
#define GLIDER_SPACESHIP 0x40

#define CAG_SIMULATOR_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 8 )
#define CAG_SIMULATOR_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )

typedef struct caMessage {
    
    int type;
    int cell_x;
    int cell_y;
} caMessage_t;

QueueHandle_t simulatorMsgQ;

int grid[16][64];


void s4575272_tsk_CAG_simulator_init(void);
void s4575272TaskCAG_Simulator(void);
int nbr_count(int grid[16][64], int i, int j);

#endif