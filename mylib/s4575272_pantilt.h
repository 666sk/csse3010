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
#ifndef __S4575272_PANTILT_H__
#define __S4575272_PANTILT_H__

extern int PanAngle;
extern int TiltAngle;

#define S4575272_REG_PANTILT_PAN_WRITE(angle) s4575272_pantilt_angle_write(0, angle)
#define S4575272_REG_PANTILT_PAN_READ() s4575272_pantilt_read(0)
#define S4575272_REG_PANTILT_TILT_WRITE(angle) s4575272_pantilt_angle_write(1, angle)
#define S4575272_REG_PANTILT_TILT_READ() s4575272_pantilt_read(1)
#define S4575272_REG_PANTILT_PAN_90_CAL_OFFSET  2.38    //Calibrate the pan angle to centre at 90 degrees
#define S4575272_REG_PANTILT_TILT_90_CAL_OFFSET 2.03    //Calibrate the tilt angle to centre at 90 degrees

void s4575272_reg_pantilt_init();
void s4575272_pantilt_angle_write(int type, int angle);
int s4575272_pantilt_read(int type);

#endif