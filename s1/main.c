#include "board.h"
#include "processor_hal.h"
#include "s4575272_lta1000g.h"

void hardware_init(void);

//defined in lta1000g.c file
extern void s4575272_reg_lta1000g_init(void);
void lta1000g_seg_set(int segment, unsigned char segment_value);
extern void s4575272_reg_lta1000g_init_write(unsigned short value);

extern void s4575272_reg_joystick_pb_init(void);
extern void s4575272_reg_joystick_pb_isr(void); //callback function
extern void s4575272_reg_joystick_press_reset(void);
extern int s4575272_reg_joystick_press_get(void);

static int joystick_press_counter; //只在定义该变量的源文件有效，同一源程序的其他源文件整不能使用它。


/*
 * Main program - flashes on board LEDs
 */
int main(void)  {

	uint16_t write_value = 0;

	HAL_Init();			  //Initialise board.
	hardware_init();	//Initialise hardware modules

	// Main processing loop
  while (1) {
    
    s4575272_reg_lta1000g_init_write(s4575272_reg_joystick_press_get());

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

