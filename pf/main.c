#include "FreeRTOS.h"
#include "board.h"
#include "processor_hal.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4575272_CAG_simulator.h"


int main(void) {
    HAL_Init();
    
    s4575272_tsk_CAG_simulator_init();
    vTaskStartScheduler();

    return 0;
}