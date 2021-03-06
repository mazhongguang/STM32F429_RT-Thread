C_SOURCES = \
board/board.c \
hardware/ap3216c/ap3216c.c \
hardware/ap3216c/sensor_lsc_ap3216c.c \
hardware/CAN/can.c \
hardware/KEY/key.c \
hardware/LCD/font.c \
hardware/LCD/ili9341.c \
hardware/LED/led.c \
hardware/pcf8574/pcf8574.c \
hardware/pcf8574/pcf8574_sample.c \
hardware/REMOTE/remote.c \
hardware/SDRAM/sdram.c \
rt-thread/components/drivers/sensors/sensor.c \
rt-thread/components/drivers/sensors/sensor_cmd.c \
rt-thread/components/drivers/serial/serial.c \
rt-thread/components/drivers/src/completion.c \
rt-thread/components/drivers/src/dataqueue.c \
rt-thread/components/drivers/src/pipe.c \
rt-thread/components/drivers/src/ringblk_buf.c \
rt-thread/components/drivers/src/ringbuffer.c \
rt-thread/components/drivers/src/waitqueue.c \
rt-thread/components/drivers/src/workqueue.c \
rt-thread/components/finsh/cmd.c \
rt-thread/components/finsh/finsh_compiler.c \
rt-thread/components/finsh/finsh_error.c \
rt-thread/components/finsh/finsh_heap.c \
rt-thread/components/finsh/finsh_init.c \
rt-thread/components/finsh/finsh_node.c \
rt-thread/components/finsh/finsh_ops.c \
rt-thread/components/finsh/finsh_parser.c \
rt-thread/components/finsh/finsh_token.c \
rt-thread/components/finsh/finsh_var.c \
rt-thread/components/finsh/finsh_vm.c \
rt-thread/components/finsh/msh.c \
rt-thread/components/finsh/msh_file.c \
rt-thread/components/finsh/shell.c \
rt-thread/components/finsh/symbol.c \
rt-thread/libcpu/arm/cortex-m4/cpuport.c \
rt-thread/src/clock.c \
rt-thread/src/components.c \
rt-thread/src/cpu.c \
rt-thread/src/device.c \
rt-thread/src/idle.c \
rt-thread/src/ipc.c \
rt-thread/src/irq.c \
rt-thread/src/kservice.c \
rt-thread/src/mem.c \
rt-thread/src/memheap.c \
rt-thread/src/mempool.c \
rt-thread/src/object.c \
rt-thread/src/scheduler.c \
rt-thread/src/signal.c \
rt-thread/src/slab.c \
rt-thread/src/thread.c \
rt-thread/src/timer.c \
SYSTEM/delay/delay.c \
SYSTEM/sys/sys.c \
USER/main.c \
USER/stm32f4xx_hal_msp.c \
USER/stm32f4xx_it.c \
USER/system_stm32f4xx.c \
hardware/HAL_Drivers/drv_gpio.c \
hardware/HAL_Drivers/drv_i2c.c \
hardware/HAL_Drivers/drv_usart.c \
rt-thread/components/drivers/misc/pin.c \
rt-thread/components/drivers/i2c/i2c-bit-ops.c \
rt-thread/components/drivers/i2c/i2c_core.c \
rt-thread/components/drivers/i2c/i2c_dev.c \

include hal_src.mk



ASM_SOURCES =  \
rt-thread/libcpu/arm/cortex-m4/context_gcc.s \
CORE/startup_stm32f429xx.s


GCC_PATH = /opt/gcc-arm-none-eabi-6-2017-q2-update/bin


