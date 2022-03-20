#include "board.h"
#include "processor_hal.h"

//Enable the ADC1 on Board pin PC0.

ADC_HandleTypeDef AdcHandle,AdcHandle1;
ADC_ChannelConfTypeDef AdcChanConfig,AdcChanConfig1;

void hardware_init(void);

int main(void)
{
	unsigned int adc_value;
	unsigned int adc_value1;

	HAL_Init();
	hardware_init();

	// Main Processing Loop
	while (1)
	{

		HAL_ADC_Start(&AdcHandle); // Start ADC conversion
		HAL_ADC_Start(&AdcHandle1); 
		// Wait for ADC conversion to finish
		while ((HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK) || (HAL_ADC_PollForConversion(&AdcHandle1, 10) != HAL_OK)); //return HAL_OK shuoming zhuanhuan wancheng
		adc_value = ADC1->DR;		//Read ADC1 Data Register for converted value.
		adc_value1 = ADC2->DR;
		// If ADC value is higher than 1/2 Reference voltage, turn on Green LED.
		// Else if ADC value is lower than 1/2 Reference voltage, turn off Green LED.
		if (adc_value > 0x7FF) {
			BRD_LEDGreenOn();
		} else {
			BRD_LEDBlueOn();
		}

		if (adc_value1 < 0x7FF) {
			BRD_LEDGreenOff();
		} else {
			BRD_LEDBlueOff();
		}
	}
}


void hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Turn off LEDs
	BRD_LEDGreenOff();
    BRD_LEDBlueOff();
	BRD_LEDRedOff();
	// Enable the GPIO Clock
  	__GPIOC_CLK_ENABLE();
  
	// Initalise PA3 as an Analog input.
  	GPIOC->MODER |= (0x03 << (0 * 2));			//Set bits for Analog input mode
  	GPIOC->OSPEEDR &= ~(0x03<<(0 * 2));
	GPIOC->OSPEEDR |= 0x02<<(0 * 2);  			// Fast speed
	GPIOC->PUPDR &= ~(0x03 << (0 * 2));			//Clear bits for no push/pull

	GPIOC->MODER |= (0x03 << (3 * 2));			//Set bits for Analog input mode
  	GPIOC->OSPEEDR &= ~(0x03<<(3 * 2));
	GPIOC->OSPEEDR |= 0x02<<(3 * 2);  			// Fast speed
	GPIOC->PUPDR &= ~(0x03 << (3 * 2));	

	__ADC1_CLK_ENABLE();						//Enable ADC1 clock
	__ADC2_CLK_ENABLE();

	AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA3)
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;

	AdcHandle1.Instance = (ADC_TypeDef *)(ADC2_BASE);						//Use ADC1 (For PA3)
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

	HAL_ADC_Init(&AdcHandle);		//Initialise ADC
	HAL_ADC_Init(&AdcHandle1);
	// Configure ADC Channel
	AdcChanConfig.Channel = ADC_CHANNEL_10;					//PC0 has Analog Channel 10 connected
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig.Offset       = 0;

	AdcChanConfig1.Channel = ADC_CHANNEL_13;
	AdcChanConfig1.Rank         = 1;
	AdcChanConfig1.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig1.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC Channel
	HAL_ADC_ConfigChannel(&AdcHandle1, &AdcChanConfig1);
}





