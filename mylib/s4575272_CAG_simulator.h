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
 * void drawBeacon(caMessage_t* msgFromMnem); - Draw Beacon model
 * void drawBlock(caMessage_t* msgFromMnem); - Draw BLock model
 * void drawBlinker(caMessage_t* msgFromMnem); - Draw Blinker model
 * void drawToad(caMessage_t* msgFromMnem); - Draw Toad model
 * void drawBeehive(caMessage_t* msgFromMnem); - Draw Beehive model
 * void drawLoaf(caMessage_t* msgFromMnem); - Draw LOaf model
 * void drawGlider(caMessage_t* msgFromMnem); - Draw Glider model
 * void clearGrid(void); - Clear the grid
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
#include "s4575272_joystick.h"

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
#define START 0x01
#define STOP 0x02
#define CLEAR 0X03
#define DEL_SIMULATOR 0X04
#define DEL_JOYSTICK 0X05
#define CRE_SIMULATOR 0X04
#define CRE_JOYSTICK 0X05

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
#define CAG_SIMULATOR_TASK_PRIORITY        ( tskIDLE_PRIORITY + 4 )


typedef struct caMessage {
    
    int type;
    int cell_x;
    int cell_y;
} caMessage_t;

QueueHandle_t simulatorMsgQ;
//QueueSetHandle_t xQueueSet;
//QueueSetMemberHandle_t xActivatedMember;
QueueHandle_t signalMsgQ;

//QueueHandle_t signalMsgQ;

typedef struct signalMsg {
    
    int xSignal;
    int ySignal;
} signalMsg_t;


SemaphoreHandle_t displaySemaphore;
SemaphoreHandle_t pbSemaphore;	// Semaphore for pushbutton interrupt


EventGroupHandle_t keyctrlEventGroup;



int grid[16][64];  //the core global grid that performs the simulation
static uint32_t prevTime1 = 0;  //Debouncing for on board pushbutton


TaskHandle_t taskSim;
TaskHandle_t taskGrid;
TaskHandle_t taskMnem;
TaskHandle_t taskJoystick;

void hardware_init(void);
void pb_callback(uint16_t GPIO_Pin);
void EXTI15_10_IRQHandler(void);

void s4575272_tsk_CAG_simulator_init(void);
void s4575272TaskCAG_Simulator(void);
int nbr_count(int grid[16][64], int i, int j);
void drawBeacon(caMessage_t* msgFromMnem);
void drawBlock(caMessage_t* msgFromMnem);
void drawBlinker(caMessage_t* msgFromMnem);
void drawToad(caMessage_t* msgFromMnem);
void drawBeehive(caMessage_t* msgFromMnem);
void drawLoaf(caMessage_t* msgFromMnem);
void drawGlider(caMessage_t* msgFromMnem);
void clearGrid(void);
void delSimulator(void);

#endif