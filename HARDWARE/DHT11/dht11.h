#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

////IO操作函数											   
#define	DHT11_DQ_OUT PAout(15) //数据端口	PA15 
#define	DHT11_DQ_IN  PAin(15)  //数据端口	PA15

#define DHT11_GPIO_PORT    	GPIOA			              //GPIO端口
#define DHT11_GPIO_CLK 	    RCC_APB2Periph_GPIOA		//GPIO端口时钟
#define DHT11_GPIO_PIN		GPIO_Pin_15			        //连接到SCL时钟线的GPIO

char* DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *tempH,u8 *tempL,u8 *humiH,u8 *humiL);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif















