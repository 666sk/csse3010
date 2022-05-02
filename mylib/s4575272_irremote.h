 /** 
 **************************************************************
 * @file mylib/s457527_irremote.h
 * @author Kuang Sheng - 45752720
 * @date 07/03/2022
 * @brief mylib irremote driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * void s4575272_reg_irremote_init(void) - Initialize the IR receiver hardware
 * void s4575272_reg_irremote_recv(void) - function for processing the received input waveform
 * int s4575272_reg_irremote_readkey(char* value) - Check if a key has been detected. Return 1 if detected and set in value parameter.
 *************************************************************** 
 */
#define TIMER_COUNTER_FREQ  500000
#define TIMER_1SECOND_PERIOD   1000  //2ms

float msCount = 0;
// uint16_t msCount = 0;
uint8_t timerValue;
int pulseCount = 0;
uint32_t bitPattern = 0; //newKey = 0;
char* newKey;

//Initialize the IR receiver hardware
void s4575272_reg_irremote_init(void);

//function for processing the received input waveform
void s4575272_reg_irremote_recv(void);

//Internal flag set if a key is pressed
int keyPressedFlag = 0;

//Internal buffer to store the lastest key that has been pressed
char keyPressedValue;

//Check if a key has been detected. Return 1 if detected and set in value parameter.
int s4575272_reg_irremote_readkey(char* value);  //value shi buffer
