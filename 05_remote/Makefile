######################################
# target
######################################
TARGET = \
remote

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
include src.mk
#C_SOURCES =  \
HALLIB/Src/stm32f4xx_hal_tim.c \
HALLIB/Src/stm32f4xx_hal_tim_ex.c \
HALLIB/Src/stm32f4xx_hal_rcc.c \
HALLIB/Src/stm32f4xx_hal_rcc_ex.c \
HALLIB/Src/stm32f4xx_hal_flash.c \
HALLIB/Src/stm32f4xx_hal_flash_ex.c \
HALLIB/Src/stm32f4xx_hal_flash_ramfunc.c \
HALLIB/Src/stm32f4xx_hal_gpio.c \
HALLIB/Src/stm32f4xx_hal_dma.c \
HALLIB/Src/stm32f4xx_hal_pwr.c \
HALLIB/Src/stm32f4xx_hal_pwr_ex.c \
HALLIB/Src/stm32f4xx_hal_cortex.c \
HALLIB/Src/stm32f4xx_hal.c \
HALLIB/Src/stm32f4xx_hal_sdram.c \
HALLIB/Src/stm32f4xx_hal_sram.c \
HALLIB/Src/stm32f4xx_ll_fmc.c \
HALLIB/Src/stm32f4xx_hal_can.c \
HALLIB/Src/stm32f4xx_hal_uart.c \
hardware/KEY/key.c \
hardware/LCD/font.c \
hardware/LCD/ili9341.c \
hardware/LED/led.c \
hardware/CAN/can.c \
hardware/SDRAM/sdram.c \
SYSTEM/delay/delay.c \
SYSTEM/sys/sys.c \
SYSTEM/usart/usart.c \
USER/main.c \
USER/stm32f4xx_it.c \
USER/system_stm32f4xx.c \
USER/stm32f4xx_hal_msp.c \
USER/board.c 

# ASM sources
#ASM_SOURCES =  \
CORE/startup_stm32f429xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard
#FLOAT-ABI = -mfloat-abi=softfp
#FLOAT-ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 


include cpppath.mk
# C def#ines
#C_DEFS =  \
#-DUSE_HAL_DRIVER \
#-DSTM32F429xx


## AS includes
#AS_INCLUDES = 

## C includes
#C_INCLUDES =  \
#-ICORE \
#-IHALLIB/Inc \
#-IHALLIB/Inc/Legacy \
#-Ihardware/KEY \
#-Ihardware/CAN \
#-Ihardware/LCD \
#-Ihardware/LED \
#-Ihardware/SDRAM \
#-Ihardware/STMFLASH \
#-Ihardware/TIM \
#-Ihardware/WKUP \
#-ISYSTEM/delay \
#-ISYSTEM/sys \
#-ISYSTEM/usart \
#-IUSER


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections 

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

ASFLAGS += -Wa,-mimplicit-it=thumb
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = board/linker_scripts/link.lds
#LDSCRIPT = STM32F429IGTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
