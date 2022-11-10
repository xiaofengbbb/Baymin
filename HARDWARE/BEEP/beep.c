#include "beep.h"
#include "delay.h"
//初始化PB8为输出口.并使能这个口的时钟		    
//蜂鸣器初始化
char* BEEP_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能GPIOB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //BEEP-->PA1 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
	
	BEEP = BEEP_ON;
	delay_ms(500);
	BEEP = BEEP_OFF;
	return "BEEP success!";
}
