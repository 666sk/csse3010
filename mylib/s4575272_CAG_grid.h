 /** 
 **************************************************************
 * @file mylib/s457527_CAG_grid.c
 * @author Kuang Sheng - 45752720
 * @date 15/05/2022
 * @brief mylib CAG grid driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_tsk_CAG_grid_init(void) - Initialize the task of grid
 * s4575272TaskCAG_Grid(void) - The task of grid mode
 *************************************************************** 
 */

#ifndef __S4575272_CAG_grid_H__
#define __S4575272_CAG_grid_H__
#include "s4575272_CAG_simulator.h"
#include "s4575272_lta1000g.h"
#define CAG_GRID_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 8 )
#define CAG_GRID_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )
uint8_t mode;

void s4575272_tsk_CAG_grid_init(void);
void s4575272TaskCAG_Grid(void);
void s4575272_reg_lta1000g_init(void);
void s4575272_reg_lta1000g_init_write(unsigned short value);
void lta1000g_seg_set(int segment, unsigned char segment_value);
EventBits_t recvCharHandler(char recvChar,EventGroupHandle_t *keyctrlEventGroup, uint8_t *xIndex, uint8_t *yIndex);


#endif





