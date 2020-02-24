#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__
/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

/* 内核对象名称最大长度 */
#define RT_NAME_MAX 8

/* 数据对齐长度 */
#define RT_ALIGN_SIZE 4


#define RT_THREAD_PRIORITY_8

// <o>Maximal level of thread priority <8-256>
//最大支持的优先级
#define RT_THREAD_PRIORITY_MAX 32

//每秒的节拍数
#define RT_TICK_PER_SECOND 1000

//SECTION：调试选项
//打开 RT-Thread 的ASSERT 选项
//#define RT_DEBUG

//打开 RT-Thread 的线程栈溢出检查
#define RT_USING_OVERFLOW_CHECK

#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 512

/* Inter-Thread communication */


/* Memory Management */
//使用内存池
//#define RT_USING_MEMPOOL
//#define RT_USING_MEMHEAP

//使用小内存
#define RT_USING_SMALL_MEM

//使用堆内存
#define RT_USING_HEAP

/* Kernel Device Object */
#define RT_USING_MUTEX
#define RT_USING_SEMAPHORE

//支持设备模块
#define RT_USING_DEVICE

//SECTION: 控制台选项
#define RT_USING_CONSOLE

//控制台缓冲区大小
#define RT_CONSOLEBUF_SIZE 128

#define RT_CONSOLE_DEVICE_NAME "uart22"

#define RT_VER_NUM 0x40002

#define ARCH_ARM

#define RT_USING_CPU_FFS

#define ARCH_ARM_CORTEX_M

#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */


#define RT_USING_COMPONENTS_INIT

//使用 main 线程
#define RT_USING_USER_MAIN

// <o>the size of main thread<1-4086>
#define RT_MAIN_THREAD_STACK_SIZE 512

//main线程优先级 10
#define RT_MAIN_THREAD_PRIORITY 10

/* C++ features */


/* Command shell */

//使用 FINSH 组件
#define RT_USING_FINSH

//FINSH名称
#define FINSH_THREAD_NAME "tshell"

//使用FINSH 历史记录
#define FINSH_USING_HISTORY

//FINSH 历史记录行 5
#define FINSH_HISTORY_LINES 5

#define FINSH_USING_SYMTAB

#define FINSH_USING_DESCRIPTION

#define FINSH_THREAD_PRIORITY 20

#define FINSH_THREAD_STACK_SIZE 4096

#define FINSH_CMD_SIZE 80

#define FINSH_USING_MSH

#define FINSH_USING_MSH_DEFAULT

//#define FINSH_USING_MSH_ONLY

#define FINSH_ARG_MAX 10

/* Device virtual file system */


/* Device Drivers */

#define RT_USING_DEVICE_IPC

#define RT_PIPE_BUFSZ 512

#define RT_USING_SERIAL

#define RT_SERIAL_USING_DMA

#define RT_SERIAL_RB_BUFSZ 64

#define RT_USING_PIN

/* Using USB */


/* POSIX layer and C standard library */


/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */


/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */


/* multimedia packages */


/* tools packages */


/* system packages */


/* peripheral libraries and drivers */


/* miscellaneous packages */


/* samples: kernel and components samples */

#define SOC_FAMILY_STM32
#define SOC_SERIES_STM32F4

/* Hardware Drivers Config */

#define SOC_STM32F429IG

/* Onboard Peripheral Drivers */

#define BSP_USING_USB_TO_USART

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART
#define BSP_USING_UART1

/* Board extended module Drivers */


#endif
