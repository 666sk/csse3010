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
void lta1000g_seg_set(int segment, unsigned char segment_value);
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
		
		PanAngle = 0.044 * (S4575272_REG_JOYSTICK_X_READ() + S4575272_REG_JOYSTICK_X_ZERO_CAL_OFFSET);    //Calculate the Pan Angle
		TiltAngle = 0.044 * (S4575272_REG_JOYSTICK_Y_READ() + S4575272_REG_JOYSTICK_Y_ZERO_CAL_OFFSET);    //Calculate the TIlt Angle
		
		S4575272_REG_PANTILT_PAN_WRITE(S4575272_REG_PANTILT_PAN_READ());
		S4575272_REG_PANTILT_TILT_WRITE(S4575272_REG_PANTILT_TILT_READ());
		display_panangle();
	}
}

//Initialise Joystick, Pantilt and LED 
void hardware_init(void) {
	s4575272_reg_joystick_init();
	s4575272_reg_pantilt_init();
	s4575272_reg_lta1000g_init();
}

//Display the Pan Angle on the LED Bar, Angles round down
void display_panangle(){

	for (int i = 1; i <= 9; i++) {    //start lighting 1st led from 20 degrees to the last led ending at 180 degrees

		if (i == PanAngle/20) {

			lta1000g_seg_set(i, '1');
		} else {

			lta1000g_seg_set(i, '0');
		}
	}
}




