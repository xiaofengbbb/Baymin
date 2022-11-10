#include "FAN.h"
#include "delay.h"

//��ʼ��PB8Ϊ�����.��ʹ������ڵ�ʱ��		    
//
char*  FAN_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(FAN_RCC, ENABLE);	 //ʹ��GPIOA�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = FAN_PIN;				 //FAN-->PA11 �˿�����
	GPIO_InitStructure.GPIO_Mode = FAN_PIN_MODE; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	 //�ٶ�Ϊ50MHz
	GPIO_Init(FAN_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(FAN_PORT, FAN_PIN);	
	
	FAN = FAN_ON;
	delay_ms(500);
	FAN = FAN_OFF;//�ر�
	return "FAN success!";
}
