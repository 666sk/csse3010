/**
  ******************************************************************************
  * @file    stage3/main.c
  * @author  Kuang Sheng
  * @date    09/04/2022
  * @brief   Nucleo429ZI onboard stage3 design task.
  ******************************************************************************
  */

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
void state_processing(int state, char inputChar1, char inputChar2);
void state1_led_output(uint8_t codedWord1, uint8_t codedWord2);
void state2_led_output(uint8_t decodedWord);
void LED_turning_off(void);
void s4575272_reg_irremote_init(void);
void tui_callback(void);
void TIM2_IRQHandler(void);

unsigned char s4575272_lib_hamming_hbyte_encode(unsigned char in);
unsigned short s4575272_lib_hamming_byte_encode(unsigned char input);
unsigned char s4575272_lib_hamming_byte_decode(unsigned char value);
int s4575272_lib_hamming_parity_error(unsigned char value);
int s4575272_lib_hamming_bit_error(void);
int parity;
int bitError;
int pbGet;
int printFlag;

//Main Cyclic Executive
int main(void) {

    hardware_init();
    uint32_t prev_tick;
    prev_tick = 0;
    int currentState = S0;
    char recvChar1, recvChar2;
    char inputChar1, inputChar2;
    int collector = 0;
    printFlag = 0;

    while(1) {

        pbGet = s4575272_reg_joystick_press_get() % 2;

        if ((HAL_GetTick() - prev_tick) >= 50) {
            prev_tick =  HAL_GetTick();
            
            if (collector == 0) {
                if ((recvChar1 = BRD_debuguart_getc()) != '\0') {
                    collector = 1;
                    inputChar1 = recvChar1;
                    debug_log("InputWord1: %c - ASCII Table value: %d\n\r", recvChar1, recvChar1);
                }
            }

            if (collector == 1) {
                if ((recvChar2 = BRD_debuguart_getc()) != '\0') {
                    collector = 0;
                    printFlag = 0;
                    inputChar2 = recvChar2;
                    debug_log("InputWord1: %c - ASCII Table value: %d\n\r", recvChar2, recvChar2);
                }
            }

            //only process input characters in pairs
            if ((recvChar1 != '\0') && (recvChar2 != '\0')) {
                currentState = fsm_processing(currentState, recvChar1, recvChar2);
            }

            if (((currentState == S1) || (currentState == S2)) && (collector == 0)) {
                state_processing(currentState, inputChar1, inputChar2);
            }    
        }
    }

    return 0;
}

//Encode in Encode state and Decode in Decode state
void state_processing(int state, char inputChar1, char inputChar2) {

        uint8_t inputWord1 = 0;
        uint8_t inputWord2 = 0;
        uint8_t codedWord1 = 0;
        uint8_t codedWord2 = 0;
        uint8_t decodedWord = 0;

        //Define the input range       
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

        //Process Encode state
        if((inputChar1 != 'E' || inputChar2 != 'E') && state == S1) {

            codedWord1 = s4575272_lib_hamming_hbyte_encode(inputWord1);
            codedWord2 = s4575272_lib_hamming_hbyte_encode(inputWord2);

            if (printFlag == 0){

                debug_log("CodedWord1 %d\n\r", codedWord1);
                debug_log("CodedWord2 %d\n\r", codedWord2);
                printFlag = 1;
            }

            state1_led_output(codedWord1, codedWord2);  //Display in LED
        }else if ((inputChar1 != 'D' || inputChar2 != 'D') && state == S2) {    //Process the Decode state

            inputWord1 = inputWord1 << 4;
            inputWord1 |= inputWord2 << 0;
            decodedWord = s4575272_lib_hamming_byte_decode(inputWord1);

            if (printFlag == 0){
                if (s4575272_lib_hamming_parity_error(parity)){
                    debug_log("There is a parity error occur!\n\r");
                }

                if (s4575272_lib_hamming_bit_error()) {
                    debug_log("There is a one-bit error occur!\n\r");
                }

                debug_log("Decodedword %d\n\r", decodedWord);
                printFlag = 1;
            }

            state2_led_output(decodedWord);
        }

}

//Process fsm state transition
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
                debug_log("Congrats we are in the Decode State!\n\r");
                nextState = S2;
            } else if (CH1 == 'E' && CH2 == 'E') {
                LED_turning_off();
                debug_log("Congrats we are in the Encode State!\n\r");
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
                debug_log("Congrats we are in the Encode State!\n\r");
                nextState = S1;
            } else if (CH1 == 'D' && CH2 == 'D') {  
                LED_turning_off();
                debug_log("Congrats we are in the Decode State!\n\r");
                nextState = S2;
            } else {
                nextState = S2;
            }
            break;
    }

    return nextState;
}

//Initialize the hardware
void hardware_init(void) {
    HAL_Init();	  //Initialise HAL library
    BRD_debuguart_init();  //Initialise debuglog
    s4575272_reg_lta1000g_init(); //Initialise GPIO pins for LED 
    s4575272_reg_joystick_pb_init(); //Initialise GPIO pins for joystick
	BRD_LEDInit();		//Initialise LEDS
    s4575272_reg_irremote_init();
}

//Display the encoded output in LED
void state1_led_output(uint8_t codedWord1, uint8_t codedWord2) {
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

//Display the decoded output in LED
void state2_led_output(uint8_t decodedWord) {
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

//Turn off all LED segments
void LED_turning_off(void) {
    for (int i = 0; i <=9; i++) {
        lta1000g_seg_set(i, '0');
    }
}
