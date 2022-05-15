
#include "s4575272_CAG_simulator.h"
#include "s4575272_CAG_display.h"
#include "s4575272_CAG_grid.h"

int main(void) {
    HAL_Init();


    s4575272_tsk_CAG_simulator_init();
    s4575272_tsk_CAG_grid_init();
    s4575272_tsk_CAG_display_init();
    
    vTaskStartScheduler();

    return 0;
}