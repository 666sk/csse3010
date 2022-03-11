 /** 
 **************************************************************
 * @file mylib/s457527_joystick.c
 * @author Kuang Sheng - 45752720
 * @date 07/03/2022
 * @brief mylib joystick driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_joystick_pb_init()- Enable the joystick pushbutton source
 * s4575272_reg_joystick_pb_isr() - Joystick pushbutton interrupt service routine. (Callback function)
 * EXTI3_IRQHandler() - Pushbutton EXTI Handler function
 * s4575272_reg_joystick_press_get() - Returns the value of the Joystick pushbutton press counter
 * s4575272_reg_joystick_press_reset() - Reset the Joystick event counter value to 0 
 *************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"
#include "s4575272_joystick.h"

static uint32_t prev_time = 0;

//Enable the joystick pushbutton source, e.g. enable GPIO input and interrupt
extern void s4575272_reg_joystick_pb_init(){

  GPIOA->OSPEEDR |= (GPIO_SPEED_FAST << 3); //set fast speed
  GPIOA->PUPDR &= ~(0x03 << (3 * 2)); //set as no pull up/pull down
  GPIOA->MODER &= ~(0x03 << (3 * 2)); //set as input mode
  
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Enable EXTI clock
  
  //Select trigger source (port A, pin 3) on EXTICR1. (Should be 0 at all bits)
  SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3; //Clear the bits
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;

  EXTI->FTSR |= EXTI_RTSR_TR3;  //enable falling edge
  EXTI->RTSR |= EXTI_RTSR_TR3;  //enable rising edge
  EXTI->IMR |= EXTI_IMR_IM3;  //enable external interrupt

  //Enable priority 10 and interrupt callback.
  HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

//The callback function that count how many times the button being pressed
extern void s4575272_reg_joystick_pb_isr(void) {
  uint32_t current_time;
  current_time = HAL_GetTick();

  //Debouncing process
  if ((current_time - prev_time) > 10) {

    if ((GPIOA->IDR & (0x01 << 3)) == (0x00 << 3)) {
      joystick_press_counter++;
    }

  prev_time = HAL_GetTick();
  }
}

//Interrupt handler function
void EXTI3_IRQHandler(void) {

  NVIC_ClearPendingIRQ(EXTI3_IRQn);
  if ((EXTI->PR & EXTI_PR_PR3) == EXTI_PR_PR3) {

    EXTI->PR |= EXTI_PR_PR3;
    s4575272_reg_joystick_pb_isr(); //call this callback function
  }
}

//Return the counter value
extern int s4575272_reg_joystick_press_get(void){
  return joystick_press_counter;
}

//Reset the counter value
extern void s4575272_reg_joystick_press_reset(void){
  joystick_press_counter = 0;
}