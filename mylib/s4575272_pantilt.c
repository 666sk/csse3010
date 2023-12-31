/** 
 **************************************************************
 * @file mylib/s457527_pantilt.c
 * @author Kuang Sheng - 45752720
 * @date 24/03/2022
 * @brief mylib pan&tilt driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_reg_pantilt_init() - Initialise servo (GPIO, PWM etc) for PE11 and PE9
 * s4575272_pantilt_angle_write(int type, int angle) - Generic function for writing an angle (0 to +-70) (type is 0 for pan or 1 for tilt)
 * s4575272_pantilt_read(int type) - Read the current pan servo angle (0 to +-70)(type is 0 for pan or 1 for tilt). 
 *************************************************************** 
 */
#include "board.h"
#include "processor_hal.h"
#include "s4575272_pantilt.h"

extern int PanAngle;
extern int TiltAngle;

#define TIMER_RUNNING_FREQ  		500000
#define TIMER_20MS_PERIOD_TICKS		20000
#define S4575272_REG_PANTILT_PAN_90_CAL_OFFSET  2.38
#define S4575272_REG_PANTILT_TILT_90_CAL_OFFSET 2.03
#define PWM_PERCENT2TICKS_DUTYCYCLE(value)	(value * TIMER_20MS_PERIOD_TICKS / 100)    //Convert Duty circle percentage to ticks in CCR
#define S4575272_REG_PANTILT_PAN_WRITE(angle) s4575272_pantilt_angle_write(0, angle)  //Access generic angle write function for the pan
#define S4575272_REG_PANTILT_PAN_READ() s4575272_pantilt_read(0)    //Access generic angle read function for the pan
#define S4575272_REG_PANTILT_TILT_WRITE(angle) s4575272_pantilt_angle_write(1, angle)    //Access generic angle write function for the tilt
#define S4575272_REG_PANTILT_TILT_READ() s4575272_pantilt_read(1)    //Access generic angle read function for the tilt

//Initialise servo (GPIO, PWM etc) for PE11 and PE9
void s4575272_reg_pantilt_init() {

	// Enable GPIOE Clock
	__GPIOE_CLK_ENABLE();

    GPIOE->MODER &= ~(0x33 << (9 * 2));     //Clear bits
	GPIOE->MODER |= (0x22 << (9 * 2)); 
	GPIOE->OSPEEDR |= (0x22 << (9 * 2));     //Set fast speed.
	GPIOE->PUPDR &= ~(0x33 << (9 * 2));
	GPIOE->AFR[1] &= ~((0x0F) << (1 * 4));     //Clear Alternate Function for pin (higher ARF register)
	GPIOE->AFR[1] &= ~((0x0F) << (3 * 4));
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << (1 * 4));     //Config Alternate Function for pin
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << (3 * 4));

    __TIM1_CLK_ENABLE();             //enable Timer1

    TIM1->PSC = ((SystemCoreClock / 2) / TIMER_RUNNING_FREQ) - 1;   //Set timer_running_frequency
    TIM1->CR1 &= ~TIM_CR1_DIR; 
	TIM1->ARR = TIMER_20MS_PERIOD_TICKS	; 		//Set pulse period to 0.02s 

	//PE9
    TIM1->CCR1 = PWM_PERCENT2TICKS_DUTYCYCLE(7.25);  //Initialise pulse width to 7.25% duty cycle for pan
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); 	// Clear OC2M (Channel 1) 
	TIM1->CCMR1 |= (0x6 << 4); 	// Enable PWM Mode 1, upcounting, on Channel 1
	TIM1->CCMR1 |= (TIM_CCMR1_OC1PE); 	// Enable output preload bit for channel 1 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC1E; 	// Set CC1E Bit 
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output 

	//PE11
	TIM1->CCR2 = PWM_PERCENT2TICKS_DUTYCYCLE(7.25);	//Initialise pulse width to 7.25% duty cycle for tilt
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 12); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC2PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CCER |= TIM_CCER_CC2E; 	// Set CC2E Bit 
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output 
    TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 
    
	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter
}


//Generic function for writing an angle (0 to +-90) (type is 0 for pan or 1 for tilt)
void s4575272_pantilt_angle_write(int type, int angle) {

	float DutyCircle;

	if (type == 0) {

		DutyCircle = 0.0551 * angle + S4575272_REG_PANTILT_PAN_90_CAL_OFFSET;
		TIM1->CCR1 = PWM_PERCENT2TICKS_DUTYCYCLE(DutyCircle);
	} else if (type == 1) {

		DutyCircle = 0.054 * angle + S4575272_REG_PANTILT_TILT_90_CAL_OFFSET;
		TIM1->CCR2 = PWM_PERCENT2TICKS_DUTYCYCLE(DutyCircle);
	}
}

//Read the current pan servo angle (0 to +-70)(type is 0 for pan or 1 for tilt). Must use a register functions to accerss duty cycle value
int s4575272_pantilt_read(int type) {

	if (type == 0) {

		return PanAngle;
	} else if (type == 1) {

		return TiltAngle;
	}
}

