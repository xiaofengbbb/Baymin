#ifndef __FAN_H
#define __FAN_H	 
#include "sys.h"

#define FAN_PORT		GPIOA
#define FAN_RCC			RCC_APB2Periph_GPIOA
#define FAN_PIN			GPIO_Pin_8
#define FAN_PIN_MODE	GPIO_Mode_Out_PP

//�������˿ڶ���
#define FAN PAout(8)	// FAN,�������ӿ�		   
#define FAN_ON		1
#define FAN_OFF		0

char*  FAN_Init();	//��ʼ��
		 				    
#endif

