#include "led.h"
#include "delay.h"
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
char* LED_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);	 //使能GPIOB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = LED0_PIN|LED1_PIN;				 //端口配置 LED0-->PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);	 

//	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);	 //使能GPIOB端口时钟
//	GPIO_InitStructure.GPIO_Pin = LED1_PIN|LED2_PIN;				 //LED1-->pb8  LED2-->pb9端口配置
//	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	 

	
	
	
	LED0 = LED0_ON	;
	delay_ms(500);
	LED0 = LED0_OFF	;
	return "LED success!";
//	LED1 = LED1_OFF;//关闭LED1输出
	
//	LED2 = LED2_OFF;//关闭LED2输出
}

 

