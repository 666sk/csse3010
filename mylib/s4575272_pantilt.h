

#ifndef __S4575272_PANTILT_H__
#define __S4575272_PANTILT_H__

void s4575272_reg_pantilt_init();
void s4575272_pantilt_angle_write(int type, float angle);
void s4575272_pantilt_read(int type);

#define S4575272_REG_PANTILT_PAN_90_CAL_OFFSET
#define S4575272_REG_PANTILT_TILT_90_CAL_OFFSET

#endif