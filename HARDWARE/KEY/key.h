#ifndef _key_H
#define _key_H


#include "sys.h"
 
//#define KEY0_Pin    GPIO_Pin_0
//#define KEY1_Pin    GPIO_Pin_1    
#define KEY2_Pin    GPIO_Pin_2    
#define KEY3_Pin    GPIO_Pin_3   
#define KEY4_Pin    GPIO_Pin_4
   

#define KEY_Port GPIOB //定义端口


//使用位操作定义
#define KEY0 	PBin(0)
#define KEY1 	PBin(1)
#define KEY2 	PBin(2)
#define KEY3 	PBin(3)
#define KEY4 	PBin(4)



//定义各个按键值  
//#define KEY1_VALUE 	1
//#define KEY2_VALUE 	2
//  



char* KEY_Init();
u8 KEY_Scan(u8 mode);
#endif
