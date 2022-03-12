 /** 
 **************************************************************
 * @file mylib/s457527_lta1000g.c
 * @author Kuang Sheng - 45752720
 * @date 07/03/2022
 * @brief mylib lta1000g driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_lta1000g_init() - intialise LED Bar GPIO pins as outputs
 * lta1000g_seg_set(int segment, unsigned char segment_value) - set the value for the corresponding LED Bar segment GPIO pin 
 * s4575272_reg_lta1000g_write(unsigned short value) - Write the LED Bar segments high or low
 *************************************************************** 
 */

#include "s4575272_lta1000g.h"
#include "board.h"
#include "processor_hal.h"
#include <string.h>

//Initialise LED Bar GPIO pins as output
void s4575272_reg_lta1000g_init(void) {

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();

  //Initialise PC6 and PC7 as output.
  GPIOC->MODER &= ~(0x0F << (6 * 2));
  GPIOC->MODER |= (0x05 << (6 * 2));   //Set for push pull output for bit 12,13,14,15
  GPIOC->OSPEEDR &= ~(0x0F<<(6 * 2)); 
  GPIOC->OSPEEDR |=   0x0A<<(6 * 2);  // Set for Fast speed
  GPIOC->OTYPER &= ~(0x03 << 6);       //Clear Bit for Push/Pull output

  //Initialise PA4 and PA15 as output
  GPIOA->MODER &= ~(0x03 << (4 * 2));
  GPIOA->MODER &= ~(0x03 << (15 * 2)); //clear bits for bit 8,30
  GPIOA->MODER |= (0x01 << (4 * 2));
  GPIOA->MODER |= (0x01 << (15 * 2)); // set for push pull output for bit 8,30
  GPIOA->OSPEEDR &= ~(0x03 << (4 * 2));
  GPIOA->OSPEEDR &= ~(0x03 << (15 * 2));
  GPIOA->OSPEEDR |= (0x02 << (4 * 2));
  GPIOA->OSPEEDR |= (0x02 << (15 * 2)); //Set fast speed
  GPIOA->OTYPER &= ~(0x01 << 4);
  GPIOA->OTYPER &= ~(0x01 << 15); //clear bit for push/pull output

  //Initialise PB3,PB4,PB5,PB12,PB13,PB15 as output
  GPIOB->MODER &= ~(0x3F << (3 * 2));
  GPIOB->MODER &= ~(0xCF << (12 * 2)); //clear bits
  GPIOB->MODER |= (0x15 << (3 * 2));
  GPIOB->MODER |= (0x45 << (12 * 2)); //set for output
  GPIOB->OSPEEDR &= ~(0x3F << (3 * 2));
  GPIOB->OSPEEDR &= ~(0xCF << (12 * 2)); //clear bits
  GPIOB->OSPEEDR |= (0x2A << (3 * 2));
  GPIOB->OSPEEDR |= (0x8A << (12 * 2)); //set fast speed
  GPIOB->OTYPER &= ~(0X07 << 3);
  GPIOB->OTYPER &= ~(0X0B << 12); //clear bit for push/pull
}

//Write the LED Bar segments high or low
extern void s4575272_reg_lta1000g_init_write(unsigned short value){

  int revSegArray[10];
  memset(revSegArray, 0, sizeof(revSegArray)); //reset array
  int remainder;
  int k = 0;

  //transform decimal to binary and store it in array in reverse order
  do {

    remainder = value % 2;
    revSegArray[k] = remainder;
    value /= 2;
    k++;
  } while (value >= 1);

  //Set the corresponding LED segment
  for (int i = 0; i <= 9; i++) {

    if (revSegArray[i] & 0x01) {

      lta1000g_seg_set(i, '1');
    } else {

      lta1000g_seg_set(i, '0');
    }
  }
}

//set the value for the corresponding LED Bar segment GPIO pin 
void lta1000g_seg_set(int segment, unsigned char segment_value){
  switch(segment) {

    case 0:

      if (segment_value == '0') {
        GPIOB->ODR &= ~(0X01 << 4);
      } else if (segment_value == '1') {
        GPIOB->ODR |= (0X01 << 4);
      }
      return;
    
    case 1:

      if (segment_value == '0') {
        GPIOA->ODR &= ~(0X01 << 4);
      } else if (segment_value == '1') {
        GPIOA->ODR |= (0X01 << 4);
      }
      return;
      
    case 2:

      if (segment_value == '0') {
        GPIOB->ODR &= ~(0X01 << 3);
      } else if (segment_value == '1') {
        GPIOB->ODR |= (0X01 << 3);
      }
      return;

    case 3:

      if (segment_value == '0') {
        GPIOB->ODR &= ~(0X01 << 5);
      } else if (segment_value == '1') {
        GPIOB->ODR |= (0X01 << 5);
      }
      return;

    case 4:

      if (segment_value == '0') {
        GPIOC->ODR &= ~(0X01 << 7);
      } else if (segment_value == '1') {
        GPIOC->ODR |= (0X01 << 7);
      }
      return;

    case 5:

      if (segment_value == '0') {
        GPIOA->ODR &= ~(0X01 << 15);
      } else if (segment_value == '1') {
        GPIOA->ODR |= (0X01 << 15);
      }
      return;

    case 6:

      if (segment_value == '0') {
        GPIOB->ODR &= ~(0X01 << 12);
      } else if (segment_value == '1') {
        GPIOB->ODR |= (0X01 << 12);
      }
      return;

    case 7:

      if (segment_value == '0') {
        GPIOB->ODR &= ~(0X01 << 13);
      } else if (segment_value == '1') {
        GPIOB->ODR |= (0X01 << 13);
      }
      return;

    case 8:

      if (segment_value == '0') {
        GPIOB->ODR &= ~(0X01 << 15);
      } else if (segment_value == '1') {
        GPIOB->ODR |= (0X01 << 15);
      }
      return;

    case 9:
    
      if (segment_value == '0') {
        GPIOC->ODR &= ~(0X01 << 6);
      } else if (segment_value == '1') {
        GPIOC->ODR |= (0X01 << 6);
      }
      return;
  }
}
