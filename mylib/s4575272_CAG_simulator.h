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
 * s4575272_tsk_CAG_simulator_init(void) - The task of simulating the game of life
 * s4575272TaskCAG_Simulator(void) - Initialise the CAG Simulator task
 * nbr_count(int grid[16][64], int i, int j) - Count how many neighbours of a cell
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
#include "event_groups.h"
#include "debug_log.h"

#define DEAD_CELL 0x10
#define ALIVE_CELL 0x11
#define BLOCK_STILL_LIFE 0x20
#define BEEHIVE_STILL_LIFE 0x21
#define LOAF_STILL_LIFE 0x22
#define BLINKER_OSCILLATOR 0x30
#define TOAD_OSCILLATOR 0x31
#define BEACON_OSCILLATOR 0x32
#define GLIDER_SPACESHIP 0x40
#define NONE 0x00

#define EVT_KEY_W   1 << 0
#define EVT_KEY_A   1 << 1 
#define EVT_KEY_S   1 << 2
#define EVT_KEY_D   1 << 3
#define EVT_KEY_X   1 << 4
#define EVT_KEY_Z   1 << 5
#define EVT_KEY_P   1 << 6
#define EVT_KEY_O   1 << 7
#define EVT_KEY_C   1 << 8
#define KEYCTRL_EVENT   0x1ff


#define CAG_SIMULATOR_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 15 )
#define CAG_SIMULATOR_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )

typedef struct caMessage {
    
    int type;
    int cell_x;
    int cell_y;
} caMessage_t;

QueueHandle_t simulatorMsgQ;
QueueSetHandle_t xQueueSet;
QueueSetMemberHandle_t xActivatedMember;


SemaphoreHandle_t displaySemaphore;
SemaphoreHandle_t pbSemaphore;	// Semaphore for pushbutton interrupt


EventGroupHandle_t keyctrlEventGroup;

int grid[16][64];  //the core global grid that performs the simulation


TaskHandle_t taskSim;

void s4575272_tsk_CAG_simulator_init(void);
void s4575272TaskCAG_Simulator(void);
int nbr_count(int grid[16][64], int i, int j);
void drawBeacon(caMessage_t* msgFromMnem);
void drawBlock(caMessage_t* msgFromMnem);
void drawBeehive(caMessage_t* msgFromMnem);
void drawLoaf(caMessage_t* msgFromMnem);

#endif