#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"


ADC_HandleTypeDef AdcHandle1,AdcHandle2;
ADC_ChannelConfTypeDef AdcChanConfig1,AdcChanConfig2;

#define TIMER_RUNNING_FREQ  		500000
#define TIMER_40MS_PERIOD_TICKS		20000								
#define PWM_PERCENT2TICKS_DUTYCYCLE(value)	(value*TIMER_40MS_PERIOD_TICKS/100)

void hardware_init(void);
void s4575272_reg_joystick_init(void);
void s4575272_reg_pantilt_init(void);
int s4575272_joystick_readxy(ADC_HandleTypeDef AdcHandleInput);
void s4575272_pantilt_angle_write(int type, float angle);
#define S4575272_REG_JOYSTICK_X_READ() (s4575272_joystick_readxy(AdcHandle1))
#define S4575272_REG_JOYSTICK_Y_READ() (s4575272_joystick_readxy(AdcHandle2))

int main(void)
{


	HAL_Init();
	hardware_init();
	BRD_debuguart_init();  //Initialise UART for debug log output

	float PanAngle;
	float TiltAngle;

	// Main Processing Loop
	while (1)
	{	
		
		// Start ADC conversion for both ADC1 and ADC2
		HAL_ADC_Start(&AdcHandle1); 
		HAL_ADC_Start(&AdcHandle2); 
		

		//debug_log("adc_value and adc_value1 %d %d\n\r",adc_value, adc_value1);
		PanAngle = 0.04395 * S4575272_REG_JOYSTICK_X_READ() - 90;
		TiltAngle = 0.04395 * S4575272_REG_JOYSTICK_Y_READ() - 90;
		s4575272_pantilt_angle_write(0, PanAngle); 
		s4575272_pantilt_angle_write(1, TiltAngle); 
		debug_log("panangle %f\n\r",PanAngle);


		//HAL_Delay(300);
	}
}


void hardware_init(void) {
	s4575272_reg_joystick_init();
	s4575272_reg_pantilt_init();
}





