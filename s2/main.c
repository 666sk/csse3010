/**
  ******************************************************************************
  * @file    stage2/main.c
  * @author  Kuang Sheng
  * @date    24/03/2022
  * @brief   Nucleo429ZI onboard stage2 design task. Using joystick to control the pan and tilt servomotors.
  * Also display the current pan angle on the LTA1000G led bard
  ******************************************************************************
  */
#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"


ADC_HandleTypeDef AdcHandle1,AdcHandle2;
ADC_ChannelConfTypeDef AdcChanConfig1,AdcChanConfig2;

#define S4575272_REG_PANTILT_PAN_90_CAL_OFFSET  -70
#define S4575272_REG_PANTILT_TILT_90_CAL_OFFSET -105
#define S4575272_REG_JOYSTICK_X_ZERO_CAL_OFFSET 3
#define S4575272_REG_JOYSTICK_Y_ZERO_CAL_OFFSET -112
#define S4575272_REG_PANTILT_PAN_WRITE(angle) s4575272_pantilt_angle_write(0, angle)
#define S4575272_REG_PANTILT_TILT_WRITE(angle) s4575272_pantilt_angle_write(1, angle)
#define S4575272_REG_JOYSTICK_X_READ() (s4575272_joystick_readxy(AdcHandle1))
#define S4575272_REG_JOYSTICK_Y_READ() (s4575272_joystick_readxy(AdcHandle2))

void hardware_init(void);
void s4575272_reg_joystick_init(void);
void s4575272_reg_pantilt_init(void);
void s4575272_reg_lta1000g_init(void);
int s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput);
void s4575272_pantilt_angle_write(int type, float angle);
float s4575272_pantilt_read(int type);

float PanAngle;
float TiltAngle;
int main(void)
{
	HAL_Init();
	hardware_init();
	BRD_debuguart_init();  //Initialise UART for debug log output

	// Main Processing Loop
	while (1)
	{	
		//GPIOA->ODR |= (0X01 << 4);
		//GPIOC->ODR |= (0X01 << 6);
		// Start ADC conversion for both ADC1 and ADC2
		HAL_ADC_Start(&AdcHandle1); 
		HAL_ADC_Start(&AdcHandle2); 
		
		//debug_log("adc_value and adc_value1 %d %d\n\r",adc_value, adc_value1);
		PanAngle = 0.04395 * (S4575272_REG_JOYSTICK_X_READ() + S4575272_REG_JOYSTICK_X_ZERO_CAL_OFFSET) + S4575272_REG_PANTILT_PAN_90_CAL_OFFSET;
		TiltAngle = 0.04395 * (S4575272_REG_JOYSTICK_Y_READ() + S4575272_REG_JOYSTICK_Y_ZERO_CAL_OFFSET) + S4575272_REG_PANTILT_TILT_90_CAL_OFFSET;
		
		S4575272_REG_PANTILT_PAN_WRITE(PanAngle);
		S4575272_REG_PANTILT_TILT_WRITE(TiltAngle);
		//S4575272_REG_PANTILT_PAN_WRITE(s4575272_pantilt_read(0));
		//S4575272_REG_PANTILT_TILT_WRITE(s4575272_pantilt_read(1));
		debug_log("adcX adcY %d %d\n\r",S4575272_REG_JOYSTICK_X_READ(),S4575272_REG_JOYSTICK_Y_READ());


		//HAL_Delay(300);
	}
}


void hardware_init(void) {
	s4575272_reg_joystick_init();
	s4575272_reg_pantilt_init();
	s4575272_reg_lta1000g_init();
}





