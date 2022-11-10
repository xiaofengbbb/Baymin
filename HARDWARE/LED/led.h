#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



//端口定义
#define LED0 PBout(8)	// LED0, 

#define LED0_ON		1
#define LED0_OFF	0


#define LED_PORT		GPIOB
#define LED_RCC		RCC_APB2Periph_GPIOB
#define LED0_PIN		GPIO_Pin_8
#define LED1_PIN		GPIO_Pin_9
//#define LED2_PIN		GPIO_Pin_9
//端口定义
#define LED1 PBout(9)	// LED1,
//#define LED2 PBout(9)	// LED1,

#define LED1_ON		1
#define LED1_OFF	0

//#define LED2_ON		1
//#define LED2_OFF	0

char* LED_Init();	//初始化


			    
#endif
