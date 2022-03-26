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
int PanAngle;
int TiltAngle;

#define S4575272_REG_JOYSTICK_X_ZERO_CAL_OFFSET 13
#define S4575272_REG_JOYSTICK_Y_ZERO_CAL_OFFSET -92
#define S4575272_REG_PANTILT_PAN_WRITE(angle) s4575272_pantilt_angle_write(0, angle)
#define S4575272_REG_PANTILT_TILT_WRITE(angle) s4575272_pantilt_angle_write(1, angle)
#define S4575272_REG_JOYSTICK_X_READ() s4575272_joystick_readxy(AdcHandle1)
#define S4575272_REG_JOYSTICK_Y_READ() s4575272_joystick_readxy(AdcHandle2)
#define S4575272_REG_PANTILT_PAN_READ() s4575272_pantilt_read(0)
#define S4575272_REG_PANTILT_TILT_READ() s4575272_pantilt_read(1)

void hardware_init(void);
void s4575272_reg_joystick_init(void);
void s4575272_reg_pantilt_init(void);
void s4575272_reg_lta1000g_init(void);
int s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput);
void s4575272_pantilt_angle_write(int type, int angle);
int s4575272_pantilt_read(int type);
void display_panangle(void);

int main(void)
{
	HAL_Init();
	hardware_init();
	BRD_debuguart_init();  //Initialise UART for debug log output

	// Main Processing Loop
	while (1)
	{	
		// Start ADC conversion for both ADC1 and ADC2
		HAL_ADC_Start(&AdcHandle1); 
		HAL_ADC_Start(&AdcHandle2); 
		
		PanAngle = 0.044 * (S4575272_REG_JOYSTICK_X_READ() + S4575272_REG_JOYSTICK_X_ZERO_CAL_OFFSET) - 90;
		TiltAngle = 0.044 * (S4575272_REG_JOYSTICK_Y_READ() + S4575272_REG_JOYSTICK_Y_ZERO_CAL_OFFSET) - 90;
		
		S4575272_REG_PANTILT_PAN_WRITE(S4575272_REG_PANTILT_PAN_READ());
		S4575272_REG_PANTILT_TILT_WRITE(S4575272_REG_PANTILT_TILT_READ());
		//display_panangle();

		//debug_log("adcX adcY %d %d\n\r",adcx,adcy);

	}
}


void hardware_init(void) {
	s4575272_reg_joystick_init();
	s4575272_reg_pantilt_init();
	s4575272_reg_lta1000g_init();
}

void display_panangle(){
	switch(PanAngle/20) {

		case -4: //bu neng yong loop a
			while(PanAngle/20 == -4) {
				GPIOC->ODR |= (0X01 << 6);
			}
		break;

		case -3:
			while(PanAngle/20 == -3){
				GPIOB->ODR |= (0X01 << 15);
			}		
		break;

		case -2:
			while (PanAngle/20 == -2){
				GPIOB->ODR |= (0X01 << 12);
			}
		break;

		case 0:
			while (PanAngle/20 == 0){
				GPIOB->ODR |= (0X01 << 15);
			}
		break;
	}
	// if (PanAngle < -50){
	// 	GPIOC->ODR |= (0X01 << 6);
	// } else if (PanAngle < -50) {
	// 	GPIOB->ODR |= (0X01 << 15);
	// } else if (PanAngle < -30) {
	// 	GPIOB->ODR |= (0X01 << 13);
	// } else if(PanAngle < -10) {
	// 	GPIOB->ODR |= (0X01 << 12);
	// }
}




