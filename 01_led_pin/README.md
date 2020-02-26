# STM32F429_HAL
RT-Thread学习

操作系统：LINUX（Ubuntu 19.10)
编译器：GCC（gcc-arm-none-eabi-6-2017-q2-update）
仿真器：JLINK V9.4

描述：
	1.增加 rt_kprintf()函数，方便在PC端使用串口进行调试
		2020.02.26  -> 增加 rt_hw_usart_init()函数进行串口初始化，禁用rt_kprintf()函数，使用内置控制台输出。（控制台为‘usart1',输出为串口1）	
	2.使用 pin通过驱动模块来驱动LED
	
	2020.02.26
		3.修改链接文件为 /board/linker_scripts/link.lds (内部有关于finsh相关的section的相关设置及优化，不然finsh组件无法使用）
备注：
	rt_pin_mode 相关函数使用之前需要先进行设备注册，本实验在board.c中的rt_hw_board_init()中加入rt_hw_init()函数注册。

RT-Thread RTOS移植
实验现象：
	LED_GREEN以500TICK（500ms)的频率闪烁255次后关闭，同时在通过串口打印出LED ON/OFF 及次数。

