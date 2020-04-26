#include "finsh_api.h"
#include "lcd.h"
#include "delay.h"
#include "rtdef.h"
#include "rtthread.h"
#include <stdio.h>

SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄(用于控制LCD)

//LCD的画笔颜色和背景色	   
u32 POINT_COLOR=0xFF000000;		//画笔颜色
//u32 BACK_COLOR =0xFFFFFFFF;  	//背景色 
u32 BACK_COLOR =GBLUE;  	//背景色 

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
	   
//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(vu16 regval)
{   
	regval=regval;		//使用-O2优化的时候,必须插入的延时
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(vu16 data)
{	  
	data=data;			//使用-O2优化的时候,必须插入的延时
	LCD->LCD_RAM=data;		 
}
//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
/*******************************************************************************************
**从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。         										  **
**通过该函数转换																    																		  **		
**c:BGR格式的颜色值																  																	    **
**返回值：RGB格式的颜色值													  																	    **					
********************************************************************************************/
u16 LCD_BGR2RGB(u16 c)
{
	u16 r,g,b,rgb;
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;
	rgb=(b<<11)+(g<<5)+(r<<0);
	return rgb;
}
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u32 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)
		return 0;	//超过了范围,直接返回 
	LCD_SetCursor(x,y);	    
	LCD_WR_REG(0X2E);//9341 发送读GRAM指令
 	r=LCD_RD_DATA();								//dummy Read	   
 	r=LCD_RD_DATA();  		  						//实际坐标颜色
	//9341要分2次读出 
	b=LCD_RD_DATA(); 
	g=r&0XFF;		//对于9341第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8; 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510需要公式转换一下
}			 
/***************LCD显示开启***************************************/
void LCD_DisplayOn(void)
{
	LCD_WR_REG(0X29);
}
/**************LCD显示关闭****************************************/
void LCD_DisplayOff(void)
{
	LCD_WR_REG(0X28);
} 
/*******************设置光标位置************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_REG(lcddev.setxcmd);				//X坐标设置
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(Xpos&0xFF);
	
	LCD_WR_REG(lcddev.setycmd);				//Y坐标设置
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(Ypos&0XFF);
}	 
/********************************************************************************************************
//设置LCD的自动扫描方向																		//
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),		//	
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.	//
//dir:0~7,代表8个方向(具体定义见lcd.h)															//
*********************************************************************************************************/
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	switch(dir)
		{
			case L2R_U2D:
				regval|=(0<<7)|(0<<6)|(0<<5);
				break;
			case L2R_D2U:
				regval|=(1<<7)|(0<<6)|(0<<5);
				break;
			case R2L_U2D:
				regval|=(0<<7)|(1<<6)|(0<<5);
				break;
			case R2L_D2U:
				regval|=(1<<7)|(1<<6)|(0<<5);
				break;
			case U2D_L2R:
				regval|=(0<<7)|(0<<6)|(1<<5);
				break;
			case U2D_R2L:
				regval|=(0<<7)|(1<<6)|(1<<5);
				break;
			case D2U_L2R:
				regval|=(1<<7)|(0<<6)|(1<<5);
				break;
			case D2U_R2L:
				regval|=(1<<7)|(1<<6)|(1<<5);	
		}
	regval|=1<<3;                  //RGB,BGR颜色顺序设置
	LCD_WriteReg(0X36,regval);
}
/****************画点函数****************************************************************/
void LCD_DrawPoint(u16 x, u16 y)
{
	LCD_SetCursor(x,y);
	LCD_WriteRAM_Prepare();
	LCD->LCD_RAM=POINT_COLOR;
}
/************快速画点函数***************************************************************/
void LCD_Fast_DrawPoint(u16 x, u16 y, u32 color)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(x>>8);
	LCD_WR_DATA(x&0xFF);

	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(y>>8);
	LCD_WR_DATA(y&0XFF);

	LCD->LCD_REG=lcddev.wramcmd;
	LCD->LCD_RAM=color;
}
/***********设置LCD显示方向(0:竖屏1:横屏)***********************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)
		{
			lcddev.dir=0;
			lcddev.width=240;
			lcddev.height=320;
			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
		}
	else
		{
			lcddev.dir=1;
			lcddev.width=320;
			lcddev.height=240;
			lcddev.wramcmd=0x2C;
			lcddev.setxcmd=0X2B;
			lcddev.setycmd=0X2A;
		}
	LCD_Scan_Dir(L2R_U2D);
}
/******************************************************************************************************
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 																				
*******************************************************************************************************/
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;

	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(sx>>8);
	LCD_WR_DATA(sx&0xFF);
	LCD_WR_DATA(twidth>>8);
	LCD_WR_DATA(twidth&0xFF);

	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(sy>>8);
	LCD_WR_DATA(sy&0xFF);
	LCD_WR_DATA(theight>>8);
	LCD_WR_DATA(theight&0xFF);
	
}
//SRAM底层驱动，时钟使能，引脚分配
//此函数会被HAL_SRAM_Init()调用
//hsram:SRAM句柄
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{	
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FMC_CLK_ENABLE();				//使能FMC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
	
	//初始化PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//推挽复用
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//高速
	GPIO_Initure.Alternate=GPIO_AF12_FMC;	//复用为FMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
	
	//初始化PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}
        
//初始化lcd
//该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)
void LCD_Init(void)
{ 	  
	GPIO_InitTypeDef GPIO_Initure;
	FMC_NORSRAM_TimingTypeDef FMC_ReadWriteTim;
	FMC_NORSRAM_TimingTypeDef FMC_WriteTim;
    
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;  //PB5,背光控制。PB6,供电控制
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
    
	SRAM_Handler.Instance=FMC_NORSRAM_DEVICE;                
	SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;    
    
	SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //使用NE1
	SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE; //地址/数据线不复用
	SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;   //SRAM
	SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16位数据宽度
	SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;   //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //存储器写使能
	SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;           //等待使能位,此处未用到
	SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //读写使用不同的时序
	SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //禁止突发写
	SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    
	//FMC读时序控制寄存器
	FMC_ReadWriteTim.AddressSetupTime=0x0F;        //地址建立时间(ADDSET)为15个HCLK 1/180M*15=5.5ns*15=82.5ns
	FMC_ReadWriteTim.AddressHoldTime=0x00;
	FMC_ReadWriteTim.DataSetupTime=0x46;           //数据保存时间(DATAST)为70个HCLK	=5.5*70=385ns
	FMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //模式A
	//FMC写时序控制寄存器
	FMC_WriteTim.AddressSetupTime=0x0F;            //地址建立时间(ADDSET)为15个HCLK=82.5ns
	FMC_WriteTim.AddressHoldTime=0x00;
	FMC_WriteTim.DataSetupTime=0x0F;               //数据保存时间(DATAST)为5.5ns*15个HCLK=82.5ns
	FMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //模式A
	HAL_SRAM_Init(&SRAM_Handler,&FMC_ReadWriteTim,&FMC_WriteTim);		
	delay_ms(50); // delay 50 ms 
	
	//尝试9341 ID的读取		
	LCD_WR_REG(0XD3);				   
	lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();	//读到0X00
	lcddev.id=LCD_RD_DATA();   	//读取93								   
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//读取41 	 
	/*rt_kprintf(" LCD ID:%x\r\n",lcddev.id); //打印LCD ID */

	/***************ILI9341初始化*************************/
		LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on
		
	LCD_Display_Dir(0);		//默认为竖屏
	LCD_LED=0;				//点亮背光
	LCD_Clear(GBLUE);
}  
#ifdef BSP_USING_ILI9341
	INIT_BOARD_EXPORT(LCD_Init);
#endif

//清屏函数
//color:要清屏的填充色
void LCD_Clear(u32 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width; 
	totalpoint*=lcddev.height; 			//得到总点数
	LCD_SetCursor(0x00,0x0000);			//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	
	} 
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{          
	u16 i,j;
	u16 xlen=0; 
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//显示颜色 	    
	}  
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j; 
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	for(i=0;i<height;i++)
	{
		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//写入数据 
	} 
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	
	if(delta_x>0)
		incx=1; //设置单步方向 
	else if
		(delta_x==0)incx=0;//垂直线 
	else 
		{incx=-1;delta_x=-delta_x;} 

	
	if(delta_y>0)
		incy=1; 
	else if
		(delta_y==0)incy=0;//水平线 
	else
		{incy=-1;delta_y=-delta_y;} 
	
	if( delta_x>delta_y)
		distance=delta_x; //选取基本增量坐标轴 
	else 
		distance=delta_y; 
	
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
