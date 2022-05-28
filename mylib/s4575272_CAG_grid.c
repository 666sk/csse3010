/** 
 **************************************************************
 * @file mylib/s457527_CAG_grid.c
 * @author Kuang Sheng - 45752720
 * @date 15/05/2022
 * @brief mylib CAG grid driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4575272_tsk_CAG_grid_init(void) - Initialize the task of grid
 * s4575272TaskCAG_Grid(void) - The task of grid mode
 *************************************************************** 
 */

#include "s4575272_CAG_grid.h"

//Initialize the task of grid
void s4575272_tsk_CAG_grid_init(void) {
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Grid,     // Function that implements the task
        (const signed char *) "TaskCAG_Grid",   // Text name for the task
        CAG_GRID_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_GRID_TASK_PRIORITY+2,              // Priority at which the task is created
        NULL);  
}

//The task of grid mode
void s4575272TaskCAG_Grid(void) {
    BRD_LEDInit();
    hardware_init();
    BRD_debuguart_init();
    s4575272_reg_lta1000g_init();

    //Event group
    EventBits_t uxBits;
    EventBits_t uxBits1;
    keyctrlEventGroup = xEventGroupCreate();
    
    //Queue
    caMessage_t msgToSimulator;
    //xQueueSet = xQueueCreateSet(sizeof(msgToSimulator)+10);
    simulatorMsgQ = xQueueCreate(10, sizeof(msgToSimulator));
    //xQueueAddToSet(simulatorMsgQ, xQueueSet);


    pbSemaphore = xSemaphoreCreateBinary();	

    
    char recvChar;   //Receiving chars from keyboard
    uint8_t xIndex = 0; 
    uint8_t yIndex = 0;   //indicates the current X and Y values used for LED Bar display cursor location

    mode = 1;   //1 is grid mode, 0 is mnemonic mode

    for (;;) {

        if (mode){
       
        if (((recvChar = BRD_debuguart_getc()) != '\0')) {

            uxBits = recvCharHandler(recvChar, &keyctrlEventGroup, &xIndex, &yIndex);

            s4575272_reg_lta1000g_init_write(16 * xIndex + yIndex);   //indicates the location in LED Bar
            
        }

    
    }
        vTaskDelay(100);
    }

}



EventBits_t recvCharHandler(char recvChar, EventGroupHandle_t *keyctrlEventGroup, uint8_t *xIndex, uint8_t *yIndex) {
    EventBits_t uxBits;

    if (recvChar == 'W') {

        if (*yIndex > 0) {
            *yIndex += (-1);
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_W);
    } else if (recvChar == 'A') {

        if (*xIndex > 0) {
            *xIndex += (-1);
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_A);
    } else if (recvChar == 'S') {

        if (*yIndex < 15) {
            *yIndex += 1;
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_S);
    } else if (recvChar == 'D') {

        if (*xIndex < 63) {
            *xIndex += 1;
        }
        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_D);
    } else if (recvChar == 'X') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_X);
    } else if (recvChar == 'Z') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_Z);
    } else if (recvChar == 'P') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_P);
    } else if (recvChar == 'O') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_O);
    } else if (recvChar == 'C') {

        uxBits = xEventGroupSetBits(*keyctrlEventGroup, EVT_KEY_C);
    }
    return uxBits;
}


/*
 * Hardware Initialisation.
 */
static void hardware_init() {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	// Enable GPIOC Clock
	__GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode

	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13) on EXTICR4.
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->RTSR |= EXTI_RTSR_TR13;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR13;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM13;		//Enable external interrupt

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	portENABLE_INTERRUPTS();	//Enable interrupts
}

/*
 * Pushbutton callback function
 */
void pb_callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == 13) {

		// Is it time for another Task() to run?
		xHigherPriorityTaskWoken = pdFALSE;

        mode = 1 - mode; //toggle mode

		if (pbSemaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR(pbSemaphore, &xHigherPriorityTaskWoken );		// Give PB Semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

/*
 * Interrupt handler for EXTI 15 to 10 IRQ Handler
 */ 
void EXTI15_10_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

	// PR: Pending register
	if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) {

		// cleared by writing a 1 to this bit
		EXTI->PR |= EXTI_PR_PR13;	//Clear interrupt flag.

		pb_callback(13);   // Callback for C13
	}
}
