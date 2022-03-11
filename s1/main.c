/**
  ******************************************************************************
  * @file    stage1/main.c
  * @author  Kuang Sheng
  * @date    11032022
  * @brief   Nucleo429ZI onboard stage1 design task. The LED Bar displays the number of times
  * the Joystick pushbutton has been pressed.
  ******************************************************************************
  */
#include "board.h"
#include "processor_hal.h"
#include "s4575272_lta1000g.h"

void hardware_init(void);
extern void s4575272_reg_lta1000g_init(void);
void lta1000g_seg_set(int segment, unsigned char segment_value);
extern void s4575272_reg_lta1000g_init_write(unsigned short value);
extern void s4575272_reg_joystick_pb_init(void);
extern void s4575272_reg_joystick_pb_isr(void); //callback function
extern void s4575272_reg_joystick_press_reset(void);
extern int s4575272_reg_joystick_press_get(void);
static int joystick_press_counter;

/*
 * Main program - flashes on board LEDs
 */
int main(void)  {

	HAL_Init();			  //Initialise board.
	hardware_init();	//Initialise hardware modules

	// Main processing loop
  while (1) {
    
    s4575272_reg_lta1000g_init_write(s4575272_reg_joystick_press_get());
    if (s4575272_reg_joystick_press_get() > 1023) { 
      s4575272_reg_joystick_press_reset(); //The maximum number can display is 1023
    }
    HAL_Delay(10); //Refresh display every 10ms

  }
  
  return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

  s4575272_reg_lta1000g_init(); //Initialise GPIO pins for LED 
  s4575272_reg_joystick_pb_init(); //Initialise GPIO pins for joystick

}

