
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
    int collector = 0;
    int flag = 0;
    while(1) {
        
        if ((HAL_GetTick() - prev_tick) >= 300) {
            prev_tick =  HAL_GetTick();
            if (collector == 0) {
                if ((recvChar1 = BRD_debuguart_getc()) != '\0') {
                    collector = 1;
                    
                    debug_log("Character1: %c - ASCII Table value: %d\n\r", recvChar1, recvChar1);
                }
            }

            if (collector == 1) {
                if ((recvChar2 = BRD_debuguart_getc()) != '\0') {
                    collector = 0;
                    debug_log("Character2: %c - ASCII Table value: %d\n\r", recvChar2, recvChar2);
                }
            }

            if ((recvChar1 != '\0') && (recvChar2 != '\0')) {
                currentState = fsm_processing(currentState, recvChar1, recvChar2);
                //debug_log("Sys time %d\n\r", HAL_GetTick());
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
                BRD_LEDGreenOn();
                uint8_t uncodedWord1 = 0;
                uint8_t uncodedWord2 = 0;
                uint8_t codedWord1;
                uint8_t codedWord2;
                
                //uint16_t codedWord;
                if (47 < CH1 < 58) {
                    uncodedWord1 |= ((CH1-48) << 0);
                } else if (64 < CH1 < 71) {
                    uncodedWord1 |= ((CH1-55) << 0);
                } else {
                    return S1;
                }
                if (47 < CH2 < 58) {
                    uncodedWord2 |= ((CH2-48) << 0);
                } else if (64 < CH2 < 71) {
                    uncodedWord2 |= ((CH2-55) << 0);
                } else {
                    return S1;
                }
                //encode here?
                
                    debug_log("uncodedWOrd1 %d\n\r", uncodedWord1);
                    debug_log("uncodedWOrd2 %d\n\r", uncodedWord2);
                    codedWord1 = s4575272_lib_hamming_hbyte_encode(uncodedWord1);
                    codedWord2 = s4575272_lib_hamming_hbyte_encode(uncodedWord2);
                    debug_log("COdedWOrd1 %d\n\r", codedWord1);
                    debug_log("COdedWOrd2 %d\n\r", codedWord2);
                

                for (int i = 0; i < 8; i++) {
                    if (codedWord1 & (1 << i)) {
                        lta1000g_seg_set(i, '1');
                    } else {
                        lta1000g_seg_set(i, '0');
                    }
                }
             
                
            }
            break;

        case S2:
            if (CH1 == 'F' && CH2 == 'F') {
                nextState = S0;
            } else if (CH1 == 'E' && CH2 == 'E') {
                nextState = S1;
            } else {
                nextState = S2;
                BRD_LEDBlueOn();
                uint8_t codedWord;
                uint8_t decodedWord;
                if (47 < CH1 < 58) {
                    codedWord |= ((CH1-48) << 4);
                } else if (64 < CH1 < 71) {
                    codedWord |= ((CH1-55) << 4);
                } else {
                    return S2;
                }
                if (47 < CH2 < 58) {
                    codedWord |= ((CH2-48) << 0);
                } else if (64 < CH2 < 71) {
                    codedWord |= ((CH2-55) << 0);
                } else {
                    return S2;
                }
                debug_log("codedWOrd %d\n\r", codedWord);
                decodedWord = s4575272_lib_hamming_byte_decode(codedWord);
                debug_log("decodedWord %d\n\r", decodedWord);


            }
            break;
    }

    return nextState;
}