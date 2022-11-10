#ifndef __FAN_H
#define __FAN_H	 
#include "sys.h"

#define FAN_PORT		GPIOA
#define FAN_RCC			RCC_APB2Periph_GPIOA
#define FAN_PIN			GPIO_Pin_8
#define FAN_PIN_MODE	GPIO_Mode_Out_PP

//蜂鸣器端口定义
#define FAN PAout(8)	// FAN,蜂鸣器接口		   
#define FAN_ON		1
#define FAN_OFF		0

char*  FAN_Init();	//初始化
		 				    
#endif

