#include "board.h"
#include "processor_hal.h"

void hardware_init(void);

/*
 * Main program - flashes on board LEDs
 */
int main(void)  {

	uint16_t write_value = 0;

	HAL_Init();			  //Initialise board.

	hardware_init();	//Initialise hardware modules

	// Main processing loop
  while (1) {

    GPIOC -> ODR |= (0x01 << 6);
    HAL_Delay(1000);
    GPIOC -> ODR &= ~(0x01 << 6);
    HAL_Delay(1000);

  }

  return 0;
}

/*
 * Initialise Hardware
 */
void hardware_init(void) {

	// Enable the GPIO G Clock
  __GPIOC_CLK_ENABLE();

  //Initialise G9 as an output.
  GPIOC->MODER &= ~(0x03 << (6 * 2));  //clear bits
  GPIOC->MODER |= (0x01 << (6 * 2));   //Set for push pull

  GPIOC->OSPEEDR &= ~(0x03<<(6 * 2));
  GPIOC->OSPEEDR |=   0x02<<(6 * 2);  // Set for Fast speed

  GPIOC->OTYPER &= ~(0x01 << 6);       //Clear Bit for Push/Pull utput

  // Activate the Pull-up or Pull down resistor for the current IO
  GPIOC->PUPDR &= ~(0x03 << (6 * 2));   //Clear Bits
  GPIOC->PUPDR |= ((0x01) << (6 * 2));  //Set for Pull down output
  

}