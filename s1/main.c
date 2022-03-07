#include "board.h"
#include "processor_hal.h"
#include "s4575272_lta1000g.h"


void hardware_init(void);

//defined in lta1000g.c file
extern void s4575272_reg_lta1000g_init(void);
void lta1000g_seg_set(int segment, unsigned char segment_value);
extern void s4575272_reg_lta1000g_init_write(unsigned short value);

extern void s4575272_reg_joystick_pb_init(void);
void s4575272_reg_joystick_pb_isr(void); //callback function
void s4575272_reg_joystick_press_reset(void);
int s4575272_reg_joystick_press_get(void);

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


//Enable the joystick pushbutton source, e.g. enable GPIO input and interrupt
void s4575272_reg_joystick_pb_init(){
  __GPIOA_CLK_ENABLE(); //no need actually

  GPIOA->OSPEEDR |= (GPIO_SPEED_FAST << 3); //set fast speed
  GPIOA->PUPDR &= ~(0x03 << (3 * 2)); //set as no pull up/pull down
  GPIOA->MODER &= ~(0x03 << (3 * 2)); //set as input mode
  
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Enable EXTI clock
  
  //Select trigger source (port A, pin 3) on EXTICR1. (Should be 0 at all bits)
  SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3; //Clear the bits
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
  //SYSCFG->EXTICR[0] &= ~(0x0FFF); //Reset the other bits

  EXTI->RTSR |= EXTI_RTSR_TR3;  //enable rising edge
  EXTI->FTSR &= ~EXTI_RTSR_TR3;  //disable falling edge
  EXTI->IMR |= EXTI_IMR_IM3;  //enable external interrupt

  //Enable priority 10 and interrupt callback.
  HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void s4575272_reg_joystick_pb_isr(void) { //+debouncing

  
  joystick_press_counter++;

}


void EXTI3_IRQHandler(void) {
  NVIC_ClearPendingIRQ(EXTI3_IRQn);

  if ((EXTI->PR & EXTI_PR_PR3) == EXTI_PR_PR3) {
    EXTI->PR |= EXTI_PR_PR3;

    s4575272_reg_joystick_pb_isr(); //call this callback function
  }
}



int s4575272_reg_joystick_press_get(void){
  return joystick_press_counter;
}

void s4575272_reg_joystick_press_reset(void){
  joystick_press_counter = 0;
}