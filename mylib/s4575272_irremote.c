 /** 
 **************************************************************
 * @file mylib/s457527_irremote.c
 * @author Kuang Sheng - 45752720
 * @date 07/03/2022
 * @brief mylib irremote driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * void s4575272_reg_irremote_init(void) - Initialize the IR receiver hardware
 * void s4575272_reg_irremote_recv(void) - function for processing the received input waveform
 * int s4575272_reg_irremote_readkey(char* value) - Check if a key has been detected. Return 1 if detected and set in value parameter.
 *************************************************************** 
 */
#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"
#include "s4575272_irremote.h"

//PB10
void s4575272_reg_irremote_init(void) {

    BRD_LEDInit();		//Initialise LEDS
    BRD_debuguart_init();
	// Turn off LEDs
	BRD_LEDGreenOff();
    BRD_LEDBlueOff();
    // Enable GPIOB Clock
	__GPIOB_CLK_ENABLE();

    GPIOB->OSPEEDR |= (GPIO_SPEED_FAST << 10);		//Set fast speed.
    //GPIOB->OSPEEDR |= (0x00 << 10);
	GPIOB->PUPDR &= ~(0x03 << (10 * 2));				//Clear bits for no push/pull
	GPIOB->MODER &= ~(0x03 << (10 * 2));				//Clear bits
	GPIOB->MODER |= (GPIO_MODE_AF_PP << (10 * 2));  	//Set Alternate Function Push Pull Mode


	GPIOB->AFR[1] &= ~((0x0F) << (2 * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOB->AFR[1] |= (GPIO_AF1_TIM2 << (2 * 4));	//Set Alternate Function for pin (lower ARF register) 

    __TIM2_CLK_ENABLE();

	// Compute the prescaler value to set the timer counting frequency to 50kHz
	// SystemCoreClock is the system clock frequency
	TIM2->PSC = ((SystemCoreClock / 1) / TIMER_COUNTER_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM2->CR1 &= ~TIM_CR1_DIR; 

    //TIM2->CNT = TIMER_1SECOND_PERIOD;	//Load counter register with 1s period value
    TIM2->CNT = 0;
	TIM2->ARR = TIMER_1SECOND_PERIOD;

    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
	TIM2->CCMR2 |= TIM_CCMR2_CC3S_0; 
											
	// Disable digital filtering by clearing IC1F[3:0] bits
	// because we want to capture every event
	TIM2->CCMR2 &= ~TIM_CCMR2_IC3F;

    // Select the edge of the active transition
	// Detect only rising edges in this example
	// CC4NP:CC4P bits
	// 00 = rising edge,
	// 01 = falling edge,
	// 10 = reserved,
	// 11 = both edges
	//TIM2->CCER |= (TIM_CCER_CC3NP | TIM_CCER_CC3P);		// Both rising and falling edges.
    TIM2->CCER |= (TIM_CCER_CC3P);  //falling edge
    //TIM2->CCER |= (TIM_CCER_CC3NP);  //rising edge

    // Program the input prescaler
	// To capture each valid transition, set the input prescaler to zero;
	// IC4PSC[1:0] bits (input capture 1 prescaler)
	TIM2->CCMR2 &= ~TIM_CCMR2_IC3PSC; // Clear filtering because we need to capture every event
	
	// Enable Capture for channel 4
	TIM2->CCER |= TIM_CCER_CC3E;    
	
	// Enable related interrupts
	TIM2->DIER |= TIM_DIER_CC3IE;		// Enable Capture interrupts for channel 3
	TIM2->DIER |= TIM_DIER_UIE;			// Enable update interrupt to occur on timer counter overflow or underflow.
	
	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(TIM2_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->CR1 |= TIM_CR1_CEN;				// Enable the counter
}


/*
 * Timer update interrupt callback
 */
void s4575272_reg_irremote_recv(void) {

	uint32_t current = 0;

	// Check if overlfow has taken place
	if(((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)) {	 
        
        //if (keyPressedFlag == 1) {
            
            if (msCount < 50) {
                msCount++;
                //debug_log("mscount = %d\n\r", msCount);
                //BRD_LEDBlueToggle();
                //keyPressedFlag = 0;
                //msCount = 0;
            }

        //}
		TIM2->SR &= ~TIM_SR_UIF; 	// Clear the UIF Flag
	}

	// Check if input capture has taken place 
	if((TIM2->SR & TIM_SR_CC3IF) == TIM_SR_CC3IF) { 
        timerValue = msCount;
        msCount = 0;
        pulseCount ++;
        //debug_log("pulsecount = %d\n\r", pulseCount);
        if ((timerValue >= 50)) {
            pulseCount = -2;
            bitPattern = 0;
            newKey = 0;
        } else if ((pulseCount >= 0) && (pulseCount < 32)) {
        //} else if (pulseCount == 30) {
            
            if(timerValue >= 2) {
                bitPattern |= (1<<(31-pulseCount));
            } else {
                bitPattern |= (0<<(31-pulseCount));
            }
        } else if (pulseCount >= 32) {
            keyPressedValue = bitPattern;
            debug_log("PULSECOUNT %d\n\r", pulseCount);
            pulseCount = 0;
            debug_log("key is %d\n\r", newKey);
            keyPressedFlag = s4575272_reg_irremote_readkey(keyPressedValue);
            newKey = 0;
        }
        
		current = TIM2->CCR3;  // Reading CCR1 clears CC4IF
	}
}

/*
 * Interrupt handler for Timer 2 Interrupt Handler
 */ 
void TIM2_IRQHandler(void)
{
  
	s4575272_reg_irremote_recv();   // Callback for overflow and input capture.
}

int s4575272_reg_irremote_readkey(char* value) {

    if (value) {
    
        return 1;
    } else {
        return 0;
    }
}

