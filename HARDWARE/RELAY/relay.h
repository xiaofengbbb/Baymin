#ifndef __RELAY_H
#define __RELAY_H
#include "sys.h"

#define RELAY_PORT		GPIOA
#define RELAY_RCC		RCC_APB2Periph_GPIOA
#define RELAY_PIN		GPIO_Pin_12
#define RELAY_PIN_MODE	GPIO_Mode_Out_PP

//�̵����˿ڶ���
#define RELAY PAout(12)	// RELAY,�̵����ӿ�		   

#define RELAY_ON	0
#define RELAY_OFF	1

char* RELAY_Init();	//��ʼ��
		 				    
#endif

