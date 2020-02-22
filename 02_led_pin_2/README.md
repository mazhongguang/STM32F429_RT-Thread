# STM32F429_HAL
RT-Thread学习

操作系统：LINUX（Ubuntu 19.10)
编译器：GCC（gcc-arm-none-eabi-6-2017-q2-update）
仿真器：JLINK V9.4

描述：
	1.在drv_gpio.h中添加所有和PIN相关的宏定义（GPIOA-GPIOI）。

RT-Thread RTOS移植
实验现象：
	LED_GREEN和LED_WHITE以500TICK（500ms)的频率和以下排列进行循环显示，同时在通过串口打印出LED 的开关状态。
	GREEN		WHITE
	X			X
	O			X
	X			O
	O			O

