 /** 
 **************************************************************
 * @file mylib/s457527_CAG_menmonic.h
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


#ifndef __S4575272_CAG_mnemonic_H__
#define __S4575272_CAG_mnemonic_H__



#define CAG_MNEMONIC_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 6 )
#define CAG_MNEMONIC_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )

void s4575272_tsk_CAG_mnemonic_init(void);
void s4575272TaskCAG_Mnemonic(void);

#endif