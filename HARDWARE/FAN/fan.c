#include "FAN.h"
#include "delay.h"

//初始化PB8为输出口.并使能这个口的时钟		    
//
char*  FAN_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(FAN_RCC, ENABLE);	 //使能GPIOA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = FAN_PIN;				 //FAN-->PA11 端口配置
	GPIO_InitStructure.GPIO_Mode = FAN_PIN_MODE; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	 //速度为50MHz
	GPIO_Init(FAN_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(FAN_PORT, FAN_PIN);	
	
	FAN = FAN_ON;
	delay_ms(500);
	FAN = FAN_OFF;//关闭
	return "FAN success!";
}
