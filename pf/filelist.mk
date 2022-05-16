########################################################################
# LIBRARY SOURCES - MUST BE IN THE SAME FOLDER as main.c (DO NOT CHANGE)
########################################################################

CPUFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CPUFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

MYLIB_PATH=$(SOURCELIB_ROOT)/../repo/mylib
SSD1306_PATH=$(SOURCELIB_ROOT)/components/peripherals/ssd1306/nucleo-f429
FREERTOS_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS

CFLAGS += -I$(MYLIB_PATH)
CFLAGS += -I$(SSD1306_PATH)

CFLAGS += -DUSE_FREERTOS_SYSTICK
CFLAGS += -I$(FREERTOS_PATH)/include -I$(FREERTOS_PATH)/portable/GCC/ARM_CM4F

LIBSRCS += $(MYLIB_PATH)/s4575272_joystick.c 
LIBSRCS += $(MYLIB_PATH)/s4575272_lta1000g.c 
LIBSRCS += $(MYLIB_PATH)/s4575272_oled.c
LIBSRCS += $(MYLIB_PATH)/s4575272_CAG_simulator.c
LIBSRCS += $(MYLIB_PATH)/s4575272_CAG_display.c
LIBSRCS += $(MYLIB_PATH)/s4575272_CAG_grid.c
LIBSRCS += $(MYLIB_PATH)/s4575272_CAG_mnemonic.c
LIBSRCS += $(MYLIB_PATH)/s4575272_CAG_joystick.c
LIBSRCS += $(wildcard $(SSD1306_PATH)/*.c)
LIBSRCS += $(FREERTOS_PATH)/portable/MemMang/heap_1.c