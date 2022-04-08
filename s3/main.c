#include "board.h"
#include "processor_hal.h"
#include "debug_log.h"

#define S0  0  //IDLE State
#define S1  1  //Encode State
#define S2  2  //Decode State
void hardware_init(void);
void s4575272_reg_lta1000g_init(void);
extern void s4575272_reg_joystick_pb_init(void);
void lta1000g_seg_set(int segment, unsigned char segment_value);
extern int s4575272_reg_joystick_press_get(void);
int fsm_processing(int current, char CH1, char CH2);
void state1_processing(char inputChar1, char inputChar2);
void LEDlight(uint8_t codedWord1, uint8_t codedWord2);

unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in);
unsigned short s4575272_lib_hamming_byte_encode(unsigned char input);
unsigned char s4575272_lib_hamming_byte_decode(unsigned char value);
int pbGet;
int p;

int main(void) {

    hardware_init();
	BRD_LEDGreenOff();

    uint32_t prev_tick;
    prev_tick = 0;
    int currentState = S0;
    char recvChar1, recvChar2;
    char inputChar1, inputChar2;
    int collector = 0;
    p = 0;

    while(1) {
        pbGet = s4575272_reg_joystick_press_get() % 2;

        if ((HAL_GetTick() - prev_tick) >= 200) {
            prev_tick =  HAL_GetTick();
            
            if (collector == 0) {
                if ((recvChar1 = BRD_debuguart_getc()) != '\0') {
                    collector = 1;
                    inputChar1 = recvChar1;
                    debug_log("Character1: %c - ASCII Table value: %d\n\r", recvChar1, recvChar1);
                }
            }

            if (collector == 1) {
                if ((recvChar2 = BRD_debuguart_getc()) != '\0') {
                    collector = 0;
                    p = 0;
                    inputChar2 = recvChar2;
                    debug_log("Character2: %c - ASCII Table value: %d\n\r", recvChar2, recvChar2);
                }
            }

            if ((recvChar1 != '\0') && (recvChar2 != '\0')) {
                currentState = fsm_processing(currentState, recvChar1, recvChar2);
            }

            if ((currentState == S1) && (collector == 0)) {
                state1_processing(inputChar1, inputChar2);
            }    
        }
    }

    return 0;
}

void state1_processing(char inputChar1, char inputChar2) {
    if((inputChar1 != 'E' || inputChar2 != 'E')) {
        uint8_t uncodedWord1 = 0;
        uint8_t uncodedWord2 = 0;
        uint8_t codedWord1 = 0;
        uint8_t codedWord2 = 0;
               
        if (inputChar1 >= '1' && inputChar1 <= '9') {
            uncodedWord1 |= ((inputChar1 - 48) << 0);
        } else if (inputChar1 >= 'A' && inputChar1 <= 'F') {
            uncodedWord1 |= ((inputChar1 - 55) << 0);
        }

        if (inputChar2 >= '1' && inputChar2 <= '9') {
            uncodedWord2 |= ((inputChar2 - 48) << 0);
        } else if (inputChar2 >= 'A' && inputChar2 <= 'F') {
            uncodedWord2 |= ((inputChar2 - 55) << 0);
        }

        codedWord1 = s4575272_lib_hamming_hbyte_encode(uncodedWord1);
        codedWord2 = s4575272_lib_hamming_hbyte_encode(uncodedWord2);

        if (p == 0){
            debug_log("uncodedWOrd1 %d\n\r", uncodedWord1);
            debug_log("uncodedWOrd2 %d\n\r", uncodedWord2);
            debug_log("COdedWOrd1 %d\n\r", codedWord1);
            debug_log("COdedWOrd2 %d\n\r", codedWord2);
            p = 1;
        }

        LEDlight(codedWord1, codedWord2);
    }

}

int fsm_processing(int current, char CH1, char CH2) {

    int nextState = S0;
   
    switch(current) {
        case S0:
            if (CH1 == 'E' && CH2 == 'E') {
                nextState = S1;
                debug_log("Congrats we are in the Encode State!\n\r");
            } else if (CH1 == 'D' && CH2 == 'D') {
                nextState = S2;
                debug_log("Congrats we are in the Decode State!\n\r");
            } else {
                nextState = S0;
                debug_log("Press <E><E> to Encode state or <D><D> to Decode state!\n\r");
            }
            break;
        
        case S1:
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

void hardware_init(void) {
    HAL_Init();	
    BRD_debuguart_init();
    s4575272_reg_lta1000g_init(); //Initialise GPIO pins for LED 
    s4575272_reg_joystick_pb_init(); //Initialise GPIO pins for joystick
	BRD_LEDInit();		//Initialise LEDS
}

void LEDlight(uint8_t codedWord1, uint8_t codedWord2) {
    if (pbGet == 0) {
        for (int i = 0; i < 8; i++) {
            if (codedWord1 & (1 << i)) {
                lta1000g_seg_set(i, '1');
            } else {
                lta1000g_seg_set(i, '0');
            }
        }
    } else if (pbGet == 1) {
        for (int i = 0; i < 8; i++) {
            if (codedWord2 & (1 << i)) {
                lta1000g_seg_set(i, '1');
            } else {
                lta1000g_seg_set(i, '0');
            }
        }
    }
}