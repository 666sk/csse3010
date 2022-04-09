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
void state1_processing(int state, char inputChar1, char inputChar2);
void state2_processing(char inputChar1, char inputChar2);
void LEDlight(uint8_t codedWord1, uint8_t codedWord2);
void LEDlight2(uint8_t decodedWord);
void LED_turning_off(void);

unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in);
unsigned short s4575272_lib_hamming_byte_encode(unsigned char input);
unsigned char s4575272_lib_hamming_byte_decode(unsigned char value);
int s4575272_lib_hamming_parity_error(unsigned char value);
int s4575272_lib_hamming_bit_error(void);
int parity;
int bitError;
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

        if ((HAL_GetTick() - prev_tick) >= 50) {
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

            if (((currentState == S1) || (currentState == S2)) && (collector == 0)) {
                state1_processing(currentState, inputChar1, inputChar2);
            }    
        }
    }

    return 0;
}

void state1_processing(int state, char inputChar1, char inputChar2) {
    //if((inputChar1 != 'E' || inputChar2 != 'E')) {
        uint8_t inputWord1 = 0;
        uint8_t inputWord2 = 0;
        uint8_t codedWord1 = 0;
        uint8_t codedWord2 = 0;
        uint8_t decodedWord = 0;
        //uint8_t decodedWord2 = 0;
               
        if (inputChar1 >= '0' && inputChar1 <= '9') {
            inputWord1 |= ((inputChar1 - 48) << 0);
        } else if (inputChar1 >= 'A' && inputChar1 <= 'F') {
            inputWord1 |= ((inputChar1 - 55) << 0);
        }

        if (inputChar2 >= '0' && inputChar2 <= '9') {
            inputWord2 |= ((inputChar2 - 48) << 0);
        } else if (inputChar2 >= 'A' && inputChar2 <= 'F') {
            inputWord2 |= ((inputChar2 - 55) << 0);
        }

        if((inputChar1 != 'E' || inputChar2 != 'E') && state == S1) {
            codedWord1 = s4575272_lib_hamming_hbyte_encode(inputWord1);
            codedWord2 = s4575272_lib_hamming_hbyte_encode(inputWord2);

            if (p == 0){
                debug_log("inputWord1 %d\n\r", inputWord1);
                debug_log("inputWord2 %d\n\r", inputWord2);
                debug_log("COdedWOrd1 %d\n\r", codedWord1);
                debug_log("COdedWOrd2 %d\n\r", codedWord2);
                p = 1;
            }
            LEDlight(codedWord1, codedWord2);
        }else if ((inputChar1 != 'D' || inputChar2 != 'D') && state == S2) {
            inputWord1 = inputWord1 << 4;
            inputWord1 |= inputWord2 << 0;
            decodedWord = s4575272_lib_hamming_byte_decode(inputWord1);
            if (p == 0){
                if (s4575272_lib_hamming_parity_error(parity)){
                    debug_log("ok PARITY get and parity error occur\n\r");
                }
                if (s4575272_lib_hamming_bit_error()) {
                    debug_log("ok one-bit error get and error occur\n\r");
                }
                debug_log("decodedword %d\n\r", decodedWord);
                p = 1;
            }
            LEDlight2(decodedWord);
        }

        //LEDlight(codedWord1, codedWord2);
    //}

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
                LED_turning_off();
                nextState = S0;
            } else if (CH1 == 'D' && CH2 == 'D') {
                LED_turning_off();
                nextState = S2;
            } else if (CH1 == 'E' && CH2 == 'E') {
                LED_turning_off();
                nextState = S1;
            } else {
                nextState = S1;
            }
            break;

        case S2:
            if (CH1 == 'F' && CH2 == 'F') {
                LED_turning_off();
                nextState = S0;
            } else if (CH1 == 'E' && CH2 == 'E') {
                LED_turning_off();
                nextState = S1;
            } else if (CH1 == 'D' && CH2 == 'D') {  
                LED_turning_off();
                nextState = S2;
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
    int i,j;
    if (pbGet == 0) {
        for (i = 7, j = 0; i >= 0, j <= 7; i--, j++) {
            if (codedWord1 & (1 << i)) {
                lta1000g_seg_set(j, '1');
            } else {
                lta1000g_seg_set(j, '0');
            }
        }
    } else if (pbGet == 1) {
        for (i = 7,j = 0; i >= 0, j <= 7; i--, j++) {
            if (codedWord2 & (1 << i)) {
                lta1000g_seg_set(j, '1');
            } else {
                lta1000g_seg_set(j, '0');
            }
        }
    }
    lta1000g_seg_set(8, '0');
    lta1000g_seg_set(9, '0');
}

void LEDlight2(uint8_t decodedWord) {
    int i,j,k;

    for (i = 3, j = 0; i >= 0, j <= 3; i--, j++) {
        if (decodedWord & (1 << i)) {
            lta1000g_seg_set(j, '1');
        } else {
            lta1000g_seg_set(j, '0');
        }
    }

    for (k = 4; k <= 7; k++) {
        lta1000g_seg_set(k, '0');
    }

    if (s4575272_lib_hamming_bit_error()) {
        lta1000g_seg_set(9, '1');
    } else {
        lta1000g_seg_set(9, '0');
    }

    if (s4575272_lib_hamming_parity_error(parity)){
        lta1000g_seg_set(8, '1');
    } else {
        lta1000g_seg_set(8, '0');
    }
}

void LED_turning_off(void) {
    for (int i = 0; i <=9; i++) {
        lta1000g_seg_set(i, '0');
    }
}