#ifndef __MAIN_H
#define __MAIN_H




#include "delay.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "timer.h"

#include "adc.h"

#include "led.h"
#include "beep.h"
#include "dht11.h"

#include "esp8266.h"
#include "onenet.h"
#include "relay.h"
#include "fan.h"



#include "key.h"

typedef struct{
	uint8_t tempH;		//温度
	uint8_t tempL;
	uint8_t humiH;
  uint8_t humiL;	//湿度
	uint8_t light; 	//光照度
	uint8_t mq;		//MQ传感器
	uint8_t led;	//LED
	uint8_t fan;	//风扇
	uint8_t relay;	//继电器
	uint8_t beep;
	uint8_t max_temp;//温度阈值
	uint8_t max_mq;//MQ传感器阈值
	uint8_t max_light;//light阈值
	uint8_t alarm_temp;  //温度报警标志  0无1有
	uint8_t alarm_mq;  //mq报警标志  0无1有
	uint8_t alarm_light;//光照报警标志  0无1有
	uint8_t mode;
}SendData;

extern SendData send_data;
extern uint8_t cmd_rev_flag;


void hardware_init(void);	
void net_init(void);
void dataview(void);
void data_get(void);
void data_display(void);
void data_user_handle(void);
void video_handle(void);
#endif



















