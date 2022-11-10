#include "relay.h"
#include "delay.h"

char* RELAY_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RELAY_RCC, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = RELAY_PIN;				 //RELAY-->PA0 �˿�����
	GPIO_InitStructure.GPIO_Mode = RELAY_PIN_MODE; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
	GPIO_Init(RELAY_PORT, &GPIO_InitStructure);	 
	
	RELAY = RELAY_ON;
	delay_ms(500);
	RELAY = RELAY_OFF;
	return "RELAY success!";
}
