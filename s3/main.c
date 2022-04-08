#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"

#define S0  0  //IDLE State
#define S1  1  //Encode State
#define S2  2  //Decode State
void s4575272_reg_lta1000g_init(void);
extern void s4575272_reg_joystick_pb_init(void);
void lta1000g_seg_set(int segment, unsigned char segment_value);
extern int s4575272_reg_joystick_press_get(void);
int fsm_processing(int current, char CH1, char CH2);
unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in);
unsigned short s4575272_lib_hamming_byte_encode(unsigned char input);
unsigned char s4575272_lib_hamming_byte_decode(unsigned char value);
int pb;

int main(void) {

    HAL_Init();	
    BRD_debuguart_init();
    s4575272_reg_lta1000g_init(); //Initialise GPIO pins for LED 
    s4575272_reg_joystick_pb_init(); //Initialise GPIO pins for joystick
	BRD_LEDInit();		//Initialise LEDS
	// Turn off LEDs
	BRD_LEDGreenOff();

    uint32_t prev_tick;
    prev_tick = 0;
    int currentState = S0;
    char recvChar1;
    char recvChar2;
    char a,b;
    int collector = 0;
    int flag = 0;
    int p = 0;
    while(1) {
        pb = s4575272_reg_joystick_press_get() % 2;
        if ((HAL_GetTick() - prev_tick) >= 300) {
            prev_tick =  HAL_GetTick();
            if (collector == 0) {
                if ((recvChar1 = BRD_debuguart_getc()) != '\0') {
                    collector = 1;
                    a = recvChar1;
                    debug_log("Character1: %c - ASCII Table value: %d\n\r", recvChar1, recvChar1);
                }
            }

            if (collector == 1) {
                if ((recvChar2 = BRD_debuguart_getc()) != '\0') {
                    collector = 0;
                    p = 0;
                    b = recvChar2;
                    debug_log("Character2: %c - ASCII Table value: %d\n\r", recvChar2, recvChar2);
                }
            }

            if ((recvChar1 != '\0') && (recvChar2 != '\0')) {
                currentState = fsm_processing(currentState, recvChar1, recvChar2);
                //debug_log("Sys time %d\n\r", HAL_GetTick());
            }
            if ((currentState == S1) && (collector == 0) && (a!='E') && (b != 'E')) {
                uint8_t uncodedWord1 = 0;
                uint8_t uncodedWord2 = 0;
                uint8_t codedWord1;
                uint8_t codedWord2;
               
                if (a >= '1' && a <= '9') {  //ALWAYS BE TRUE. ITS A PROBLEM
                    uncodedWord1 |= ((a - 48) << 0);
                } else if (a >= 'A' && a <= 'F') {
                    uncodedWord1 |= ((a - 55) << 0);
                }

                if (a >= '1' && a <= '9') {
                    uncodedWord2 |= ((b - 48) << 0);
                } else if (a >= 'A' && a <= 'F') {
                    uncodedWord2 |= ((b - 55) << 0);
                }
                
                if (p == 0){
                    debug_log("uncodedWOrd1 %d\n\r", uncodedWord1);
                    debug_log("uncodedWOrd2 %d\n\r", uncodedWord2);
                    codedWord1 = s4575272_lib_hamming_hbyte_encode(uncodedWord1);
                    codedWord2 = s4575272_lib_hamming_hbyte_encode(uncodedWord2);
                    debug_log("COdedWOrd1 %d\n\r", codedWord1);
                    debug_log("COdedWOrd2 %d\n\r", codedWord2);
                    p = 1;
                }

                if (pb == 0) {
                    for (int i = 0; i < 8; i++) {
                        if (codedWord1 & (1 << i)) {
                            lta1000g_seg_set(i, '1');
                        } else {
                            lta1000g_seg_set(i, '0');
                        }
                    }
                } else if (pb == 1) {
                    for (int i = 0; i < 8; i++) {
                        if (codedWord2 & (1 << i)) {
                            lta1000g_seg_set(i, '1');
                        } else {
                            lta1000g_seg_set(i, '0');
                        }
                    }
                }
                
            }    
        }
    }

    return 0;
}

int fsm_processing(int current, char CH1, char CH2) {

    int nextState = S0;
   

    switch(current) {
        case S0:
            if (CH1 == 'E' && CH2 == 'E') {
                nextState = S1;
            } else if (CH1 == 'D' && CH2 == 'D') {
                nextState = S2;
            } else {
                nextState = S0;
            }
            break;
        
        case S1:
        debug_log("Congrats head into Encode State!\n\r");
            if (CH1 == 'F' && CH2 == 'F') {
                nextState = S0;
            } else if (CH1 == 'D' && CH2 == 'D') {
                nextState = S2;
            } else {
                nextState = S1;
            }
            break;

        case S2:
            if (CH1 == 'F' && CH2 == 'F') {
                nextState = S0;
            } else if (CH1 == 'E' && CH2 == 'E') {
                nextState = S1;
            } else {
                nextState = S2;
            }
            break;
    }

    return nextState;
}
