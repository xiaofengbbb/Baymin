#include "led.h"
#include "delay.h"
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
char* LED_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = LED0_PIN|LED1_PIN;				 //�˿����� LED0-->PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);	 

//	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
//	GPIO_InitStructure.GPIO_Pin = LED1_PIN|LED2_PIN;				 //LED1-->pb8  LED2-->pb9�˿�����
//	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	 

	
	
	
	LED0 = LED0_ON	;
	delay_ms(500);
	LED0 = LED0_OFF	;
	return "LED success!";
//	LED1 = LED1_OFF;//�ر�LED1���
	
//	LED2 = LED2_OFF;//�ر�LED2���
}

 

