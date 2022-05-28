/**
  ******************************************************************************
  * @file    pf/main.c
  * @author  Kuang Sheng
  * @date    15/05/2022
  * @brief   Nucleo429ZI final project design task. 
  * Conway's game of life design using freeRTOS.
  ******************************************************************************
  */
#include "s4575272_CAG_simulator.h"
#include "s4575272_CAG_display.h"
#include "s4575272_CAG_grid.h"
#include "s4575272_CAG_mnemonic.h"
#include "s4575272_CAG_joystick.h"


int main(void) {

  HAL_Init();

  s4575272_tsk_CAG_simulator_init();

  s4575272_tsk_CAG_grid_init();

  s4575272_tsk_CAG_mnemonic_init();
  
  s4575272_tsk_CAG_display_init();
  
  
  vTaskStartScheduler();
  return 0;
}