#include "beep.h"
#include "delay.h"
//��ʼ��PB8Ϊ�����.��ʹ������ڵ�ʱ��		    
//��������ʼ��
char* BEEP_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //BEEP-->PA1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
	
	BEEP = BEEP_ON;
	delay_ms(500);
	BEEP = BEEP_OFF;
	return "BEEP success!";
}
