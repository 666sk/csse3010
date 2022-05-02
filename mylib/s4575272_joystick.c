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
 * s4575272_reg_joystick_init() - Initialise GPIO and ADC
 * s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput) - Generic function to read X or Y joystick value
 *************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"
#include "s4575272_joystick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

static uint32_t prevTime = 0;
ADC_HandleTypeDef AdcHandle1,AdcHandle2;
ADC_ChannelConfTypeDef AdcChanConfig1,AdcChanConfig2;

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
  EXTI->RTSR &= ~EXTI_RTSR_TR3;  //disable rising edge
  EXTI->IMR |= EXTI_IMR_IM3;  //enable external interrupt

  //Enable priority 10 and interrupt callback.
  HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

//The callback function that count how many times the button being pressed
extern void s4575272_reg_joystick_pb_isr(void) {
  uint32_t currentTime;
  //currentTime = HAL_GetTick();

  //Debouncing process
  if ((HAL_GetTick() - prevTime) > 50) {

    if ((GPIOA->IDR & (0x01 << 3)) == (0x00 << 3)) {
		
		BaseType_t xHigherPriorityTaskWoken;

		xHigherPriorityTaskWoken = pdFALSE;
		if (pbSemaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR( pbSemaphore, &xHigherPriorityTaskWoken );		// Send semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );


        //joystick_press_counter++;
    }

  prevTime = HAL_GetTick();
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

/****************************************************************/

//Initialise GPIO and ADC
void s4575272_reg_joystick_init(){
    BRD_LEDInit();		//Initialise LEDS

	// Enable the GPIO Clock
    __GPIOC_CLK_ENABLE();

	GPIOC->MODER &= ~(0xC3 << (0 * 2));       //Clear bits for MODER
	GPIOC->MODER |= (0xC3 << (0 * 2));		  //Set bits for Analog input mode
	GPIOC->OSPEEDR &= ~(0xC3 << (0 * 2));
	GPIOC->OSPEEDR |= (0x82 << (0 * 2));	      // Fast speed
	GPIOC->PUPDR &= ~(0xC3  << (0 * 2));		  //Clear bits for no push/pull

	__ADC1_CLK_ENABLE();						//Enable ADC1 clock
	__ADC2_CLK_ENABLE();					    //Enable ADC2 clock

	AdcHandle1.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA3)
	AdcHandle1.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle1.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle1.Init.ScanConvMode          = DISABLE;
	AdcHandle1.Init.ContinuousConvMode    = DISABLE;
	AdcHandle1.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle1.Init.NbrOfDiscConversion   = 0;
	AdcHandle1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle1.Init.NbrOfConversion       = 1;
	AdcHandle1.Init.DMAContinuousRequests = DISABLE;
	AdcHandle1.Init.EOCSelection          = DISABLE;

	AdcHandle2.Instance = (ADC_TypeDef *)(ADC2_BASE);						//Use ADC1 (For PA3)
	AdcHandle2.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle2.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle2.Init.ScanConvMode          = DISABLE;
	AdcHandle2.Init.ContinuousConvMode    = DISABLE;
	AdcHandle2.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle2.Init.NbrOfDiscConversion   = 0;
	AdcHandle2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle2.Init.NbrOfConversion       = 1;
	AdcHandle2.Init.DMAContinuousRequests = DISABLE;
	AdcHandle2.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&AdcHandle1);		//Initialise ADC1
	HAL_ADC_Init(&AdcHandle2);      //Initialise ADC2

	AdcChanConfig1.Channel = ADC_CHANNEL_10;					//PC0 has Analog Channel 10 connected
	AdcChanConfig1.Rank         = 1;
	AdcChanConfig1.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig1.Offset       = 0;
	
	AdcChanConfig2.Channel = ADC_CHANNEL_13;                     //PC3 has Analog Channel 13 connected
	AdcChanConfig2.Rank         = 1;
	AdcChanConfig2.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig2.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle1, &AdcChanConfig1);		//Initialise ADC Channel
	HAL_ADC_ConfigChannel(&AdcHandle2, &AdcChanConfig2);

}

//Generic function to read X or Y joystick value
int s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput) {

	if (AdcHandleInput.Instance == (ADC_TypeDef *)(ADC1_BASE)) {

		return ADC1->DR;
	} else if (AdcHandleInput.Instance == (ADC_TypeDef *)(ADC2_BASE)) {
		
		return ADC2->DR;
	}
}



/****************************************************************/

// SemaphoreHandle_t pbSemaphore;	// Semaphore for pushbutton interrupt

void s4575272TaskJoystick(void) {

	pbSemaphore = xSemaphoreCreateBinary();
	s4575272_reg_joystick_pb_init();
	
	uint8_t mode = 1;
	sendToLeftQ = xQueueCreate(10, sizeof(mode));
	sendToRightQ = xQueueCreate(10, sizeof(mode));

	for (;;) {
		
		if (pbSemaphore != NULL) {	// Check if semaphore exists

			if( xSemaphoreTake( pbSemaphore, 10 ) == pdTRUE ) {
				
				if (sendToLeftQ != NULL) {
					xQueueSendToFront(sendToLeftQ, ( void * ) &mode, ( portTickType ) 10 );
				}
				if (sendToRightQ != NULL) {
					xQueueSendToFront(sendToRightQ, ( void * ) &mode, ( portTickType ) 10 );
				}
				
			}
		}

		// Wait for 10ms
		vTaskDelay(10);
	}
}


void s4575272_tsk_joystick_init(void) {
	portDISABLE_INTERRUPTS();

	xTaskCreate(
        (void *) &s4575272TaskJoystick,     // Function that implements the task
        (const signed char *) "JoystickTask",   // Text name for the task
        OLEDTASK_STACK_SIZE * 6,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        OLEDTASK_PRIORITY + 4,              // Priority at which the task is created
        NULL);                          // Used to pass out the created task's handle

	portENABLE_INTERRUPTS();
}
