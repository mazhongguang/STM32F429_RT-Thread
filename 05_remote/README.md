# STM32F429_HAL
RT-Thread学习

操作系统：LINUX（Ubuntu 19.10)
编译器：GCC（gcc-arm-none-eabi-6-2017-q2-update）
仿真器：JLINK V9.4

描述：
	2020.02.28	添加红外线相关驱动（直接复制裸机驱动使用）。
				添加ILI9341的LCD驱动（直接复制裸机驱动使用），删除驱动初始化中的printf()打印LCD ID的函数，GCC不支持。

RT-Thread RTOS移植
实验现象：
	遥控器按键
	TEST ->	LED_GREEN,LED_WHITE,BEEP同时开启
	PWR_OFF	->	LED_GREEN,LED_WHITE,BEEP同时关闭
	MENU	->	流水灯
	播放	->	BEEP开启2s
	+		->	LED_GREEN慢闪
	-		->	LED_WHITE慢闪
	快进	->	LED_GREEN快闪
	快退	->	LED_WHITE快闪
