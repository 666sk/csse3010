 /** 
 **************************************************************
 * @file mylib/s457527_CAG_menmonic.c
 * @author Kuang Sheng - 45752720
 * @date 15/05/2022
 * @brief mylib CAG mnemonic driver
 * REFERENCE:  
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 *
 *************************************************************** 
 */

#include "s4575272_CAG_mnemonic.h"
#include "s4575272_cli_CAG_mnemonic.h"


//Initialize the task of mnemonic mode
void s4575272_tsk_CAG_mnemonic_init(void) {

    FreeRTOS_CLIRegisterCommand(&xStill);
    
    xTaskCreate(
        (void *) &s4575272TaskCAG_Mnemonic,     // Function that implements the task
        (const signed char *) "TaskCAG_Mnemonic",   // Text name for the task
        CAG_MNEMONIC_TASK_STACK_SIZE,            // Stack size in words, not bytes
        NULL,                           // No Parameter needed
        CAG_MNEMONIC_TASK_PRIORITY,              // Priority at which the task is created
        NULL);  
}

void s4575272TaskCAG_Mnemonic(void) {
    
}